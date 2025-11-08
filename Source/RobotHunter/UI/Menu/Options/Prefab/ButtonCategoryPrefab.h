// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/Utils/ENUM_Option.h"
#include "ButtonCategoryPrefab.generated.h"

class UOptionFrame;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UButtonCategoryPrefab final : public UButtonPrefab
{
	GENERATED_BODY()

	UPROPERTY()
	ECategoryOption categoryEnum;

	UPROPERTY()
	TObjectPtr<UOptionFrame> parent;

public:
	FORCEINLINE TObjectPtr<UButton> GetButton() const { return buttonBase; }
	FORCEINLINE ECategoryOption GetIndex() const { return categoryEnum; }

	void Setup(const TObjectPtr<UOptionFrame>& _parent, ECategoryOption _category);

private:
	UFUNCTION()
	void ShowCategory();

	virtual void RegisterEvents() override;
};
