// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Overlay.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/UI.h"

#include "EndDay.generated.h"


class UGameSectionSettings;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UEndDay : public UUI
{
	GENERATED_BODY()

	// ========================================
	// DELEGATES DECLARATIONS
	// ========================================
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathPlayer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathByKillZone);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDayEnd);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDayNext);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEnd);

	// ========================================
	// DELEGATE INSTANCES
	// ========================================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnDeathPlayer onDeathPlayer;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnDeathByKillZone onDeathByKillZone;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnDayEnd onDayEnd;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnDayNext onDayNext;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnAnimationEnd onAnimationEnd;

	// ========================================
	// REFERENCES & OBJECTS
	// ========================================
	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	AMainCharacter* Character;

	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	TObjectPtr<UGameSectionSettings> settings;

	// ========================================
	// UI WIDGETS
	// ========================================
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UOverlay> deathPlayerOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UOverlay> nextDayOverlay;

	// ========================================
	// TEXT PROPERTIES
	// ========================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Text", meta = (AllowPrivateAccess))
	FText deathPlayerMessage = TEXTSTR("Death");

	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	FText dayEndMessage = TEXTSTR("{0}:00");

	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	FText dayNextMessage = TEXTSTR("Day {0}");

	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	FString customFormatText = "<normal>{0}</>";

	// ========================================
	// GAME STATE
	// ========================================
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	int currentDay;

	// ========================================
	// INLINE FUNCTIONS
	// ========================================	
	FORCEINLINE FOnDeathPlayer& OnDeathPlayer() { return onDeathPlayer; }

public:
	virtual void InitWithDelay() override;

	UFUNCTION()
	FORCEINLINE void Callback_OnDeathPlayer() { onDeathPlayer.Broadcast(); }

	UFUNCTION()
	FORCEINLINE void Callback_OnDayNext() { onDayNext.Broadcast(); }

	UFUNCTION()
	FORCEINLINE void Callback_OnDayEnd() { onDayEnd.Broadcast(); }

	UFUNCTION()
	FORCEINLINE void Callback_OnDeathByKillZone() { onDeathByKillZone.Broadcast(); }


	UFUNCTION(BlueprintCallable, Category = "Custom")
	FORCEINLINE FString GetDayNextMessage() const { return FORMAT(dayNextMessage.ToString(), INTSTR(currentDay + 1)); }

private:
	UFUNCTION(BlueprintCallable, Category = "Custom")
	FORCEINLINE FString GetFormatText(const FString FormatText) const
	{
		return FORMAT(customFormatText, FormatText);
	}

	UFUNCTION()
	void LoadMap();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void BackToHub() const;

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void DisableInputCharacter();

	UFUNCTION(BlueprintCallable, Category = "Custom")
	bool IsLastDay() const;

	UFUNCTION()
	void BackToMain();
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void DeleteSave() const;

	virtual void RegisterEvents() override;
};
