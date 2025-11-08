// Fill out your copyright notice in the Description page of Project Settings.


#include "CarcassActor.h"
#include <RobotHunter/CCC/Character/Components/Inventory/InventoryComponent.h>
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include <RobotHunter/Network/OnlineManagerSubsystem.h>
#include <RobotHunter/Network/Component/NetworkGPEComponent.h>

void ACarcassActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
	UInventoryComponent* _inventory = _character->GetInventoryComponent();

	if (!_inventory)
		return;
	numberLoot = FMath::RandRange(minDrop, maxDrop);
	_inventory->AddItemToInventory(dropName, numberLoot);
	if (HasAuthority())
		SetLifeSpan(0.1);
	else
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_DestroyCustomActor(this);
}

