// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "ResolutionOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, HideDropdown)
class ROBOTHUNTER_API UResolutionOption final : public UOptionData
{
	GENERATED_BODY()

	/*Set the different resolution supported
	 *Default resolution: 1080p, 720p, 480p, 360p, 240p
	 */
	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<FIntPoint> Resolutions;

	UPROPERTY(EditAnywhere, Category = "Custom")
	int defaultIndexValue = 0;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

private:
	int GetIndexOfResolution(const FIntPoint& resolution);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
