// Objectif 3D Copyright


#include "NailgunProjectile.h"
#include <Kismet/KismetSystemLibrary.h>
#include <RobotHunter/AI/Component/Life/LifeComponent.h>
#include <RobotHunter/CCC/Character/MainCharacter.h>
#include <RobotHunter/Game/GPE/Members/RightArmActors/NailGun/NailGunActor.h>
#include <RobotHunter/Network/Component/NetworkGPEComponent.h>
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"

ANailgunProjectile::ANailgunProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bonusDamage = 1.0f;
	bleedRate = 0.0f;
	bleedBonusParams = FStatusParams(EStatusType::BLEED, 0.0f, 0.0f);
}

void ANailgunProjectile::SetNailgun(ANailGunActor* _nailgun)
{
	nailGun = _nailgun;
	SetOwner(nailGun);
}

void ANailgunProjectile::Init(const float _damageMultiplier, ULifeComponent* _life)
{
	damageMultiplier = _damageMultiplier;
	SetLifeSpan(nailLifespan);
	if (_life)
		_life->OnDie().AddUniqueDynamic(this, &ANailgunProjectile::ActorDie);
	damageDiff = maxExplosionDamage - minExplosionDamage;
}

TArray<FHitResult> ANailgunProjectile::MakeDamage()
{
	LOG_COLOR_TIME(NAIL_PROJECTILE_USE_LOG, "DAMAGE", GREEN, 3);
	TArray<AActor*> damagedActors;
	TArray<FHitResult> temp_results;
	//bool _hit = UKismetSystemLibrary::SphereTraceMultiForObjects(this, GetActorLocation(), GetActorLocation(), explosionRadius, explosionDamageLayer, false, {}, EDrawDebugTrace::None, _results, true);
	//if (_hit)
	//{
	//	const int _resultCount = _results.Num();
	//	for (int i = 0; i < _resultCount; i++)
	//	{
	//		if (_results[i].GetActor())
	//		{
	//			//mainCharacter->OnHit().Broadcast(_piercingResults[i]);
	//			ULifeComponent* _life = _results[i].GetActor()->GetComponentByClass<ULifeComponent>();
	//			if (_life && !damagedActors.Contains(_results[i].GetActor()))
	//			{
	//				const float _ratio = 1 - (FVector::Distance(GetActorLocation(), _results[i].ImpactPoint) / explosionRadius);
	//				_life->Damage(minExplosionDamage + (damageDiff * _ratio), nailGun->GetMainCharacter());
	//				damagedActors.Add(_results[i].GetActor());
	//			}
	//			//DRAW_BOX_TIME(_results[i]->GetActorLocation(), FVector(10), FColor::Green, 5);
	//		}
	//	}
	//}
	TArray<AActor*> _results;
	bool _hit = UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), explosionRadius, explosionDamageLayer, AActor::StaticClass(), {this}, _results);
	if (_hit)
	{
		const int _resultCount = _results.Num();
		const FVector& _nailPosition = GetActorLocation();
		for (int i = 0; i < _resultCount; i++)
		{
			if (_results[i])
			{
				//mainCharacter->OnHit().Broadcast(_piercingResults[i]);
				ULifeComponent* _life = _results[i]->GetComponentByClass<ULifeComponent>();
				if (_life && !damagedActors.Contains(_results[i]))
				{
					const FVector& _min = _results[i]->GetActorLocation() + _results[i]->GetRootComponent()->GetLocalBounds().GetBox().Min;
					const FVector& _max = _results[i]->GetActorLocation() + _results[i]->GetRootComponent()->GetLocalBounds().GetBox().Max;
					LOG_COLOR(NAIL_PROJECTILE_USE_LOG, "Min => " + _min.ToString() + " | Max => " + _max.ToString(), BLUE);
					//DRAW_BOX_TIME(_results[i]->GetActorLocation(), FVector(10), FColor::Orange, 5);
					//DRAW_SPHERE_TIME(_min, 10, FColor::Magenta, 5);
					//DRAW_SPHERE_TIME(_max, 10, FColor::Magenta, 5);

					const float _x = _nailPosition.X > _max.X ? _max.X : _nailPosition.X < _min.X ? _min.X : _nailPosition.X;
					const float _y = _nailPosition.Y > _max.Y ? _max.Y : _nailPosition.Y < _min.Y ? _min.Y : _nailPosition.Y;
					const float _z = _nailPosition.Z > _max.Z ? _max.Z : _nailPosition.Z < _min.Z ? _min.Z : _nailPosition.Z;
					const FVector& _impactPosition = FVector(_x, _y, _z);

					//DRAW_SPHERE_TIME(_impactPosition, 10, FColor::Red, 5);

					const float _ratio = 1 - (FVector::Distance(GetActorLocation(), _impactPosition) / explosionRadius);
					const int _damageDiff = FMath::RoundToInt(((float)damageDiff * _ratio));
					LOG_COLOR(NAIL_PROJECTILE_USE_LOG, "Diff => " + FString::FromInt(_damageDiff), BLUE);

					_life->Damage((minExplosionDamage + _damageDiff) * damageMultiplier * bonusDamage, nailGun->GetMainCharacter());
					AddBleed(_results[i]);
					damagedActors.Add(_results[i]);
				}
				//DRAW_BOX_TIME(_results[i]->GetActorLocation(), FVector(10), FColor::Green, 5);
			}
		}
	}
	//DRAW_SPHERE_TIME(GetActorLocation(), explosionRadius, FColor::Red, 0.05f);
	return temp_results;
}


void ANailgunProjectile::Explode(const float _explosionRadius, const float _min, const float _max)
{
	if (!canExploded) return;
	canExploded = false;
	LOG_COLOR_TIME(NAIL_PROJECTILE_USE_LOG, "EXPLODE", ORANGE, 3);
	FTimerHandle _explosionTimer;
	GetWorldTimerManager().SetTimer(_explosionTimer, this, &ANailgunProjectile::MakeExplosion, explosionDelay, false);
	SetLifeSpan(explosionDelay + 10.0f);
	minExplosionDamage = _min;
	maxExplosionDamage = _max;
	explosionRadius = _explosionRadius;
}

void ANailgunProjectile::MakeExplosion()
{
	LOG_COLOR_TIME(NAIL_PROJECTILE_USE_LOG, "MAKE EXPLOSION", GREEN, 3);
	onExplode.Broadcast();
	if (HasAuthority())
		SetLifeSpan(0.1f);
	else
	{
		ServerRpc_CallOnExplosionEvent();
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_DestroyCustomActor(this);
	}

	TArray<FHitResult> _results = MakeDamage();
	for (size_t i = 0; i < _results.Num(); i++)
	{
		if (ANailgunProjectile* _projectile = Cast<ANailgunProjectile>(_results[i].GetActor()))
		{
			LOG_COLOR_TIME(NAIL_PROJECTILE_USE_LOG, "EXPLODE 2", GREEN, 3);
			_projectile->Explode(explosionRadius, minExplosionDamage * bonusDamage, maxExplosionDamage * bonusDamage);
			AddBleed(_results[i].GetActor());
		}
	}
}

void ANailgunProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Destroyed)
		MakeExplosion();
}

void ANailgunProjectile::ActorDie()
{
	canExploded = false;
	Destroy();
}


void ANailgunProjectile::AddBleed(AActor* _target)
{
	if (_target)
	{
		if (bleedBonusParams.value > 0.0f && bleedBonusParams.duration > 0.0f && bleedRate > 0.0f)
		{
			UStatusComponent* _statusComp = _target->GetComponentByClass<UStatusComponent>();

			if (_statusComp)
			{
				UBleedStatus* _bleed = Cast<UBleedStatus>(_statusComp->AddStatus(nailGun->GetOwner(), bleedBonusParams, true));

				if (_bleed)
					_bleed->SetBleedRate(bleedRate);
			}
		}
	}
}


void ANailgunProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANailgunProjectile, nailGun);
}




void ANailgunProjectile::ServerRpc_CallOnExplosionEvent_Implementation()
{
	onExplode.Broadcast();
}

bool ANailgunProjectile::ClientRpc_CallOnExplosionEvent_Validate()
{
	return !(HasAuthority());
}

void ANailgunProjectile::ClientRpc_CallOnExplosionEvent_Implementation()
{
	onExplode.Broadcast();
}
