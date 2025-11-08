// Fill out your copyright notice in the Description page of Project Settings.

#include "Egg.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"
#include "RobotHunter/Utils/DebugUtils.h"

AEgg::AEgg()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	hasBeenCollect = false;
	isLoaded = false;
}

void AEgg::BeginPlay()
{
	Super::BeginPlay();
	//SetVisibility(visibleOnStart);
}

void AEgg::Hatch()
{
	if (HasAuthority())
	{
		//GetWorldTimerManager().ClearTimer(hatchTimer);
		onHatch.Broadcast();
		SetLifeSpan(0.1f);
	}
}

void AEgg::SetVisibility(const bool _visibility)
{
	isLoaded = _visibility;
	RootComponent->SetVisibility(isLoaded, true);
}

void AEgg::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
}

void AEgg::Carry(AMainCharacter* _character)
{
	Super::Carry(_character);
	AttachActor();
}

void AEgg::Drop(AMainCharacter* _character)
{
	Super::Drop(_character);
	DetachActor();
}

void AEgg::LaunchAnimation(AMainCharacter* _character)
{
	//_character->GetCharaAnim()->SetAnimWeight(1);
	_character->GetCharaAnim()->SetHasCarry(true);
	_character->GetInteractionComponent()->SetHasInteracted(true);
	LOG_COLOR(true, "Interacted Egg", GREEN);
}

void AEgg::AttachActor()
{
	AMainCharacter* _mainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!_mainCharacter)
		return;
	UInventoryComponent* _inventory = _mainCharacter->GetInventoryComponent();
	if (!_inventory)
		return;
	if (!_inventory->GetHasEgg())
	{
		LOG_COLOR(EGG_USE_LOG, "Get", PINK);
		_inventory->AddEgg(this);
		onAttachPlayer.Broadcast(this);
	}
}

void AEgg::DetachActor()
{
	AMainCharacter* _mainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!_mainCharacter)
		return;
	UInventoryComponent* _inventory = _mainCharacter->GetInventoryComponent();
	if (!_inventory)
		return;
		LOG_COLOR(EGG_USE_LOG, "Throw", PINK);
		_inventory->DropEgg();
		
}

