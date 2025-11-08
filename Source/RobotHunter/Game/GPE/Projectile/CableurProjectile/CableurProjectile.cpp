// Objectif 3D Copyright


#include "CableurProjectile.h"
#include <RobotHunter/Utils/DebugUtils.h>
#include <NiagaraFunctionLibrary.h>
#include <RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h>

ACableurProjectile::ACableurProjectile()
{
	cableNiagaraSceneComp = CreateDefaultSubobject<USceneComponent>("Niagara");
	cableNiagaraSceneComp->SetupAttachment(RootComponent);
	hitBox = CreateDefaultSubobject<UBoxComponent>("BOX");
	hitBox->SetupAttachment(RootComponent);
}

void ACableurProjectile::BeginPlay()
{
	Super::BeginPlay();
	hitBox->SetVisibility(true);
	hitBox->SetHiddenInGame(false);
	hitBox->OnComponentBeginOverlap.AddDynamic(this, &ACableurProjectile::Bounce);
}

void ACableurProjectile::Init()
{
	FTimerHandle _timer;
	GetWorldTimerManager().SetTimer(_timer, this, &ACableurProjectile::SpawnNiagara, 0.5f);
	FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),startLocation);
	hitBox->SetRelativeRotation(_rotation);
	hitBox->SetBoxExtent(FVector(FVector::Dist(startLocation, endLocation) / 2, 10, 10));
	SetLifeSpan(platformLifetime);
	nail1->SetLifeSpan(platformLifetime);
	nail2->SetLifeSpan(platformLifetime);
}

void ACableurProjectile::Bounce(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (UMovableComponent* _movable = _OtherActor->GetComponentByClass<UMovableComponent>())
	{
		if (_movable->IsAffectBy(EMovementSource::Cableur))
		{
			_movable->Launch(GetActorUpVector() * launchingForce);
		}
	}
}

void ACableurProjectile::SpawnNiagara()
{
	LOG(true, "C'est bon");
	onSetupCable.Broadcast(startLocation, endLocation);
}