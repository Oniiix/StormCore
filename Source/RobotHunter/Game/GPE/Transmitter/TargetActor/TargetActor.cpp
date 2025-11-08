// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

ATargetActor::ATargetActor()
{
	lifeComponent = CreateDefaultSubobject<ULifeComponent>("Life Component");
	AddOwnedComponent(lifeComponent);
}

void ATargetActor::BeginPlay()
{
	Super::BeginPlay();
	lifeComponent->OnLifeChangeByDamage().AddUniqueDynamic(this, &ATargetActor::ActivateTarget);
}

void ATargetActor::ActivateTarget(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	lifeComponent->RestoreLife(-_damage);
	if (hasShoot)
		return;

	Activate();
	hasShoot = true;

	FTimerHandle _cooldownTimer;
	FTimerDelegate _cooldownDelegate;
	_cooldownDelegate.BindLambda([&]() { hasShoot = false; });
	WORLD->GetTimerManager().SetTimer(_cooldownTimer, _cooldownDelegate, cooldownTime, false);
}
