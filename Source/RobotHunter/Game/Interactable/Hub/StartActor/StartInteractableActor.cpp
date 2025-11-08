// Fill out your copyright notice in the Description page of Project Settings.


#include "StartInteractableActor.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../../../../UI/HUD/GameHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <RobotHunter/SaveSystem/GameplaySave.h>
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"

void AStartInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	onBlendEnd.AddDynamic(this, &AStartInteractableActor::ShowMenu);
}

void AStartInteractableActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AStartInteractableActor::Interacted(AMainCharacter* _character)
{
	if (!HasAuthority()) return;
	Super::Interacted(_character);
}

void AStartInteractableActor::Interacted_Internal()
{
	Super::Interacted_Internal();
}

void AStartInteractableActor::ShowMenu()
{
	AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	_hud->SetState(EStateUI::HubStart);
}
