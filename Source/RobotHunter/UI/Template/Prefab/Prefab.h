// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "RobotHunter/UI/UI.h"
#include "Prefab.generated.h"

#define DEBUG_PREFAB false
/**
 * 
 */
UCLASS(HideDropdown, Abstract)
class ROBOTHUNTER_API UPrefab : public UUI
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	bool bOverrideStyle = false;

	template <typename T>
	void CheckPropertyOverrideStyle(const FPropertyChangedEvent& property, const FString& propertyName,
	                                const T& settingsOverride, const bool& checkActivate,
	                                const std::function<void()>& callback_deny,
	                                const std::function<void()>& callback_override) const
	{
		FName PropertyName = property.Property ? property.Property->GetFName() : NAME_None;

		if (PropertyName.ToString().ToLower() == propertyName.ToLower())
		{
			if (!checkActivate)
			{
				callback_deny();
			}
			else if (settingsOverride)
			{
				callback_override();
			}
		}
	}
};
