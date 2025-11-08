// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "AlternativeShotgunCrosshair.generated.h"

#define INIT_LIST { \
		{ \
				{true, TEXTSTR("First Version")}, \
				{false, TEXTSTR("Current Version")} \
		} \
	}
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UAlternativeShotgunCrosshair final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Alternative shotgun crosshair",
		meta = (EditCondition = "OptionType == EOptionType::Checker", EditConditionHides))
	TMap<bool, FText> values = INIT_LIST;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
