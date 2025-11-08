// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Scout.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/AI_Torpedo/AI_Torpedo.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"

AAI_Scout::AAI_Scout() : AAI_Base(EAIType::Pennant)
{
}

void AAI_Scout::BeginPlay()
{
	Super::BeginPlay();
	UMovableComponent* _movable = GetComponentByClass<UMovableComponent>();
	if (!_movable)
		return;
	_movable->OnLaunch().AddDynamic(this, &AAI_Scout::DisableJump);

}

void AAI_Scout::DisableJump()
{
	canMarkJump = false;
}
