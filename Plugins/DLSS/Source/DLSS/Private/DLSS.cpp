/*
* Copyright (c) 2020 - 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
*
* NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
* property and proprietary rights in and to this material, related
* documentation and any modifications thereto. Any use, reproduction,
* disclosure or distribution of this material and related documentation
* without an express license agreement from NVIDIA CORPORATION or
* its affiliates is strictly prohibited.
*/
#include "DLSS.h"
#include "CoreMinimal.h"
#include "DLSSUpscalerPrivate.h"

#include "DLSSUpscaler.h"
#include "DLSSDenoiser.h"

#include "NGXRHI.h"

#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "GeneralProjectSettings.h"
#include "DLSSSettings.h"

#include "SceneViewExtension.h"
#include "SceneView.h"
#include "Misc/MessageDialog.h"


#define LOCTEXT_NAMESPACE "FDLSSModule"
DEFINE_LOG_CATEGORY(LogDLSS);

static TAutoConsoleVariable<int32> CVarNGXBinarySearchOrder(
	TEXT("r.NGX.BinarySearchOrder"),
	0,
	TEXT("0: automatic: (default)\n")
	TEXT("   use custom binaries from project and launch folder $(ProjectDir)/Binaries/ThirdParty/NVIDIA/NGX/$(Platform) if present\n")
	TEXT("   fallback to generic binaries from plugin folder\n")
	TEXT("1: force generic binaries from plugin folder, fail if not found\n")
	TEXT("2: force custom binaries from project or launch folder, fail if not found\n")
	TEXT("3: force generic development binaries from plugin folder, fail if not found. This is only supported in non-shipping build configurations\n"),
		ECVF_ReadOnly);

static TAutoConsoleVariable<int32> CVarNGXEnable(
	TEXT("r.NGX.Enable"), 1,
	TEXT("Whether the NGX library should be loaded. This allow to have the DLSS plugin enabled but avoiding potential ")
	TEXT("incompatibilities by skipping the driver side NGX parts of DLSS. Can also be set on the command line via -ngxenable and -ngxdisable"),
	ECVF_ReadOnly);

static TAutoConsoleVariable<int32> CVarNGXDLSSMinimumWindowsBuildVersion(
	TEXT("r.NGX.DLSS.MinimumWindowsBuildVersion"), 16299,
	TEXT("Sets the minimum Windows 10 build version required to enable DLSS. (default: 16299 for v1709, Windows 10 Fall 2017 Creators Update 64-bit)"),
	ECVF_Default
);

static TAutoConsoleVariable<int32> CVarNGXProjectIdentifier(
	TEXT("r.NGX.ProjectIdentifier"),
	0,
	TEXT("0: automatic: (default)\n")
	TEXT("   use NVIDIA NGX Application ID if non-zero, otherwise use UE Project ID)\n")
	TEXT("1: force UE Project ID\n")
	TEXT("2: force NVIDIA NGX Application ID (set via the Project Settings -> NVIDIA DLSS plugin)\n"),
	ECVF_ReadOnly);


static TAutoConsoleVariable<bool> CVarNGXAutomationEnable(
	TEXT("r.NGX.Automation.Enable"),
	false,
	TEXT("Enable automation for NGX DLSS image quality and performance evaluation. (default = 0)\n"),
	ECVF_RenderThreadSafe);

static TAutoConsoleVariable<int> CVarNGXAutomationViewIndex(
	TEXT("r.NGX.Automation.ViewIndex"),
	0,
	TEXT("Select which view to use with NGX DLSS image quality and performance automation. (default = 0). \n"),
	ECVF_RenderThreadSafe);

static TAutoConsoleVariable<int> CVarNGXAutomationNonGameViews(
	TEXT("r.NGX.Automation.NonGameViews"),
	0,
	TEXT("Enable NGX DLSS image quality and performance automation for non-game views. (default = 0). \n"),
	ECVF_RenderThreadSafe);


class FNGXAutomationViewExtension final : public FSceneViewExtensionBase
{
public:
	FNGXAutomationViewExtension(const FAutoRegister& AutoRegister): FSceneViewExtensionBase(AutoRegister)	
	{
		FSceneViewExtensionIsActiveFunctor IsActiveFunctor;

		IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
		{
			return CVarNGXAutomationEnable.GetValueOnAnyThread();
		};

		IsActiveThisFrameFunctions.Add(IsActiveFunctor);
	}

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) {}
	virtual void SetupViewPoint(APlayerController* Player, FMinimalViewInfo& InViewInfo) {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) {}
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) final {}
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) final
	{
		int32 ViewIndex = CVarNGXAutomationViewIndex.GetValueOnRenderThread();

		if (InViewFamily.Views.IsValidIndex(ViewIndex))
		{
			const FSceneView* View = InViewFamily.Views[ViewIndex];
			if (View->bIsGameView || CVarNGXAutomationNonGameViews.GetValueOnRenderThread())
			{
				const FString AiAgentMarker = FString::Printf(TEXT("{\"camera\":{\"position\":{\"x\": %f, \"y\": %f, \"z\": %f},\"rotation\":{\"pitch\": %f, \"roll\": %f, \"yaw\": %f}}}"),
					View->ViewLocation.X, View->ViewLocation.Y, View->ViewLocation.Z,
					View->ViewRotation.Pitch, View->ViewRotation.Roll, View->ViewRotation.Yaw);

				static FColor ColorMarker = FColor::FromHex("0xA1A5E87");
				FRHICommandListImmediate& RHICmdList = GraphBuilder.RHICmdList;
				RHICmdList.PushEvent(*AiAgentMarker, ColorMarker);
				RHICmdList.PopEvent();
			}
		}
	}
};

void FDLSSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogDLSS, Log, TEXT("%s Enter"), ANSI_TO_TCHAR(__FUNCTION__));

	if (!GDynamicRHI)
	{
		UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX DLSS requires an RHI"));
		DLSSSupport = EDLSSSupport::NotSupported;
		return;
	}

	// Get the base directory of this plugin
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("DLSS"))->GetBaseDir();
	const FString NGXBinariesDir = FPaths::Combine(*PluginBaseDir, TEXT("Binaries/ThirdParty/Win64/"));
	const FString RHIName = GDynamicRHI->GetName();

	UE_LOG(LogDLSS, Log, TEXT("PluginBaseDir %s"), *PluginBaseDir);
	UE_LOG(LogDLSS, Log, TEXT("NGXBinariesDir %s"), *NGXBinariesDir);
	UE_LOG(LogDLSS, Log, TEXT("GDynamicRHIName %s %s"), RHIVendorIdToString(), *RHIName );

	bool bLoadLibraries = CVarNGXEnable.GetValueOnAnyThread() != 0;
	if (FParse::Param(FCommandLine::Get(), TEXT("ngxenable")))
	{
		bLoadLibraries = true;
	}
	else if (FParse::Param(FCommandLine::Get(), TEXT("ngxdisable")))
	{
		bLoadLibraries = false;
	}

	const int32 NGXDLSSMinimumWindowsBuildVersion = CVarNGXDLSSMinimumWindowsBuildVersion.GetValueOnAnyThread();
	
	if (!IsRHIDeviceNVIDIA())
	{
		UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX DLSS requires an NVIDIA RTX series graphics card"));
		DLSSSupport = EDLSSSupport::NotSupportedIncompatibleHardware;
	}
#if PLATFORM_WINDOWS
	else if (NGXDLSSMinimumWindowsBuildVersion > 0 && !FPlatformMisc::VerifyWindowsVersion(10, 0
		#if PLATFORM_DESKTOP
		, NGXDLSSMinimumWindowsBuildVersion
		#endif
	))
	{
		// From https://docs.microsoft.com/en-us/windows/release-information/
		UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX DLSS requires at least Windows 10, build %u "), NGXDLSSMinimumWindowsBuildVersion);
		DLSSSupport = EDLSSSupport::NotSupportedOperatingSystemOutOfDate;
	}
#endif
	else if (!bLoadLibraries)
	{
		UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX library loading has been disabled with r.NGX.Enable=0"));
		DLSSSupport = EDLSSSupport::NotSupported;
	}
	else
	{
		const ERHIInterfaceType RHIType = RHIGetInterfaceType();

		const bool bIsDX12 = (RHIType == ERHIInterfaceType::D3D12) && GetDefault<UDLSSSettings>()->bEnableDLSSD3D12;
		const bool bIsDX11 = (RHIType == ERHIInterfaceType::D3D11) && GetDefault<UDLSSSettings>()->bEnableDLSSD3D11;
		const bool bIsVulkan = (RHIType == ERHIInterfaceType::Vulkan) && GetDefault<UDLSSSettings>()->bEnableDLSSVulkan;
		const TCHAR* NGXRHIModuleName = nullptr;

		DLSSSupport = (bIsDX11 || bIsDX12 || bIsVulkan) ? EDLSSSupport::Supported : EDLSSSupport::NotSupported; 

		if (DLSSSupport == EDLSSSupport::Supported)
		{
			if (bIsDX11)
			{
				NGXRHIModuleName = TEXT("NGXD3D11RHI");
			}
			else if (bIsDX12)
			{
				NGXRHIModuleName = TEXT("NGXD3D12RHI");
			}
			else if (bIsVulkan)
			{
				NGXRHIModuleName = TEXT("NGXVulkanRHI");
			}


			uint32 NGXAppID = GetDefault<UDLSSSettings>()->NVIDIANGXApplicationId;
			UE_LOG(LogDLSS, Log, TEXT("Plugin settings: NGXAppId = %u"), NGXAppID);

			if (NGXAppID != 0)
			{
				UE_LOG(LogDLSS, Warning, TEXT("NGX Application ID is specified, overriding the Project ID. Please refer to https://developer.nvidia.com/dlss."));
			}
			else
			{
				UE_LOG(LogDLSS, Warning, TEXT("NGX Application ID not specified, using the Project ID by default."));
			}

			bool bAllowOTAUpdate = GetDefault<UDLSSSettings>()->bAllowOTAUpdate;
			if (FParse::Param(FCommandLine::Get(), TEXT("ngxenableota")))
			{
				bAllowOTAUpdate = true;
			}
			else if (FParse::Param(FCommandLine::Get(), TEXT("ngxdisableota")))
			{
				bAllowOTAUpdate = false;
			}

			FNGXRHICreateArguments Arguments;
			Arguments.PluginBaseDir = PluginBaseDir;
			Arguments.DynamicRHI = GDynamicRHI;
			Arguments.NGXBinariesSearchOrder = ENGXBinariesSearchOrder(FMath::Clamp(CVarNGXBinarySearchOrder.GetValueOnAnyThread(), int32(ENGXBinariesSearchOrder::MinValue), int32(ENGXBinariesSearchOrder::MaxValue)));

			Arguments.ProjectIdentifier = ENGXProjectIdentifier(FMath::Clamp(CVarNGXProjectIdentifier.GetValueOnAnyThread(), int32(ENGXProjectIdentifier::MinValue), int32(ENGXProjectIdentifier::MaxValue)));
			Arguments.NGXAppId = NGXAppID;
			Arguments.UnrealEngineVersion = FString::Printf(TEXT("%u.%u"), FEngineVersion::Current().GetMajor(), FEngineVersion::Current().GetMinor());
			Arguments.UnrealProjectID = GetDefault<UGeneralProjectSettings>()->ProjectID.ToString();
			Arguments.bAllowOTAUpdate = bAllowOTAUpdate;

			INGXRHIModule* NGXRHIModule = &FModuleManager::LoadModuleChecked<INGXRHIModule>(NGXRHIModuleName);
			NGXRHIExtensions = NGXRHIModule->CreateNGXRHI(Arguments);


			const bool bRenderDocPluginFound = FModuleManager::Get().ModuleExists(TEXT("RenderDocPlugin"));
			const bool bDLSSUnavailable = (!NGXRHIExtensions || !NGXRHIExtensions->IsDLSSAvailable());
			const bool bIncompatibleAPICaptureToolActive = (bRenderDocPluginFound || NGXRHI::IsIncompatibleAPICaptureToolActive());
			if (bDLSSUnavailable && bIncompatibleAPICaptureToolActive)
			{
				if (bRenderDocPluginFound)
				{
					UE_LOG(LogDLSS, Warning, TEXT("DLSS is not compatible with the RenderDoc plugin. To enable DLSS please disable the RenderDoc plugin."));
				}

				if (NGXRHI::IsIncompatibleAPICaptureToolActive())
				{
					UE_LOG(LogDLSS, Warning, TEXT("DLSS is not compatible with some API capture tools, such as RenderDoc. To enable DLSS please use an API capture tool such as NVIDIA NSIGHT graphics, which support the APIs required by DLSS."));
				}
// we don't want this ever show up in packaged builds
#if WITH_EDITOR
				const bool IsUnattended =  FApp::IsUnattended() || IsRunningCommandlet() || GIsRunningUnattendedScript;
				auto LocalSettings = GetMutableDefault<UDLSSOverrideSettings>();
				if (!IsUnattended && LocalSettings->bShowDLSSIncompatiblePluginsToolsWarnings)
				{
					const FText DialogTitle(LOCTEXT("DLSSIncompatibleWithCaptureToolTitle", "Warning -- DLSS incompatible with plugins and/or API capture tools"));
					const FText WarningMessage(LOCTEXT("DLSSIncompatibleWithCaptureTool",
						"To enable DLSS, please disable the RenderDoc plugin and/or use an API capture tool such as NVIDIA NSIGHT graphics, which supports the APIs required by DLSS.\n\n"
						"Would you like to continue to see this message for this project? It can also be disabled in the NVIDIA DLSS Overrides (Local) plugin settings."));

					EAppReturnType::Type YesNoCancelReply = FMessageDialog::Open(EAppMsgType::YesNoCancel, EAppReturnType::Yes, WarningMessage, &DialogTitle);
				
					if(YesNoCancelReply == EAppReturnType::No)
					{
						LocalSettings->bShowDLSSIncompatiblePluginsToolsWarnings = false; 
					}
					else if (YesNoCancelReply == EAppReturnType::Cancel)
					{
						FPlatformMisc::RequestExit(true);
					}
				}
#endif //WITH_EDITOR
			}

			if (NGXRHIExtensions)
			{
				if (NGXRHIExtensions->IsDLSSAvailable())
				{
					check(NVSDK_NGX_SUCCEED(NGXRHIExtensions->GetDLSSInitResult()));
					DLSSSupport = EDLSSSupport::Supported;
				}
				else
				{
					// map some of the NGX error codes to something that the UI/gameplay could suggest the end user to do something about
					if (bIncompatibleAPICaptureToolActive)
					{
						DLSSSupport = EDLSSSupport::NotSupportedIncompatibleAPICaptureToolActive;
					}
					else if (NVSDK_NGX_Result_FAIL_OutOfDate == NGXRHIExtensions->GetDLSSInitResult())
					{
						DLSSSupport = EDLSSSupport::NotSupportedDriverOutOfDate;
					} 
					else if (NVSDK_NGX_Result_FAIL_FeatureNotSupported == NGXRHIExtensions->GetDLSSInitResult())
					{
						DLSSSupport = EDLSSSupport::NotSupportedIncompatibleHardware;
					}
					else
					{
						DLSSSupport =  EDLSSSupport::NotSupported;
					}
				}

				const FNGXDriverRequirements DriverRequirements = NGXRHIExtensions->GetDLSSDriverRequirements();
				MinDriverVersionMajor = DriverRequirements.MinDriverVersionMajor;
				MinDriverVersionMinor = DriverRequirements.MinDriverVersionMinor;
				if (DriverRequirements.DriverUpdateRequired)
				{
					if (DLSSSupport == EDLSSSupport::Supported)
					{
						UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX DLSS could be loaded properly. However it is recommended to update the version to at least: %u.%u"), DriverRequirements.MinDriverVersionMajor, DriverRequirements.MinDriverVersionMinor);
					}
					else
					{
						UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX DLSS cannot be loaded properly. Please verify that at least this driver version is installed: %u.%u"), DriverRequirements.MinDriverVersionMajor, DriverRequirements.MinDriverVersionMinor);
						DLSSSupport = EDLSSSupport::NotSupportedDriverOutOfDate;
						MinDriverVersionMajor = DriverRequirements.MinDriverVersionMajor;
						MinDriverVersionMinor = DriverRequirements.MinDriverVersionMinor;
					}
					
				}
			}
			else
			{
				UE_LOG(LogDLSS, Log, TEXT("Could not load %s module"), NGXRHIModuleName);
				DLSSSupport = EDLSSSupport::NotSupported;
			}

			if (DLSSSupport != EDLSSSupport::Supported)
			{
				UE_LOG(LogDLSS, Log, TEXT("DLSS not supported by the %s %s RHI in the %s module at runtime"), RHIVendorIdToString(), *RHIName, NGXRHIModuleName);
			}
		}
		else
		{
			UE_LOG(LogDLSS, Log, TEXT("DLSS not implemented for the %s RHI, or disabled in the project settings"), *RHIName);
			DLSSSupport = EDLSSSupport::NotSupported;
		}
	}

	
	if (DLSSSupport == EDLSSSupport::Supported)
	{
		// set the upscaler
		{
			DLSSUpscaler.Reset(new FDLSSUpscaler(NGXRHIExtensions.Get()));
			check(DLSSUpscaler);
			if (DLSSUpscaler->GetNumRuntimeQualityModes() == 0)
			{
				UE_LOG(LogDLSS, Log, TEXT("DLSS not available due to not supporting any quality modes at runtime"));
				DLSSSupport = EDLSSSupport::NotSupported;
				DLSSUpscaler.Reset();
				NGXRHIExtensions.Reset();
			}
		}
	}

	UE_LOG(LogDLSS, Log, TEXT("NVIDIA NGX DLSS supported %u"), QueryDLSSSupport() == EDLSSSupport::Supported);

	// and the other related interfaces

	if (DLSSSupport == EDLSSSupport::Supported)
	{
		//	set up the view extension for setting up the FDLSSUpscaler on FSceneViewFamily
		{
			DLSSUpscalerViewExtension = FSceneViewExtensions::NewExtension<FDLSSUpscalerViewExtension>();
		}

		// set the denoiser
		{

			static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Reflections.Denoiser"));
			if (CVar && (CVar->GetInt() != 2))
			{
				UE_LOG(LogDLSS, Warning, TEXT("For optimal results NVIDIA NGX DLSS requires the use of a custom denoiser plugin, which currently is disabled. To enable, set r.Reflections.Denoiser=2"));
			}

			DLSSDenoiser.Reset(new FDLSSDenoiser(GScreenSpaceDenoiser, DLSSUpscaler.Get()));
			GScreenSpaceDenoiser = DLSSDenoiser.Get();
			UE_LOG(LogDLSS, Log, TEXT("%s wrapping %s"), DLSSDenoiser->GetDebugName(), DLSSDenoiser->GetWrappedDenoiser()->GetDebugName());
		}

		// set the resource pool
		{
			checkf(GCustomResourcePool == nullptr, TEXT("GCustomResourcePool is already in use. Please check that only one upscaling plugin is active."));
			GCustomResourcePool = DLSSUpscaler.Get();
		}
	}
	
	// setup DLSS image quality and performance automation hooks
	{
		NGXAutomationViewExtension = FSceneViewExtensions::NewExtension<FNGXAutomationViewExtension>();
	}

	UE_LOG(LogDLSS, Log, TEXT("%s Leave"), ANSI_TO_TCHAR(__FUNCTION__));
}

void FDLSSModule::ShutdownModule()
{
	UE_LOG(LogDLSS, Log, TEXT("%s Enter"), ANSI_TO_TCHAR(__FUNCTION__));

	// reset DLSS image quality and performance automation hooks
	{
		NGXAutomationViewExtension = nullptr;
	}

	if (QueryDLSSSupport() == EDLSSSupport::Supported)
	{
		// reset the view extension
		{
			DLSSUpscalerViewExtension = nullptr;
		}

		// reset the resource pool
		if (GCustomResourcePool == DLSSUpscaler.Get())
		{
			GCustomResourcePool = nullptr;
		}

		// reset the denoiser
		if (GScreenSpaceDenoiser == DLSSDenoiser.Get())
		{
			UE_LOG(LogDLSS, Log, TEXT("%s unwrapping %s"), DLSSDenoiser->GetDebugName(), DLSSDenoiser->GetWrappedDenoiser()->GetDebugName());
			GScreenSpaceDenoiser = DLSSDenoiser->GetWrappedDenoiser();
			DLSSDenoiser.Reset();
		}

		// reset the upscaler
		{
			FDLSSUpscaler::ReleaseStaticResources();
			DLSSUpscaler.Reset();
		}

		NGXRHIExtensions.Reset();
	}

	UE_LOG(LogDLSS, Log, TEXT("%s Leave"), ANSI_TO_TCHAR(__FUNCTION__));
}


EDLSSSupport FDLSSModule::QueryDLSSSupport() const
{
	return DLSSSupport;
}


void FDLSSModule::GetDLSSMinDriverVersion(int32& OutMajorVersion, int32& OutMinorVersion) const
{
	OutMajorVersion = MinDriverVersionMajor;
	OutMinorVersion = MinDriverVersionMinor;
}


float FDLSSModule::GetResolutionFractionForQuality(int32 Quality) const
{
	checkf(QueryDLSSSupport() == EDLSSSupport::Supported,TEXT("GetResolutionFractionForQuality should only be called when DLSS is supported"));
	checkf(Quality >= int32(EDLSSQualityMode::MinValue) && Quality <= int32(EDLSSQualityMode::MaxValue),TEXT("The Quality %d  argument of GetResolutionFractionForQuality is out of range and/or an unsupported DLSS quality mode"), Quality);
	return DLSSUpscaler->GetOptimalResolutionFractionForQuality(EDLSSQualityMode(Quality));
}

FDLSSUpscaler* FDLSSModule::GetDLSSUpscaler() const
{
	return DLSSUpscaler.Get();
}

TSharedPtr< ISceneViewExtension, ESPMode::ThreadSafe> FDLSSModule::GetDLSSUpscalerViewExtension() const
{
	return StaticCastSharedPtr<ISceneViewExtension>(DLSSUpscalerViewExtension);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDLSSModule, DLSS)
	

