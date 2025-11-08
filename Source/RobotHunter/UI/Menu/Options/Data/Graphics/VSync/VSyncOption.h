// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "VSyncOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, HideDropdown)
class ROBOTHUNTER_API UVSyncOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<bool, FText> values;


public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;
};
