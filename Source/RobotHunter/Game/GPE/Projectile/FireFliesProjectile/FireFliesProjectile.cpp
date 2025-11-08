// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFliesProjectile.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include <RobotHunter/Utils/DebugUtils.h>
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"

AFireFliesProjectile::AFireFliesProjectile()
{
	PrimaryActorTick.bCanEverTick = true;


	fireCollision = CreateDefaultSubobject<USphereComponent>("FireCollision");
	RootComponent = fireCollision;

	bonusDamage = 1.0f;
	bleedRate = 0.0f;
	bleedBonusParams = FStatusParams(EStatusType::BLEED, 0.0f, 0.0f);
}

#if WITH_EDITOR
void AFireFliesProjectile::PostEditChangeProperty(FPropertyChangedEvent& _propertyChangedEvent)
{
	if (_propertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AFireFliesProjectile, maxAngleCurve))
		if (maxAngleCurve < minAngleCurve)
			maxAngleCurve = minAngleCurve;

	if (_propertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AFireFliesProjectile, minAngleCurve))
		if(minAngleCurve> maxAngleCurve)
			maxAngleCurve = minAngleCurve;
}
#endif

void AFireFliesProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifeSpan);
}

void AFireFliesProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (!isInit)
		return;
	const FVector& _actorLocation = GetActorLocation();
	target ? MoveActor(target, _actorLocation) : MoveDirection(direction, _actorLocation);
}

void AFireFliesProjectile::MoveActor(const AActor* _target, const FVector& _actorLocation)
{
	FVector::Distance(GetActorLocation(), target->GetActorLocation()) <= distanceOfTarget ? MoveToTarget(_target, _actorLocation) : MoveCurve();

	//const FVector& _newLocationMesh = FMath::VInterpConstantTo(fireCollision->GetRelativeLocation(), meshDestination, GetWorld()->DeltaTimeSeconds, speedMesh);
	//fireCollision->SetRelativeLocation(_newLocationMesh);

	//if (FVector::Distance(meshDestination, _newLocationMesh) <= 10)
	//	meshDestination = meshDestination * -1;
}

void AFireFliesProjectile::MoveToTarget(const AActor* _target, const FVector& _actorLocation)
{
	const FVector& _targetLocation = _target->GetActorLocation();
	const FVector& _newLocation = FMath::VInterpConstantTo(_actorLocation, _targetLocation, GetWorld()->DeltaTimeSeconds, speed);
	direction = _targetLocation - _actorLocation;

	SetActorLocation(_newLocation, true, &moveResult);
	if (moveResult.GetActor())
		MakeDamage(moveResult);

	const FRotator& _rota = UKismetMathLibrary::FindLookAtRotation(_actorLocation, _targetLocation);
	SetActorRotation(_rota);
}

void AFireFliesProjectile::MoveCurve()
{
	if (index < allPoints.Num())
	{
		const FVector& _newLocation = FMath::VInterpConstantTo(GetActorLocation(), allPoints[index], GetWorld()->DeltaTimeSeconds, speed);

		SetActorLocation(_newLocation, true, &moveResult);
		if (moveResult.GetActor())
			MakeDamage(moveResult);

		if (FVector::DistXY(_newLocation, allPoints[index]) <= 10)
		{
			index++;
			if (index < allPoints.Num())
			{
				const FRotator& _rota = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), allPoints[index]);
				SetActorRotation(_rota);
			}
		}
	}
}

void AFireFliesProjectile::MoveDirection(const FVector& _direction, const FVector& _actorLocation)
{
	const FVector& _newLocation = FMath::Lerp(_actorLocation, _actorLocation + _direction, GetWorld()->DeltaTimeSeconds * speed);

	SetActorLocation(_newLocation, true, &moveResult);
	if (moveResult.GetActor())
		MakeDamage(moveResult);
}


void AFireFliesProjectile::AddBleed(AActor* _target)
{
	if (_target)
	{
		if (bleedBonusParams.value > 0.0f && bleedBonusParams.duration > 0.0f && bleedRate > 0.0f)
		{
			UStatusComponent* _statusComp = _target->GetComponentByClass<UStatusComponent>();

			if (_statusComp)
			{
				UBleedStatus* _bleed = Cast<UBleedStatus>(_statusComp->AddStatus(player, bleedBonusParams, true));

				if (_bleed)
					_bleed->SetBleedRate(bleedRate);
			}
		}
	}
}


void AFireFliesProjectile::InitActor(const bool _canHit)
{
	canHit = _canHit; 
	//fireCollision->SetGenerateOverlapEvents(true);

	//fireCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFireFliesProjectile::OnOverlap);

	if (!target)
	{
		isInit = true;
		return;
	}

	ULifeComponent* _lifeComponent = target->GetComponentByClass<ULifeComponent>();
	if (!_lifeComponent)
	{
		isInit = true;
		return;
	}
	_lifeComponent->OnDie().AddUniqueDynamic(this, &AFireFliesProjectile::TargetDie);


	if (topCurve < 0)
		maxRangeMesh = maxRangeMesh * -1;

	direction = GetActorForwardVector();
	SetMiddleCurve(target->GetActorLocation(), GetActorLocation());
	SetAllPoint(CreateCurve(target->GetActorLocation()));


	meshDestination = FVector(0, maxRangeMesh, 0);

	isInit = true;

}

void AFireFliesProjectile::SetMiddleCurve(const FVector& _goalLocation, const FVector& _baseLocation)
{
	const FVector& _middle = (_goalLocation + _baseLocation) / 2;
	float	_z = FMath::Sin(FMath::DegreesToRadians(angleCurve)) * FMath::Abs(topCurve);
	middleCurve = _middle + (GetActorRightVector() * topCurve) + FVector(0, 0, _z);
}

TArray<FVector> AFireFliesProjectile::CreateCurve(const FVector& _goalPosition)const
{
	TArray<FVector> _pointsProjectile;
	float _t;
	float _definitionCurve = 10;
	for (float i = 0; i <= _definitionCurve; i++)
	{
		_t = i / _definitionCurve;
		_pointsProjectile.Add(GetMovementCurve(GetActorLocation(), middleCurve, _goalPosition, _t));
	}
	return _pointsProjectile;
}

FVector AFireFliesProjectile::GetMovementCurve(const FVector& _pos1, const FVector& _pos2, const FVector& _pos3, const float _time) const
{
	const FVector& _lerp1 = FMath::Lerp(_pos1, _pos2, _time);
	const FVector& _lerp2 = FMath::Lerp(_pos2, _pos3, _time);

	return FMath::Lerp(_lerp1, _lerp2, _time);
}

TPair<float, float> AFireFliesProjectile::RandomValue()
{
	topCurve = FMath::RandRange(minRangeCurve, maxRangeCurve);
	angleCurve = FMath::RandRange(minAngleCurve, maxAngleCurve);
	return { topCurve, angleCurve };
}

void AFireFliesProjectile::TargetDie()
{
	target = nullptr;
}
//
//void AFireFliesProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (canHit)
//	{
//		ULifeComponent* _lifeComponent = OtherActor->GetComponentByClass<ULifeComponent>();
//		if (_lifeComponent)
//		{
//			float _damage = damage;
//			if (OtherComp->IsA<UWeakPointComponent>())
//			{
//				_damage *= Cast<UWeakPointComponent>(OtherComp)->GetDamageMultiplier();
//				LOG_COLOR(true, "c'est un Weak Point", BLUE);
//			}
//			LOG_COLOR(true, "bonus Damage => " + FString::FromInt(bonusDamage), BLUE);
//			_lifeComponent->Damage(_damage * bonusDamage, player);
//			AddBleed(OtherActor);
//		}
//	}
//	onHit.Broadcast(SweepResult);
//	Destroy();
//}

void AFireFliesProjectile::MakeDamage(const FHitResult& _result)
{
	if (canHit && _result.GetActor())
	{
		ULifeComponent* _lifeComponent = _result.GetActor()->GetComponentByClass<ULifeComponent>();
		if (_lifeComponent)
		{
			float _damage = damage;
			if (_result.GetComponent()->IsA<UWeakPointComponent>())
				_damage *= Cast<UWeakPointComponent>(_result.GetComponent())->GetDamageMultiplier();

			_lifeComponent->Damage(_damage * bonusDamage, player);
			AddBleed(_result.GetActor());
		}
	}
	onHit.Broadcast(_result);
	Destroy();
}
