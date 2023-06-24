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
#pragma once

#include "Modules/ModuleManager.h"

#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/CoreDelegates.h"

#include "DLSSLibrary.generated.h"

class FDLSSUpscaler;
class FDelegateHandle;

#ifndef WITH_DLSS
#define WITH_DLSS 0
#endif

UENUM(BlueprintType)
enum class UDLSSSupport : uint8
{
	Supported UMETA(DisplayName = "Supported"),

	NotSupported UMETA(DisplayName = "Not Supported"),
	NotSupportedIncompatibleHardware UMETA(DisplayName = "Incompatible Hardware", ToolTip = "DLSS requires an NVIDIA RTX GPU"),
	NotSupportedDriverOutOfDate UMETA(DisplayName = "Driver Out of Date", ToolTip = "The driver is outdated. Also see GetDLSSMinimumDriverVersion"),
	NotSupportedOperatingSystemOutOfDate UMETA(DisplayName = "Operating System Out of Date", ToolTip = "DLSS requires at least Windows 10 Fall 2017 Creators Update 64-bit, (v1709, build 16299)"),
	NotSupportedByPlatformAtBuildTime UMETA(DisplayName = "Platform Not Supported At Build Time", ToolTip = "This platform doesn't not support DLSS at build time. Currently DLSS is only supported on Windows 64"),
	NotSupportedIncompatibleAPICaptureToolActive UMETA(DisplayName = "Incompatible API Capture Tool Active", ToolTip = "DLSS is not compatible with an active API capture tool such as RenderDoc.")
};


UENUM(BlueprintType)
enum class UDLSSMode : uint8
{
	Off              UMETA(DisplayName = "Off"),
	Auto             UMETA(DisplayName = "Auto", ToolTip = "Not a real quality mode. Use Auto to query best settings for a given resolution with GetDLSSModeInformation"),
	UltraQuality     UMETA(DisplayName = "Ultra Quality"),
	Quality          UMETA(DisplayName = "Quality"),
	Balanced         UMETA(DisplayName = "Balanced"),
	Performance      UMETA(DisplayName = "Performance"),
	UltraPerformance UMETA(DisplayName = "Ultra Performance")
};

UCLASS(MinimalAPI)
class  UDLSSLibrary : public UBlueprintFunctionLibrary
{
	friend class FDLSSBlueprintModule;
	GENERATED_BODY()
public:

	/** Checks whether DLSS/DLAA is supported by the current GPU. Further details can be retrieved via QueryDLSSSupport*/
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Is NVIDIA DLSS Supported"))
	static DLSSBLUEPRINT_API bool IsDLSSSupported();

	/** Checks whether DLSS/DLAA is supported by the current GPU	*/
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Query NVIDIA DLSS Support"))
	static DLSSBLUEPRINT_API UDLSSSupport QueryDLSSSupport();

	/** If QueryDLSSSupport returns "NotSupportedDriverOutOfDate", then MinDriverVersionMajor and MinDriverVersionMinor contains the required driver version.*/
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get DLSS Minimum DriverVersion"))
	static DLSSBLUEPRINT_API void GetDLSSMinimumDriverVersion(int32& MinDriverVersionMajor, int32& MinDriverVersionMinor);

	/**
	 * Enable/disable DLSS/DLAA
	 * To select a DLSS quality mode, set an appropriate upscale screen percentage with r.ScreenPercentage. Use GetDlssModeInformation to find optimal screen percentage
	 * To select DLAA, set the upscale screen percentage to 100 (r.ScreenPercentage=100)
	 */
	UFUNCTION(BlueprintCallable, Category = "DLSS", meta = (DisplayName = "Enable DLSS"))
	static DLSSBLUEPRINT_API void EnableDLSS(bool bEnabled);

	/** Checks whether DLSS/DLAA is enabled */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Is DLSS Enabled"))
	static DLSSBLUEPRINT_API bool IsDLSSEnabled();

	/** Enable/disable DLAA. Note that while DLAA is enabled, DLSS will be automatically disabled */
	UFUNCTION(BlueprintCallable, Category = "DLSS", meta = (DisplayName = "Enable DLAA", DeprecatedFunction, DeprecationMessage = "Use EnableDLSS instead"))
	static DLSSBLUEPRINT_API void EnableDLAA(bool bEnabled);

	/** Checks whether DLAA is enabled */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Is DLAA Enabled", DeprecatedFunction, DeprecationMessage = "Use IsDLSSEnabled instead"))
	static DLSSBLUEPRINT_API bool IsDLAAEnabled();

	/** Checks whether a DLSS mode is supported */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Is DLSS Mode Supported"))
	static DLSSBLUEPRINT_API bool IsDLSSModeSupported(UDLSSMode DLSSMode);

	/** Retrieves all supported DLSS modes. Can be used to populate UI */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get Supported DLSS Modes"))
	static DLSSBLUEPRINT_API TArray<UDLSSMode> GetSupportedDLSSModes();

	/** Provides additional details (such as screen percentage ranges) about a DLSS mode. Screen Resolution is required for Auto mode */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get DLSS Mode Information"))
	static DLSSBLUEPRINT_API void GetDLSSModeInformation(UDLSSMode DLSSMode, FVector2D ScreenResolution, bool& bIsSupported, float& OptimalScreenPercentage, bool& bIsFixedScreenPercentage, float& MinScreenPercentage, float& MaxScreenPercentage, float& OptimalSharpness);

	/** The global screen percentage range that DLSS supports. Excludes DLSS modes with fixed screen percentage. Also see GetDLSSModeInformation*/
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get DLSS Screenpercentage Range"))
	static DLSSBLUEPRINT_API void GetDLSSScreenPercentageRange(float& MinScreenPercentage, float& MaxScreenPercentage);

	/** Enables/disables DLSS */
	UFUNCTION(BlueprintCallable, Category = "DLSS", meta=(WorldContext="WorldContextObject", DisplayName = "Set DLSS Mode", DeprecatedFunction, DeprecationMessage = "Use EnableDLSS instead"))
	static DLSSBLUEPRINT_API void SetDLSSMode(UObject* WorldContextObject, UDLSSMode DLSSMode);

	/* Reads the current DLSS mode */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get DLSS Mode", DeprecatedFunction, DeprecationMessage = "Use IsDLSSEnabled instead"))
	static DLSSBLUEPRINT_API UDLSSMode GetDLSSMode();

	/* Sets the console variables to enable additional DLSS sharpening. Set to 0 to disable (r.NGX.DLSS.Sharpness) */
	UFUNCTION(BlueprintCallable, Category = "DLSS", meta = (DisplayName = "Set DLSS Sharpness", DeprecatedFunction, DeprecationMessage = "Use NIS sharpening instead"))
	static DLSSBLUEPRINT_API void SetDLSSSharpness(float Sharpness);
	
	/*Reads the console variables to infer the current DLSS sharpness (r.NGX.DLSS.Sharpness) */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get DLSS Sharpness", DeprecatedFunction, DeprecationMessage = "Use NIS sharpening instead"))
	static DLSSBLUEPRINT_API float GetDLSSSharpness();

	/* Find a reasonable default DLSS mode based on current hardware */
	UFUNCTION(BlueprintPure, Category = "DLSS", meta = (DisplayName = "Get Default DLSS Mode"))
	static DLSSBLUEPRINT_API UDLSSMode GetDefaultDLSSMode();

private:
	static UDLSSSupport DLSSSupport;

#if WITH_DLSS
	static int32 MinDLSSDriverVersionMinor;
	static int32 MinDLSSDriverVersionMajor;
	static FDLSSUpscaler* DLSSUpscaler;
	static bool bDLSSLibraryInitialized;

	static UDLSSMode CurrentDLSSModeDeprecated;
	static bool bDLAAEnabledDeprecated;

	static bool TryInitDLSSLibrary();

#if !UE_BUILD_SHIPPING
	struct FDLSSErrorState
	{
		bool bIsDLSSModeUnsupported = false;
		UDLSSMode InvalidDLSSMode = UDLSSMode::Off;
	};

	static FDLSSErrorState DLSSErrorState;

	static void GetDLSSOnScreenMessages(TMultiMap<FCoreDelegates::EOnScreenMessageSeverity, FText>& OutMessages);
	static FDelegateHandle DLSSOnScreenMessagesDelegateHandle;
#endif

#endif
};

class FDLSSBlueprintModule final : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
};
