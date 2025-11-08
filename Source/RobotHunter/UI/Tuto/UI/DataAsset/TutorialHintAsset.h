// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "TutorialHintAsset.generated.h"

class UTutorialPopupAsset;
class UConditionDataAsset;
/**
 * 
 */
UCLASS(BlueprintType)
class ROBOTHUNTER_API UTutorialHintAsset : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValidateProgress, float, progress);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnValidateProgress onValidateProgress;

	UPROPERTY(EditAnywhere, Category = "Custom")
	bool isBigPopup = false;

	UPROPERTY(EditAnywhere, Category = "Custom|Text", meta = (EditCondition="!isBigPopup", EditConditionHides))
	FText tutorial = TEXTSTR("Insert the tutorial here");

	UPROPERTY(EditAnywhere, Category = "Custom|Text", meta = (EditCondition="!isBigPopup", EditConditionHides))
	FString tutorialFormat = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom|Input", meta = (EditCondition="!isBigPopup", EditConditionHides))
	TObjectPtr<UConditionDataAsset> InputCondition = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|BigPopup", meta = (EditCondition="isBigPopup", EditConditionHides))
	TObjectPtr<UTutorialPopupAsset> bigTutorial = nullptr;

public:
	FORCEINLINE FOnValidateProgress& OnValidateProgress() { return onValidateProgress; }
	FORCEINLINE FString GetTextFormated() const { return FORMAT(tutorialFormat, tutorial.ToString()); }

	UFUNCTION()
	void ValidationProgress(float _value);

	UFUNCTION(BlueprintCallable) void LaunchTutorial(class AMainCharacter* _chara);
};
