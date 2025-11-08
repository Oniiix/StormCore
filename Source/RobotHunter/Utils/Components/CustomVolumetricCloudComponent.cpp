// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomVolumetricCloudComponent.h"

#include "Engine/Texture2D.h"
#include "VolumetricCloudProxy.h"
#include "Components/BillboardComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "RenderingThread.h"
#include "UObject/UE5ReleaseStreamObjectVersion.h"
#include "SceneInterface.h"
#include "UObject/UE5MainStreamObjectVersion.h"



/*=============================================================================
	UVolumetricCloudComponent implementation.
=============================================================================*/

UCustomVolumetricCloudComponent::UCustomVolumetricCloudComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, LayerBottomAltitude(5.0f)
	, LayerHeight(10.0f)
	, TracingStartMaxDistance(350.0f)
	, TracingStartDistanceFromCamera(0.0f)
	, TracingMaxDistanceMode(EVolumetricCloudTracingMaxDistanceMode::DistanceFromCloudLayerEntryPoint)
	, TracingMaxDistance(50.0f)
	, PlanetRadius(6360.0f)					// Default to earth-like
	, GroundAlbedo(FColor(170, 170, 170))	// 170 => 0.4f linear
	, Material(nullptr)
	, bUsePerSampleAtmosphericLightTransmittance(false)
	, SkyLightCloudBottomOcclusion(0.5f)
	, ViewSampleCountScale(1.0f)
	, ReflectionViewSampleCountScaleValue(1.0f)
	, ReflectionViewSampleCountScale_DEPRECATED(0.15f)		// Roughly equivalent to previous default 1.0f, scaled by OldToNewReflectionViewRaySampleCount
	, ReflectionSampleCountScale_DEPRECATED(1.0f)
	, ShadowViewSampleCountScale(1.0f)
	, ShadowReflectionViewSampleCountScaleValue(1.0f)
	, ShadowReflectionViewSampleCountScale_DEPRECATED(0.3f)	// Roughly equivalent to previous default 1.0f, scaled by OldToNewReflectionShadowRaySampleCount
	, ShadowReflectionSampleCountScale_DEPRECATED(1.0f)
	, ShadowTracingDistance(15.0f)
	, StopTracingTransmittanceThreshold(0.005f)
	, AerialPespectiveRayleighScatteringStartDistance(0.0f)
	, AerialPespectiveRayleighScatteringFadeDistance(0.0f)
	, AerialPespectiveMieScatteringStartDistance(0.0f)
	, AerialPespectiveMieScatteringFadeDistance(0.0f)
	, bHoldout(false)
	, bRenderInMainPass(true)
	, VolumetricCloudSceneProxy(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> VolumetricCloudDefaultMaterialRef(TEXT("/Engine/EngineSky/VolumetricClouds/m_SimpleVolumetricCloud_Inst.m_SimpleVolumetricCloud_Inst"));
	Material = VolumetricCloudDefaultMaterialRef.Object;
}


#define CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(MemberType, MemberName) void UCustomVolumetricCloudComponent::Set##MemberName(MemberType NewValue)\
{\
	if (AreDynamicDataChangesAllowed() && MemberName != NewValue)\
	{\
		MemberName = NewValue;\
		MarkRenderStateDirty();\
	}\
}\

CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, LayerBottomAltitude);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, LayerHeight);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, TracingStartMaxDistance);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, TracingStartDistanceFromCamera);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, TracingMaxDistance);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, PlanetRadius);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(FColor, GroundAlbedo);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(bool, bUsePerSampleAtmosphericLightTransmittance);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, SkyLightCloudBottomOcclusion);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, ViewSampleCountScale);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, ShadowViewSampleCountScale);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, ShadowTracingDistance);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(float, StopTracingTransmittanceThreshold);
CLOUD_DECLARE_BLUEPRINT_SETFUNCTION(UMaterialInterface*, Material);

void UCustomVolumetricCloudComponent::SetHoldout(bool bNewHoldout)
{
	if (bHoldout != bNewHoldout)
	{
		bHoldout = bNewHoldout;
		MarkRenderStateDirty();
	}
}

void UCustomVolumetricCloudComponent::SetRenderInMainPass(bool bValue)
{
	if (bRenderInMainPass != bValue)
	{
		bRenderInMainPass = bValue;
		MarkRenderStateDirty();
	}
}

void UCustomVolumetricCloudComponent::SetReflectionViewSampleCountScale(float NewValue)
{
	if (AreDynamicDataChangesAllowed() && ReflectionViewSampleCountScaleValue != NewValue)
	{
		ReflectionViewSampleCountScaleValue = NewValue;
		MarkRenderStateDirty();
	}
}
void UCustomVolumetricCloudComponent::SetShadowReflectionViewSampleCountScale(float NewValue)
{
	if (AreDynamicDataChangesAllowed() && ShadowReflectionViewSampleCountScaleValue != NewValue)
	{
		ShadowReflectionViewSampleCountScaleValue = NewValue;
		MarkRenderStateDirty();
	}
}

void UCustomVolumetricCloudComponent::SetReflectionSampleCountScale(float NewValue)
{
	if (AreDynamicDataChangesAllowed() && ReflectionViewSampleCountScaleValue != NewValue * UCustomVolumetricCloudComponent::OldToNewReflectionViewRaySampleCount)
	{
		ReflectionViewSampleCountScaleValue = NewValue * UCustomVolumetricCloudComponent::OldToNewReflectionViewRaySampleCount;
		MarkRenderStateDirty();
	}
}
void UCustomVolumetricCloudComponent::SetShadowReflectionSampleCountScale(float NewValue)
{
	if (AreDynamicDataChangesAllowed() && ShadowReflectionViewSampleCountScaleValue != NewValue * UCustomVolumetricCloudComponent::OldToNewReflectionShadowRaySampleCount)
	{
		ShadowReflectionViewSampleCountScaleValue = NewValue * UCustomVolumetricCloudComponent::OldToNewReflectionShadowRaySampleCount;
		MarkRenderStateDirty();
	}
}

/*=============================================================================
	AVolumetricCloud implementation.
=============================================================================*/