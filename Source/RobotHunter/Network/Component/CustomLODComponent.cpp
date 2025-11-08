// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomLODComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

UCustomLODComponent::UCustomLODComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomLODComponent::BeginPlay()
{
	Super::BeginPlay();

	gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());
	if (!gameState)
	{
		LOG_ERROR(true, "Failed to get gameState of " + OWNER_NAME);
		return;
	}
	arena = Cast<AArena>(GetOwner());
	if (!arena)
	{
		LOG_ERROR(true, "Failed to get arena of " + OWNER_NAME);
		return;
	}

	distance = arena->GetTrackingRange() + distance;
	TickRate(isLag);
	UpdateFrequency(isLag);

	isInitializationGood = true;
}

void UCustomLODComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isInitializationGood)
		return;

	DrawDebug();

	if (!isLag)
		return;

	DistanceCharacterTick();
	//DistanceCharacterNetworkUpdate();
}

void UCustomLODComponent::DrawDebug()
{
	if (!isDebug)
		return;
	DRAW_SPHERE(GetOwner()->GetActorLocation(), distance, FColor::Cyan);
}

void UCustomLODComponent::DistanceCharacterNetworkUpdate()
{
	if (!gameState)
		return;

	const int _playerNum = gameState->GetPlayers().Num();
	if (_playerNum < 2)
		return;
	for (int i = 0; i < _playerNum; i++)
	{
		if (!gameState->GetPlayers()[i]->HasAuthority())
			return;
		const FVector _characterLocation = gameState->GetPlayers()[1]->GetActorLocation();
		const float _distance = FVector::Distance(_characterLocation, GetOwner()->GetActorLocation());
		if (_distance > distance)
			UpdateFrequency(true);
		else
			UpdateFrequency(false);
	}
}

void UCustomLODComponent::DistanceCharacterTick()
{
	if (!gameState)
		return;

	ACharacter* _fpc = GETFPC->GetCharacter();
	if (!_fpc || !_fpc->HasAuthority())
		return;

	const FVector _characterLocation = _fpc->GetActorLocation();
	const float _distance = FVector::Distance(_characterLocation, arena->GetActorLocation());
	//if (_playerNum > 1)
	//{
	//	const FVector _characterLocation2 = gameState->GetPlayers()[1]->GetActorLocation();
	//	const float _distance2 = FVector::Distance(_characterLocation2, arena->GetActorLocation());
	//	if (_distance > distance && _distance2 > distance)
	//		TickRate(true);
	//	else
	//		TickRate(false);
	//	return;
	//}
	if (_distance > distance)
		TickRate(true);
	else
		TickRate(false);
}

void UCustomLODComponent::TickRate(const bool _isLag)
{
	TMap<uint32, AAI_Base*> _mapAIs = arena->GetMapAIs();
	for (TPair<uint32, AAI_Base*>& _element : _mapAIs)
	{
		if (!_element.Value)
			continue;

		if (_isLag)
			_element.Value->SetTickIntervalForActorAndComponents(updateWithTickInterval);
		else
			_element.Value->SetTickIntervalForActorAndComponents(0.f);
	}
}

void UCustomLODComponent::UpdateFrequency(const bool _isLag)
{
	TMap<uint32, AAI_Base*> _mapAIs = arena->GetMapAIs();
	for (TPair<uint32, AAI_Base*>& _element : _mapAIs)
	{
		if (!_element.Value)
			continue;

		if (_isLag)
			_element.Value->NetUpdateFrequency = netUpdateFrequencyMin;
		else
			_element.Value->NetUpdateFrequency = netUpdateFrequencyMax;
	}
}
