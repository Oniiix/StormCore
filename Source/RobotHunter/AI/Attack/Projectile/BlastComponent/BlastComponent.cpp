


#include "BlastComponent.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RobotHunter/AI/Attack/Projectile/ParabolProjectile/Blast/TriggerProjectileSphere.h"

UBlastComponent::UBlastComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBlastComponent::Init(UProjectileStats* _stats)
{
	owner = Cast<AProjectile>(GetOwner());
	onBlast.AddDynamic(this, &UBlastComponent::ActivateBlast);
	sphereRadius = _stats->GetHeightSphere();
	collisionBlast = _stats->GetCollisionTrigger();
	isFloor = _stats->GetIsOnFloor();
	statsProjectile = _stats;
}

void UBlastComponent::ActivateBlast(const FVector& _actorLocation, const FHitResult& _result)
{
	FVector _position = _actorLocation;
	if (isFloor)
	{
		FHitResult _resultFloor;
		const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _actorLocation, _actorLocation - owner->GetActorUpVector() * 10000, statsProjectile->GetLayerFloor(), false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, _resultFloor, true);
		if (_hit)
			_position = _resultFloor.ImpactPoint;
	}
	ATriggerProjectileSphere* _trigger = GetWorld()->SpawnActor<ATriggerProjectileSphere>(statsProjectile->GetTriggerRef(), _position, owner->GetActorRotation());
	_trigger->InitTrigger(collisionBlast, statsProjectile->GetHeightSphere(), _result);
	if (arena)
		arena->OnClearArena().AddUniqueDynamic(_trigger, &ATriggerProjectileSphere::ClearArena);
}
