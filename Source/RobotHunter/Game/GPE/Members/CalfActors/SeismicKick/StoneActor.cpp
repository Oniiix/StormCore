


#include "StoneActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include <Net/UnrealNetwork.h>
#include "SeismicKickDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

AStoneActor::AStoneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = mesh;
}

void AStoneActor::InitStone(const FVector& _initLocation, USeismicKickDataAsset* _settings)
{
	//initSpeed = _settings->GetInitSpeed();
	ejectionForce = _settings->GetEjectionForce();
	ejectionAngle = _settings->GetEjectionAngle();


	SetActorLocation(_initLocation - FVector(0, 0, mesh->Bounds.BoxExtent.Z));
	finalLocation = _initLocation + FVector(0, 0, mesh->Bounds.BoxExtent.Z);
	mesh->OnComponentHit.AddUniqueDynamic(this, &AStoneActor::StoneHit);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AStoneActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (isInit || !HasAuthority()) return;
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), finalLocation, _deltaTime, initSpeed), true, nullptr, ETeleportType::ResetPhysics);
	if (GetActorLocation().Equals(finalLocation, 2))
		InitFinish();
}

void AStoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStoneActor, finalLocation);
}

void AStoneActor::StoneHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UMovableComponent* _comp = OtherActor->GetComponentByClass<UMovableComponent>())
	{
		UCharacterMovementComponent* _movement = OtherActor->GetComponentByClass< UCharacterMovementComponent>();
		if (_movement)
			_movement->MovementMode = EMovementMode::MOVE_Walking;

		touchActor.Add(FSeismicActorData(OtherActor, OtherComp, OtherComp->BodyInstance.bSimulatePhysics, _comp));
		OtherComp->BodyInstance.SetInstanceSimulatePhysics(false, false, true);
		OtherComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Hit.GetActor()->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
	}
}

void AStoneActor::InitFinish()
{
	isInit = true;
	const int _actorCount = touchActor.Num();
	mesh->OnComponentHit.Clear();
	for (int i = 0; i < _actorCount; i++)
	{
		touchActor[i].owner->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		touchActor[i].collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		if (touchActor[i].moveComp)
		{
			if (touchActor[i].moveComp->IsAffectBy(EMovementSource::Seismic))
			{
				touchActor[i].collisionComponent->BodyInstance.SetInstanceSimulatePhysics(touchActor[i].simulatePhysics, false, true);
				FVector _dir = (touchActor[i].owner->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				_dir.Z = 0;
				const FVector& _launchDir = UCustomMathBlueprintLibrary::GetLocalTrigoPointXZ(ejectionAngle, 1, _dir, FVector::UpVector);
				touchActor[i].moveComp->Launch(_launchDir * (500 * ejectionForce));
			}
		}
	}
	touchActor.Empty();
}

void AStoneActor::OnRep_UpdateInit()
{
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
