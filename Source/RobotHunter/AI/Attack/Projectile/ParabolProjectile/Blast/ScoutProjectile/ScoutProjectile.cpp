// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoutProjectile.h"

void AScoutProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	const AMainCharacter* _chara = Cast<AMainCharacter>(OtherActor);
	if (_chara)
		InitBlast(_chara);
	else
		BlastProjectile(GetActorLocation());
	Destroy();
}

void AScoutProjectile::InitBlast(const AMainCharacter* _chara)
{
	const FVector& _origin = GetActorLocation();
	ULifeComponent* _lifeComponent = _chara->GetLifeComponent();
	if(_lifeComponent)
		_lifeComponent->Damage(damage, this);

	FHitResult _result;
	const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _origin, _origin - FVector(0, 0, 500), outLayer, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, _result, false);
	if (!_hit)
		return;
	BlastProjectile(_result.ImpactPoint);
}

void AScoutProjectile::BlastProjectile(const FVector& _origin)
{
	ATriggerProjectileSphere* _triggerSphere = GetWorld()->SpawnActor<ATriggerProjectileSphere>(triggerSphere, _origin, GetActorRotation());
	_triggerSphere->SetTimeDamage(timeDamageBlast);
	_triggerSphere->SetDamage(damage);
	_triggerSphere->SetLifeSpan(timeDuringBlast);
	_triggerSphere->SetActorScale3D(_triggerSphere->GetActorScale3D() * radiusBlast);
}
