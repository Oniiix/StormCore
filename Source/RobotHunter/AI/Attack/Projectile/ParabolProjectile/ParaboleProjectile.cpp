// Fill out your copyright notice in the Description page of Project Settings.


#include "ParaboleProjectile.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/Component/NetworkAIComponent.h"

#pragma region Setteur
void AParaboleProjectile::SetMiddleCurve(const FVector& _goalLocation, const FVector& _baseLocation)
{
	const FVector& _middle = (_goalLocation + _baseLocation) / 2;
	const float _topCurve = paraboleStat->GetTopCurve();

	switch (paraboleStat->GetCurveType())
	{
	case VERTICAL:
		middleCurve = FVector(_middle.X, _middle.Y, _middle.Z + _topCurve);
		break;
	case HORIZONTAL:
		middleCurve = _middle + GetActorRightVector() * _topCurve;
		break;
	default:
		break;
	}
}
#pragma endregion

void AParaboleProjectile::BeginPlay()
{
	Super::BeginPlay();
	canMove = false;
}

void AParaboleProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (!canMove)
		return;
	MoveToParabole();
}

#pragma region Move
void AParaboleProjectile::MoveToParabole()
{
	if (index < allPoints.Num())
	{
		const FVector& _newLocation = FMath::VInterpConstantTo(GetActorLocation(), allPoints[index], GetWorld()->DeltaTimeSeconds, speed);
		SetActorLocationProjectile(_newLocation);
		if (FVector::DistXY(_newLocation, allPoints[index]) <= 10)
			index++;
	}
	else
		EndMove();
}

void AParaboleProjectile::EndMove()
{
	const FVector& _actorLocation = GetActorLocation();
	const FVector& _newLocation = FMath::Lerp(_actorLocation, _actorLocation - GetActorUpVector(), GetWorld()->DeltaTimeSeconds * speed);
	SetActorLocationProjectile(_newLocation);
}
#pragma endregion

#pragma region Curve
TArray<FVector> AParaboleProjectile::CreateCurve(const FVector& _goalPosition)
{
	TArray<FVector> _pointsProjectile;
	float _t;
	float _definitionCurve = paraboleStat->GetDefinitionCurve();
	for (float i = 0; i <= _definitionCurve; i++)
	{
		_t = i / _definitionCurve;
		_pointsProjectile.Add(GetMovementCurve(startLocation, middleCurve, _goalPosition, _t));
	}
	return _pointsProjectile;
}

FVector AParaboleProjectile::GetMovementCurve(const FVector& _pos1, const FVector& _pos2, const FVector& _pos3, const float _time) const
{
	const FVector& _lerp1 = FMath::Lerp(_pos1, _pos2, _time);
	const FVector& _lerp2 = FMath::Lerp(_pos2, _pos3, _time);

	return FMath::Lerp(_lerp1, _lerp2, _time);
}
#pragma endregion

void AParaboleProjectile::InitProjectile(const FVector& _targetLocation)
{
	Super::InitProjectile(_targetLocation);
	paraboleStat = Cast< UPrabolProjectileStat>(stats);
	SetMiddleCurve(targetLocation, startLocation);

	TArray<FVector> _curve = CreateCurve(targetLocation);
	UNetworkAIComponent* _net = GetWorld()->GetFirstPlayerController()->GetComponentByClass< UNetworkAIComponent>();

	if (HasAuthority())
		SetAllPoints(_curve);
	else
		_net->ServerRPC_SetAllPoints(this, _curve);

	canMove = true;
}

void AParaboleProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParaboleProjectile, allPoints);
}

