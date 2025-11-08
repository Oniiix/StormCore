// Fill out your copyright notice in the Description page of Project Settings.


#include "BagActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../../../CCC/Character/MainCharacter.h"

ABagActor::ABagActor()
{
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	AddOwnedComponent(inventoryComponent);
}

void ABagActor::BeginPlay()
{
	Super::BeginPlay();
	LOG_COLOR_TIME(BAG_USE_LOG, "SAC A DOS", MAGENTA, 10);
}

void ABagActor::Interacted(AMainCharacter* _character)
{
	if (isOnPlayer)
		return;
	Super::Interacted(_character);
	GetMesh()->SetSimulatePhysics(false);
	SetActorLocation(_character->GetActorLocation() - (_character->GetMesh()->GetForwardVector() * 40) + (_character->GetActorUpVector() * 50));
	AttachToComponent(_character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	SetActorRotation(_character->GetMesh()->GetRelativeRotation() + FRotator(0, 0, 90));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	isOnPlayer = true;
}

void ABagActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (isOnPlayer)
	{
		canBeDetected = false;
		ACustomActor::NotifyActorBeginOverlap(OtherActor);
		return;
	}

	canBeDetected = true;
	Super::NotifyActorBeginOverlap(OtherActor);
}

void ABagActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (isOnPlayer)
	{
		canBeDetected = false;
		ACustomActor::NotifyActorEndOverlap(OtherActor);
		return;
	}

	canBeDetected = true;
	Super::NotifyActorEndOverlap(OtherActor);
}
