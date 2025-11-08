// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerangProjectil.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void ABoomerangProjectil::InitProjectile(const FVector& _targetLocation)
{
	Super::InitProjectile(_targetLocation);
	middle = (targetLocation + startLocation) / 2;
}

#pragma region Return Curve
void ABoomerangProjectil::EndMove()
{
	if (!returnMode)
		SetReturnPoint();
	else
		Destroy();
}

void ABoomerangProjectil::SetReturnPoint()
{
	FVector _returnPosition;
	TArray<FVector> _allReturnPoint;
	returnMode = true;
	index = 0;
	for (int i = 0; i < allPoints.Num(); i++)
	{
		_returnPosition = FVector((2 * middle.X) - allPoints[i].X, (2 * middle.Y) - allPoints[i].Y, allPoints[allPoints.Num() - 1 - i].Z);
		_allReturnPoint.Add(_returnPosition);
	}
	allPoints.Empty();
	allPoints = _allReturnPoint;
}
#pragma endregion

void ABoomerangProjectil::NotifyActorBeginOverlap(AActor* _actor)
{
	AMainCharacter* _player = Cast<AMainCharacter>(_actor);
	if (_player)
		_player->GetLifeComponent()->Damage(stats->GetDamage(), this);
}
