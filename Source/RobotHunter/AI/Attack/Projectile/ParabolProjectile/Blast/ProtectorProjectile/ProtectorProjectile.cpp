// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtectorProjectile.h"

void AProtectorProjectile::InitProjectile(const FVector& _targetLocation)
{
	Super::InitProjectile(_targetLocation);
	canMove = false;
}

void AProtectorProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (currentTime < timeChargeProjectile)
		GrowProjectile();
}

void AProtectorProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	SpawnTriggerSphere();
	Destroy();
}

void AProtectorProjectile::SpawnTriggerSphere()
{
	ATriggerProjectileSphere* _triggerSphere = GetWorld()->SpawnActor<ATriggerProjectileSphere>(triggerSphere, GetActorLocation(), GetActorRotation());
	_triggerSphere->SetDamage(damage);
	_triggerSphere->SetLifeSpan(0.1);
	_triggerSphere->SetActorScale3D(_triggerSphere->GetActorScale3D() * radiusBlast);
}

void AProtectorProjectile::GrowProjectile()
{
	FVector _newScale = FMath::Lerp(GetActorScale(), GetActorScale() + FVector(1,1,1), GetWorld()->DeltaTimeSeconds);
	SetActorScale3D(_newScale);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= timeChargeProjectile)
		canMove = true;
}

