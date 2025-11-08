#include "ZoneFlower.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../Utils/DebugUtils.h"
#include "Math/UnrealMathUtility.h"

#define DEBUG_ZONE_FLOWER false




void AZoneFlower::Init()
{
	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}
	if (activeDamageDuringTime)
		CooldownPhase();

	if (!areaEffect)
	{
		LOG_ERROR(DEBUG_ZONE_FLOWER, "AZoneFlower::not areaEffect");
		return;
	}
	areaEffect->OnComponentBeginOverlap.AddDynamic(this, &AZoneFlower::OnBoxBeginOverlap);
	areaEffect->OnComponentEndOverlap.AddDynamic(this, &AZoneFlower::OnBoxEndOverlap);
}


void AZoneFlower::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	actorWithEffect.Add(OtherActor);
	if (!isActive)
		return;
	LOG_WARNING(DEBUG_ZONE_FLOWER, "AZoneFlower::OnBoxBeginOverlap");
	InitZoneTimer(actorWithEffect);

	
}

void AZoneFlower::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	actorWithEffect.Remove(OtherActor);

	ExitZoneEffect(OtherActor);
	InitZoneTimer(actorWithEffect);

	LOG_WARNING(DEBUG_ZONE_FLOWER, "AZoneFlower::OnBoxEndOverlap");
	if (actorWithEffect.IsEmpty())
		ClearTimer();

	
}

void AZoneFlower::ActivePhase()
{
	Super::ActivePhase();
	InitZoneTimer(actorWithEffect);
	GetWorld()->GetTimerManager().ClearTimer(timerFlowerPhase);
	FTimerHandle _TimerHandle;
	if (isRandomCooldown)
	{
		int _randomEffect = FMath::RandRange(effectMin, effectMax);
		GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AZoneFlower::CooldownPhase, _randomEffect, false);
	}
	else
		GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AZoneFlower::CooldownPhase, effectDuration, false);
}

void AZoneFlower::CooldownPhase()
{
	Super::CooldownPhase();
	ClearTimer();
	FTimerHandle _TimerHandle;
	if (isRandomCooldown)
	{
		int _randomCooldown = FMath::RandRange(cooldownMin, cooldownMax);
		GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AZoneFlower::ActivePhase, _randomCooldown, false);
	}
	else
		GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AZoneFlower::ActivePhase, cooldown, false);
}

void AZoneFlower::InitZoneTimer(TArray<AActor*> _actorWithEffect)
{
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AZoneFlower::ZoneEffect, _actorWithEffect);
	
	GetWorld()->GetTimerManager().SetTimer(zoneTimer, TimerDelegate, hitRate, true,0.1);
}



void AZoneFlower::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(zoneTimer);
}


//void AZoneFlower::MultiRpc_Cooldown_Implementation()
//{
//	
//}
