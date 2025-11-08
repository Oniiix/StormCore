// Fill out your copyright notice in the Description page of Project Settings.


#include "Flower.h"
#include "RobotHunter/Game/CustomGameState.h"

// Sets default values
AFlower::AFlower()
{

	RootComponent = mesh;
	bReplicates = true;

}
void AFlower::BeginPlay()
{
	Super::BeginPlay();
	Init();
}


void AFlower::CooldownPhase()
{
	isActive = false;

}

void AFlower::ActivePhase()
{
	isActive = true;
}

bool AFlower::IsItThisWeather(EMeteoStatus _statue)
{
	if (!HasAuthority())
		return false;

	ACustomGameState* _customGameState = WORLD->GetGameState<ACustomGameState>();

	if (_customGameState && _customGameState->GetCurrentMeteo() == _statue)
		return true;

	return false;
}

