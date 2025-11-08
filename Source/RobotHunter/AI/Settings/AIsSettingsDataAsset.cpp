// Fill out your copyright notice in the Description page of Project Settings.

#include "AIsSettingsDataAsset.h"

#if WITH_EDITOR
void UAIsSettingsDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	/// Clamp min runSpeed to walkSpeed
	const bool _moveValueChange = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIsSettingsDataAsset, walkSpeed) || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAIsSettingsDataAsset, runSpeed);
	if (_moveValueChange)
	{
		if (walkSpeed > runSpeed)
			runSpeed = walkSpeed;
	}
	/// Clamp imprecisionFactor

	const bool _imprecisionFactorChange = PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAIsSettingsDataAsset, imprecisionFactor);
	if (_imprecisionFactorChange)
	{
		const bool _imprecisionFactorMemberChange = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(FVector2D, X) || PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(FVector2D, Y);
		if (_imprecisionFactorMemberChange)
		{
			if (imprecisionFactor.X > imprecisionFactor.Y)
				imprecisionFactor.Y = imprecisionFactor.X;
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
