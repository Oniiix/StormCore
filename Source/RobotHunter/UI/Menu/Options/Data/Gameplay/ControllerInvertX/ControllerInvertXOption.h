// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "ControllerInvertXOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UControllerInvertXOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "InvertX",
		meta = (EditCondition = "OptionType == EOptionType::Checker", EditConditionHides))
	TMap<bool, FText> values = {
		{
			{true, TEXTSTR("Enabled")},
			{false, TEXTSTR("Disabled")}
		}
	};

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void SetInvertXToPlayer() const;
};
