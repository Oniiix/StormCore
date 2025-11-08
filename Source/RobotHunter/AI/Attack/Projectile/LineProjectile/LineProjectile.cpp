// Fill out your copyright notice in the Description page of Project Settings.


#include "LineProjectile.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/Component/NetworkAIComponent.h"

void ALineProjectile::InitProjectile(const FVector& _targetLocation)
{
	canMove = false;
	Super::InitProjectile(_targetLocation);
	direction = GetActorLocation();
	lineProjectileStat = Cast<ULineProjectileStat>(stats);
	if (!lineProjectileStat)
	{
		LOG_ERROR(LINE_PROJECTILE_USE_LOG, "Failed to cast stats in ULineProjectileStat");
		return;
	}
	SetLifeSpan(lineProjectileStat->GetLifeSpan());
	UNetworkAIComponent* _net = GetWorld()->GetFirstPlayerController()->GetComponentByClass< UNetworkAIComponent>();

	if (HasAuthority())
		SetDirection(targetLocation - startLocation);
	else
		_net->ServerRPC_SetDirection(this, targetLocation - startLocation);
	canMove = true;
}

void ALineProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (!canMove)
		return;
	MoveToLine();
}

void ALineProjectile::MoveToLine()
{
	const FVector& _actorLocation = GetActorLocation();
	const FVector& _newLocation = FMath::Lerp(_actorLocation, _actorLocation + direction.GetSafeNormal(), GetWorld()->DeltaTimeSeconds * speed);
	SetActorLocationProjectile(_newLocation);
}

void ALineProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALineProjectile, direction);
}
