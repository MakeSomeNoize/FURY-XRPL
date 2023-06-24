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

class FDLSSUpscaler;
class FDLSSDenoiser;
class ISceneViewExtension;
class FDLSSUpscalerViewExtension;
class FNGXAutomationViewExtension;
class NGXRHI;


enum class EDLSSSupport : uint8
{
	Supported,
	NotSupported,
	NotSupportedIncompatibleHardware,
	NotSupportedDriverOutOfDate,
	NotSupportedOperatingSystemOutOfDate,
	NotSupportedIncompatibleAPICaptureToolActive,
};


class IDLSSModuleInterface : public IModuleInterface
{
	public:
		virtual EDLSSSupport QueryDLSSSupport() const = 0;
		virtual void GetDLSSMinDriverVersion(int32& MajorVersion, int32& MinorVersion) const = 0;
		virtual float GetResolutionFractionForQuality(int32 Quality) const = 0;
		virtual FDLSSUpscaler* GetDLSSUpscaler() const = 0;
		virtual TSharedPtr< ISceneViewExtension, ESPMode::ThreadSafe> GetDLSSUpscalerViewExtension() const = 0;
};

class FDLSSModule final: public IDLSSModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule();
	virtual void ShutdownModule();

	// Inherited via IDLSSModuleInterface
	virtual EDLSSSupport QueryDLSSSupport() const;
	virtual void GetDLSSMinDriverVersion(int32& MajorVersion, int32& MinorVersion) const;
	virtual float GetResolutionFractionForQuality(int32 Quality) const;
	virtual FDLSSUpscaler* GetDLSSUpscaler() const;

	virtual TSharedPtr< ISceneViewExtension, ESPMode::ThreadSafe> GetDLSSUpscalerViewExtension() const;

private:

	TUniquePtr<FDLSSUpscaler> DLSSUpscaler;
	TUniquePtr<FDLSSDenoiser> DLSSDenoiser;
	TUniquePtr<NGXRHI> NGXRHIExtensions;
	TSharedPtr< FDLSSUpscalerViewExtension, ESPMode::ThreadSafe> DLSSUpscalerViewExtension;
	TSharedPtr< FNGXAutomationViewExtension, ESPMode::ThreadSafe> NGXAutomationViewExtension;
	EDLSSSupport DLSSSupport = EDLSSSupport::NotSupported;
	int32 MinDriverVersionMinor;
	int32 MinDriverVersionMajor;
};
