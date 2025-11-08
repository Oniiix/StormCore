#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "DaltonismForceOption.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UDaltonismForceOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<int> forceDaltonism = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;
};
