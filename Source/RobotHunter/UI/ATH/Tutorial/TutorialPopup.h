// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/ATH/TutorialATH.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "TutorialPopup.generated.h"

class UTutorialPopupAsset;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTutorialPopup : public UUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShow);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuit);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow onShow;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow onQuit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> media;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> buttonValidate;
	UPROPERTY()
	const UTutorialPopupAsset* currentAsset;

	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	UTutorialATH* tutorialATH;

public:
	FORCEINLINE TObjectPtr<UButtonPrefab> GetValidateButton() const { return buttonValidate; }

	virtual void Init() override;

	void Setup(const UTutorialPopupAsset* Asset, UTutorialATH* TutorialAth);
	void Stop() const;

private:
	virtual void RegisterEvents() override;
	UFUNCTION()
	FORCEINLINE void OnInputChange(bool _isGamepad);
};
