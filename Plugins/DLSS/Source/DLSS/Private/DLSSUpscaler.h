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

#include "CoreMinimal.h"
#include "SceneViewExtension.h"
#include "RendererInterface.h"
#include "PostProcess/TemporalAA.h"
#include "ScreenPass.h"
#include "NGXRHI.h"

#include "CustomResourcePool.h"

class FSceneTextureParameters;

struct FTemporalAAHistory;

class FRHITexture;
class NGXRHI;
struct FDLSSOptimalSettings;
class FDLSSUpscaler;

struct FDLSSPassParameters
{
	FIntRect InputViewRect;
	FIntRect OutputViewRect;
	bool bHighResolutionMotionVectors = false;

	FRDGTexture* SceneColorInput = nullptr;
	FRDGTexture* SceneVelocityInput = nullptr;
	FRDGTexture* SceneDepthInput = nullptr;

	FDLSSPassParameters(const FViewInfo& View)
		: InputViewRect(View.ViewRect)
		, OutputViewRect(FIntPoint::ZeroValue, View.GetSecondaryViewRectSize())
	{
	}

	/** Returns the texture resolution that will be output. */
	FIntPoint GetOutputExtent() const;

	/** Validate the settings of TAA, to make sure there is no issue. */
	bool Validate() const;
};

struct FDLSSOutputs
{
	FRDGTexture* SceneColor = nullptr;
};

enum class EDLSSQualityMode
{
	MinValue = -2,
	UltraPerformance = -2,
	Performance = -1,
	Balanced = 0,
	Quality = 1,
	UltraQuality = 2,
	MaxValue = UltraQuality,
	NumValues = 5
};

class DLSS_API FDLSSUpscalerViewExtension final : public FSceneViewExtensionBase
{
public:
	FDLSSUpscalerViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
	{ }

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) final override {}
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) final override {}
	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;
};

class DLSS_API FDLSSSceneViewFamilyUpscaler final : public ITemporalUpscaler
{
public:
	FDLSSSceneViewFamilyUpscaler(const FDLSSUpscaler* InUpscaler, EDLSSQualityMode InDLSSQualityMode)
		: Upscaler(InUpscaler)
		, DLSSQualityMode(InDLSSQualityMode)
	{ }

	virtual const TCHAR* GetDebugName() const final override;
	virtual float GetMinUpsampleResolutionFraction() const final override;
	virtual float GetMaxUpsampleResolutionFraction() const final override;
	virtual ITemporalUpscaler* Fork_GameThread(const class FSceneViewFamily& ViewFamily) const final override;
	virtual ITemporalUpscaler::FOutputs AddPasses(
		FRDGBuilder& GraphBuilder,
		const FViewInfo& View,
		const FPassInputs& PassInputs) const final override;

	static bool IsDLSSTemporalUpscaler(const ITemporalUpscaler* TemporalUpscaler);

private:
	const FDLSSUpscaler* Upscaler;
	const EDLSSQualityMode DLSSQualityMode;

	FDLSSOutputs AddDLSSPass(
		FRDGBuilder& GraphBuilder,
		const FViewInfo& View,
		const FDLSSPassParameters& Inputs,
		const FTemporalAAHistory& InputHistory,
		FTemporalAAHistory* OutputHistory,
		const TRefCountPtr<ICustomTemporalAAHistory> InputCustomHistoryInterface,
		TRefCountPtr<ICustomTemporalAAHistory>* OutputCustomHistoryInterface
	) const;
};

class DLSS_API FDLSSUpscaler final : public ICustomResourcePool
{

	friend class FDLSSModule;
public:

	void SetupViewFamily(FSceneViewFamily& ViewFamily);

	float GetOptimalResolutionFractionForQuality(EDLSSQualityMode Quality) const;
	float GetOptimalSharpnessForQuality(EDLSSQualityMode Quality) const;
	float GetMinResolutionFractionForQuality(EDLSSQualityMode Quality) const;
	float GetMaxResolutionFractionForQuality(EDLSSQualityMode Quality) const;
	bool IsFixedResolutionFraction(EDLSSQualityMode Quality) const;
	
	const NGXRHI* GetNGXRHI() const
	{
		return NGXRHIExtensions;
	}

	// Inherited via ICustomResourcePool
	virtual void Tick(FRHICommandListImmediate& RHICmdList) override;

	bool IsQualityModeSupported(EDLSSQualityMode InQualityMode) const;
	uint32 GetNumRuntimeQualityModes() const
	{
		return NumRuntimeQualityModes;
	}

	bool IsDLSSActive() const;

	// Give the suggested EDLSSQualityMode if one is appropriate for the given pixel count, or nothing if DLSS should be disabled
	TOptional<EDLSSQualityMode> GetAutoQualityModeFromPixels(int PixelCount) const;

	static void ReleaseStaticResources();

	static float GetMinUpsampleResolutionFraction()
	{
		return MinDynamicResolutionFraction;
	}

	static float GetMaxUpsampleResolutionFraction()
	{
		return MaxDynamicResolutionFraction;
	}

private:
	FDLSSUpscaler(NGXRHI* InNGXRHIExtensions);
	FDLSSUpscaler(const FDLSSUpscaler&) = default;
	

	bool EnableDLSSInPlayInEditorViewports() const;

	// The FDLSSUpscaler(NGXRHI*) will update those once
	static NGXRHI* NGXRHIExtensions;
	static float MinDynamicResolutionFraction;
	static float MaxDynamicResolutionFraction;

	static uint32 NumRuntimeQualityModes;
	static TArray<FDLSSOptimalSettings> ResolutionSettings;
	float PreviousResolutionFraction;

	friend class FDLSSUpscalerViewExtension;
	friend class FDLSSSceneViewFamilyUpscaler;
};

