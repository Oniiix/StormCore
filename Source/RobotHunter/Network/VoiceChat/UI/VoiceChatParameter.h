// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "VoiceChatParameter.generated.h"

/**
 * 
 */

#define VOICECHAT_PARAMETER_USE_LOG true

class ACustomGameState;

class AMainCharacter;
UCLASS()
class ROBOTHUNTER_API UVoiceChatParameter : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<USlider> VolumeSlider;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<USlider> micThreshold;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UCheckBox> muteCheckBox;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UCheckBox> pushToTalkCheckBox;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> closeButton;
	UPROPERTY()
	ACustomGameState* gameState;
	UPROPERTY()
	AMainCharacter* player;
		
private:
	void NativeConstruct() override;
	void Init();
	UFUNCTION() void ValueVolumeChanged(float _value);
	void Debug(AMainCharacter* _player, float _value) const;
	UFUNCTION() void MicThresholdChanged(float _value);
	UFUNCTION() void MuteCheckBoxChanged(bool _bool);
	UFUNCTION() void PushToTalkCheckBoxChanged(bool _bool);
	UFUNCTION() void CloseButtonClicked();
};
