// Fill out your copyright notice in the Description page of Project Settings.


#include "CalfActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void ACalfActor::PressBehavior()
{
	Super::PressBehavior();
	isUsing = true;
	mainCharacter->SetCanMove(false);
	mainCharacter->GetInteractionComponent()->SetCanInteract(false);
}

void ACalfActor::ReleaseBehavior()
{
	Super::ReleaseBehavior();
}

void ACalfActor::SetupMember()
{
	if (!memberSkeletalMesh || !mainCharacter)return;
	mainCharacter->GetMesh()->SetSkeletalMesh(memberSkeletalMesh);
	mainCharacter->GetMesh()->SetAnimClass(animClass);
}

void ACalfActor::ResetMember()
{
	isUsing = false;
	Super::ResetMember();
}

