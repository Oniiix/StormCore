// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "TutoHint.generated.h"

class UTutorialHintAsset;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTutoHint : public UUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShow);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHide);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow onShow;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHide onHide;

	FORCEINLINE FOnShow& OnShow() { return onShow; }
	FORCEINLINE FOnHide& OnHide() { return onHide; }

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textTutorial;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UProgressBar> progressTutorial;

public:
	FORCEINLINE void Callback_OnShow() const { onShow.Broadcast(); }
	FORCEINLINE void Callback_OnHide() const { onHide.Broadcast(); }

	void Setup(const UTutorialHintAsset* _asset) const;
	void SetValue(float Progress) const;
};
