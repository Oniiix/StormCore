// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/Settings/PrefabSettings.h"
#include "InputFieldPrefabOverrideStyle.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UInputFieldPrefabOverrideStyle final : public UPrefabSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FEditableTextBoxStyle EditableTextBoxStyle;

public:
	FORCEINLINE FEditableTextBoxStyle GetStyle() const { return EditableTextBoxStyle; }
};
