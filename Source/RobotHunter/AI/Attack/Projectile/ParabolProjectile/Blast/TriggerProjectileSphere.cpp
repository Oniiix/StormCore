// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerProjectileSphere.h"

void ATriggerProjectileSphere::InitTrigger(const TArray< TEnumAsByte<ECollisionChannel>>& _collision,const float _radius, const FHitResult& _result)
{
	SetActorScale3D(GetActorScale3D() * _radius);
	const int _num = _collision.Num();
	for (int i = 0; i < _num; i++)
		GetCollisionComponent()->SetCollisionResponseToChannel(_collision[i], ECollisionResponse::ECR_Overlap);
	if (typeBlast == ETypeBlast::DURING)
		SetLifeSpan(lifeSpanTrigger);
	hitResult = _result;
	onInit.Broadcast(_radius);
}

void ATriggerProjectileSphere::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(!player)
	{
		player = Cast<AMainCharacter>(OtherActor);
		if (!player)
			return;
		player->GetLifeComponent()->OnDie().AddDynamic(this, &ATriggerProjectileSphere::PlayerDead);
	}
	
	GetWorldTimerManager().SetTimer(timerInit, this, &ATriggerProjectileSphere::TypeDamage, 0.1f);
}

void ATriggerProjectileSphere::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!Cast<AMainCharacter>(OtherActor) || !player)
		return;
	player->GetLifeComponent()->OnDie().RemoveDynamic(this, &ATriggerProjectileSphere::PlayerDead);
	player = nullptr;
	GetWorldTimerManager().ClearTimer(timerDamage);
}

void ATriggerProjectileSphere::TypeDamage()
{
	GetWorldTimerManager().ClearTimer(timerInit);
	switch (typeBlast)
	{
	case ETypeBlast::INSTANT:
		Damage();
		Destroy();
		break;
	case ETypeBlast::DURING:
		GetWorldTimerManager().SetTimer(timerDamage, this, &ATriggerProjectileSphere::Damage, timeDamage, true, 0.1f);
		break;
	default:
		break;
	}
}

void ATriggerProjectileSphere::Damage()
{
	if (!player)
		return;
	player->GetLifeComponent()->Damage(damage, this);
}

void ATriggerProjectileSphere::PlayerDead()
{
	GetWorldTimerManager().ClearTimer(timerDamage);
	player = nullptr;
}

void ATriggerProjectileSphere::ClearArena(AAlienDoor* _exitGate, bool _giveBonus)
{
	PlayerDead();
	Destroy();
}
