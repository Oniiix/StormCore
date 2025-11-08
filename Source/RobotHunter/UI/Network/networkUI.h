// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SlotPlayerUI.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "NetworkUI.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UNetworkUI final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextBlock> txtName;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UVerticalBox> playerListVerticalBox;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USlotPlayerUI> slotPlayerUIWidget;
	UPROPERTY(meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> btn_Return = nullptr;

	virtual void NativeConstruct() override;
	void Init();
	void RefreshUI();
	void BindButton();
	void CreateSlotPlayerUI(const TObjectPtr<AMainCharacter>& _player);
	UFUNCTION()
	void OnClickedBtn_Return();
};
