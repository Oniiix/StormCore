// Fill out your copyright notice in the Description page of Project Settings.


#include "RockProjectile.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void ARockProjectile::InitProjectile(const FVector& _targetLocation)
{
	Super::InitProjectile(_targetLocation);
	SetActorScale3D(FVector::One() * initSize);
	speedSize = (maxSize - initSize) / timeSize;

	maxScale = FVector::One() * maxSize;
}

void ARockProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (currentTime <= timeSize)
		SizeProjectile();
}

void ARockProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMainCharacter* _chara = Cast<AMainCharacter>(OtherActor);
	if (!_chara)
		return;
	_chara->GetLifeComponent()->Damage(GetDamage(), this);
	Destroy();
}

void ARockProjectile::SizeProjectile()
{
	initScale = GetActorScale3D();

	newScale = FMath::Lerp(initScale, initScale+ FVector(1,1,1), GetWorld()->DeltaTimeSeconds* speedSize);
	SetActorScale3D(newScale);
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	FVector _newLocation = GetActorLocation() + FVector(0, 0, ((newScale.X / 2) - (initScale.X / 2) )* 100);
	SetActorLocationProjectile(_newLocation);
}

float ARockProjectile::GetDamage()
{
	return initDamage + (((maxDamage - initDamage) / (maxSize - initSize)) * (newScale.X - initSize));
}
