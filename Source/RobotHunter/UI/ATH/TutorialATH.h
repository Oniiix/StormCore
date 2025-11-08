// 

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Tuto/Condition/ConditionDataAsset.h"
#include "TutorialATH.generated.h"

class UTutorialPopupAsset;
class UTutorialHintAsset;
class UTutoHint;
class UTutorialPopup;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTutorialATH : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> verticalBox;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UTutoHint> tutorialHintReference;

	UPROPERTY(VisibleAnywhere, Category = "Custom")
	TObjectPtr<UTutoHint> tutorialHint;
	UPROPERTY()
	TObjectPtr<UConditionDataAsset> currentInputCondition;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTutorialPopup> tutorialPopup;

public:
	virtual void Init() override;
	virtual void RegisterEvents() override;

	void AddTutorial(UTutorialHintAsset* TutorialAsset, const TObjectPtr<UConditionDataAsset>& _inputCondition);
	void ShowBigTutorial(const UTutorialPopupAsset* TutorialAsset);

private:
	UFUNCTION()
	void UpdateProgress(float Progress);
	void ResetTutorial();
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CloseTutorialPopup();
};
