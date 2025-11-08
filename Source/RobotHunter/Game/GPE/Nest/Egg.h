// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotHunter/Game/Interactable/Loot/LootActor.h"
#include "Egg.generated.h"

#define EGG_USE_LOG false

class ANest;
/// <summary>
/// The GPE interactable of the Protector's egg.
/// Can be lootable by player and hatch a Protector.
/// </summary>
UCLASS()
class ROBOTHUNTER_API AEgg : public ALootActor
{
	GENERATED_BODY()

#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHatch);
	FOnHatch onHatch;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttachPlayer, AEgg*, _egg);
	FOnAttachPlayer onAttachPlayer;
#pragma endregion

#pragma region Info
	UPROPERTY(EditAnywhere, Category = "Informations")
	float price = 5;
	UPROPERTY(EditAnywhere, Category = "Informations")
	float weight = 10;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Property")
	bool visibleOnStart = false;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool canDrop = false;
	UPROPERTY(EditAnywhere, Category = "Property")
	bool canHatch = false;
	UPROPERTY(EditAnywhere, Category = "Property", meta = (EditConditionHide = "canHatch"))
	int hatchDelay = 5;
	UPROPERTY(EditAnywhere, Category = "Property")
	int hitCount = 5;
	UPROPERTY(EditAnywhere, Category = "Property")
	int minFallThreshol = 100;

	FTimerHandle hatchTimer;
	bool hasBeenCollect = false;
	bool isLoaded = false;
#pragma endregion

public:
	AEgg();

#pragma region Methods

#pragma region Getter
	FORCEINLINE FOnHatch& OnHatch() { return onHatch; }
	FORCEINLINE FOnAttachPlayer& OnAttachPlayer() { return onAttachPlayer; }
	FORCEINLINE bool GetCanDrop() const { return canDrop; }
	FORCEINLINE bool GetCanHatch() const { return canHatch; }
	FORCEINLINE float GetPrice()const { return price; }
	FORCEINLINE float GetWeight() { return weight; }
	FORCEINLINE void SetPrice(float _price) { price = _price; }
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetCanDrop(const bool _canDrop) { canDrop = _canDrop; }
	FORCEINLINE void SetCanHatch(const bool _canHatch) { canHatch = _canHatch; }
#pragma endregion

	/// <summary>
	/// Hatch the egg (destroy) to spawn a Protector
	/// </summary>
	void Hatch();
	/// <summary>
	/// Change the visibility of the egg
	/// </summary>
	/// <param name="_visibility"> visible or not </param>
	void SetVisibility(const bool _visibility);

	void AttachActor();
	void DetachActor();
protected:
	virtual void BeginPlay() override;
	virtual void Interacted(AMainCharacter* _character)override;

	virtual void Carry(AMainCharacter* _character) override;
	virtual void Drop(AMainCharacter* _character) override;

	virtual void LaunchAnimation(AMainCharacter* _character) override;
#pragma endregion
};
