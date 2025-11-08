#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/Hub/RadioActor/RadioActor.h"
#include "RobotHunter/UI/FloatingUI/FloatingUI.h"
#include "RadioUI.generated.h"

class URadioSettings;
class AWeather;

UENUM(BlueprintType)
enum ERadioTranscriptStatus { First, Second, Transcript, End };

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API URadioUI final : public UFloatingUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShow, AWeather*, weather, int, day);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTranscription);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Custom|Event", meta = (AllowPrivateAccess))
	FOnShow onShow;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Custom|Event", meta = (AllowPrivateAccess))
	FOnEndTranscription onEndTranscription;

	bool isInitialized = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	TObjectPtr<AWeather> weather;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	TObjectPtr<ARadioActor> RadioGPE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	int currentDay;

public:
	FORCEINLINE bool IsInitialized() const { return isInitialized; }

	void CallBackOnShow(const TSubclassOf<AWeather>& Weather, int Day, const TObjectPtr<ARadioActor>& _actor);
	void Hide();
};
