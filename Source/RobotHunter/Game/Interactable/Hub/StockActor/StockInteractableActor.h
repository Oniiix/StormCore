// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../HUBInteractableActor.h"
#include "StockInteractableActor.generated.h"

class UNewStock;
class ACustomGameState;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AStockInteractableActor : public AHUBInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	FString currentCharacter;
	
	UPROPERTY(Replicated)
	bool isInteracting = false;
	TObjectPtr<ACustomGameState> game;

	UPROPERTY()
	TObjectPtr<UNewStock> stockUI;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<AMainCharacter> characterPtr;

public:
	FORCEINLINE void SetCurrentCharacter(const FString& _characterName) { currentCharacter = _characterName; }

public:
	AStockInteractableActor();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Interacted(AMainCharacter* _character) override;
	virtual void LaunchAnimation(AMainCharacter* _character) override;


	void SetCharacterVisibility(const AMainCharacter* Character, bool bVisible);
	UFUNCTION(NetMulticast, Unreliable)
	void NetMuticastRPC_SetCharacterVisibility(AMainCharacter* Character, bool bVisible);
	void SetOtherCharacterVisibility_Client(const AMainCharacter* _character);
	UFUNCTION()
	void VisibleChangeMesh();


protected:
	virtual void Interacted_Internal() override;

private:
	UFUNCTION()
	virtual void ShowMenu() override;
	UFUNCTION()
	void ExitMenu();
	UFUNCTION()
	void NetworkExitMenu();
	UFUNCTION()
	void DebugMenu();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
