// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveProjectile.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void ACurveProjectile::BeginPlay()
{
	canMove = false;
	curveProjectileStat = Cast<UCurveProjectileStat>(stats);
	//if (curveProjectileStat->GetUseTrigger())
	//{
	//	blastComponent = NewObject<UBlastComponent>(this, UBlastComponent::StaticClass(), FName("BlastComponent"));
	//	blastComponent->Init(curveProjectileStat);
	//}
	Super::BeginPlay();
	SetLifeSpan(stats->GetLifeSpan());

}

void ACurveProjectile::InitProjectile(const FVector& _targetLocation)
{
	Super::InitProjectile(_targetLocation);
	canMove = true;

	curveProjectile = curveProjectileStat->GetProjectileCurve();
	speedCurve = curveProjectileStat->GetSpeedCurve();

	InitSpeed();

	direction = targetLocation - startLocation;

	float _dist = FVector::Distance(targetLocation, startLocation);
	totalTime = _dist / mynSpeed;

	GetWorldTimerManager().SetTimer(testTimer, this, &ACurveProjectile::StopMove, totalTime);
}

void ACurveProjectile::InitSpeed()
{
	float _mynSpeed = 0;
	const float& _def = curveProjectileStat->GetDefinitionCurve();
	for (int i = 0; i < _def + 1; i++)
		_mynSpeed += speedCurve->GetFloatValue(i / _def) * speed;

	mynSpeed = _mynSpeed / _def;
}

void ACurveProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (!canMove)
		return;
	Move();
}

void ACurveProjectile::Move()
{
	if (!isEndMove)
		MoveCurve();
	else
		EndMove();
}

void ACurveProjectile::MoveCurve()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	UpdateSpeed();

	const FVector& _actorPosition = GetActorLocation();

	const FVector& _newLocation = FMath::Lerp(_actorPosition, _actorPosition + direction.GetSafeNormal(), GetWorld()->DeltaTimeSeconds * currentSpeed);

	SetActorLocationProjectile(_newLocation);

	UpdatePositionCurve(_newLocation);
}

void ACurveProjectile::EndMove()
{
	const FVector& _actorPosition = GetActorLocation();

	const FVector& _newLocation = FMath::Lerp(_actorPosition, _actorPosition + direction.GetSafeNormal(), GetWorld()->DeltaTimeSeconds * currentSpeed);

	SetActorLocationProjectile(_newLocation);
}

void ACurveProjectile::UpdatePositionCurve(const FVector& _newLocation)
{
	const float _ratioTime = currentTime / totalTime;
	const float ratioUp = (targetLocation.Z - startLocation.Z) * _ratioTime;
	const FVector& _curvePosition = startLocation + curveProjectile->GetVectorValue(currentTime / totalTime) * curveProjectileStat->GetTopCurve();
	const FVector& _curvePositionActor = FVector(_newLocation.X, _newLocation.Y, ratioUp + _curvePosition.Z);

	SetActorLocationProjectile(_curvePositionActor);

	if (currentTime / totalTime >= 0.9f && lastPosition == FVector::ZeroVector)
		lastPosition = _curvePositionActor;
}

void ACurveProjectile::UpdateSpeed()
{
	currentSpeed = speedCurve->GetFloatValue(currentTime / totalTime) * speed;
}

void ACurveProjectile::StopMove()
{
	direction = GetActorLocation() - lastPosition;
	isEndMove = true;
}
