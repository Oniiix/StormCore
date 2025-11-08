// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../../../RobotHunter/CCC/Character/MainCharacter.h"
#include "Components/TextBlock.h"
#include "SlotPlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USlotPlayerUI final : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> btnBan;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> btnKick_1;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextBlock> txtName_1 = nullptr;
	UPROPERTY()
	TObjectPtr<AMainCharacter> player = nullptr;

#pragma region Setter/Getter

public:
	FORCEINLINE void SetPlayer(const TObjectPtr<AMainCharacter>& _player) { player = _player; }
#pragma endregion

	virtual void NativeConstruct() override;
	void Init();
	void SetText(const FText& _text);
	UFUNCTION()
	void KickButtonClicked();
	UFUNCTION()
	void BanButtonClicked();
};
