// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/Loot/LootActor.h"
#include "KeyLootActor.generated.h"


class UBoxComponent;


UCLASS()
class ROBOTHUNTER_API AKeyLootActor : public ALootActor
{
	GENERATED_BODY()
private:
	//UPROPERTY(ReplicatedUsing = On_RepAsCharacter) bool asCharacter = false;
	//UPROPERTY(Replicated) TObjectPtr<AMainCharacter> characterOwner = nullptr;
public:
	AKeyLootActor();
//public:
//	void AttachActor(AMainCharacter* _character);
//	void DetachActor();
protected:
	virtual void Interacted(AMainCharacter* _character)override;
	//virtual void ServerRPC_Interacted() override;
	//UFUNCTION() void On_RepAsCharacter();
	virtual void Carry(AMainCharacter* _character) override;
	virtual void Drop(AMainCharacter* _character) override;
};
