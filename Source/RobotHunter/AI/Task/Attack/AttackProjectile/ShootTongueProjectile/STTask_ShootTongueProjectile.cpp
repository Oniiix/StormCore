// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_ShootTongueProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/Component/NetworkAIComponent.h"

EStateTreeRunStatus USTTask_ShootTongueProjectile::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	/*hunter = Cast<AAI_Hunter>(owner);
	hunter->OnShootTongue().AddDynamic(this, &USTTask_ShootTongueProjectile::ShootTongue);
	hunter->TongueAttackActivate();*/
	return EStateTreeRunStatus();
}

void USTTask_ShootTongueProjectile::ShootTongue()
{
	//owner->GetWorldTimerManager().SetTimer(shootTimer, this, &USTTask_ShootTongueProjectile::Shoot, owner->GetAttackProjectileComponent()->GetCoolDown(), true, 0.1);
}

void USTTask_ShootTongueProjectile::Shoot()
{
	/*indexDebug++;
	attackComponent = owner->GetAttackProjectileComponent();
	if (!attackComponent)
	{
		LOG_ERROR(TASK_SHOOT_TONGUE_PROJECTILE_USE_LOG, "attackComponent is null !!");
		return;
	}
	UNetworkAIComponent* _net = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkAIComponent>();
	if (!_net)
	{
		LOG_ERROR(TASK_SHOOT_TONGUE_PROJECTILE_USE_LOG, "_net is null !!");
		return;
	}
	_net->ServerRPC_Shoot(attackComponent, target);*/
}

EStateTreeRunStatus USTTask_ShootTongueProjectile::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	if (!target)
		return EStateTreeRunStatus();

	//FVector _targetLocation = target->GetActorLocation();
	//FVector _ownerLocation = owner->GetActorLocation();

	//FRotator _rota = UKismetMathLibrary::FindLookAtRotation(_ownerLocation, _targetLocation);

	//owner->SetActorRotation(_rota);
	return EStateTreeRunStatus();
}

void USTTask_ShootTongueProjectile::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	//hunter->EndTongueAttack();
	//owner->GetWorldTimerManager().ClearTimer(shootTimer);
}
