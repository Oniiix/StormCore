// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "SoundMasterOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API USoundMasterOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Sound)
	int minValue = 0;
	UPROPERTY(EditAnywhere, Category = Sound)
	int maxValue = 100;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<USoundClass> soundClass;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;

	virtual void Apply() override;

	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

private:
	void SetSoundGlobal() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
