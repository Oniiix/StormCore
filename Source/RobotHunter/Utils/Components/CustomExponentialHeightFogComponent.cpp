// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomExponentialHeightFogComponent.h"

void UCustomExponentialHeightFogComponent::SetVolumetricFogStartDistance(const float _newValue)
{
	if (VolumetricFogStartDistance != _newValue)
	{
		VolumetricFogStartDistance = _newValue;
		MarkRenderStateDirty();
	}
}

void UCustomExponentialHeightFogComponent::SetVolumetricFogNearFadeInDistance(const float _newValue)
{
	if (VolumetricFogNearFadeInDistance != _newValue)
	{
		VolumetricFogNearFadeInDistance = _newValue;
		MarkRenderStateDirty();
	}
}
