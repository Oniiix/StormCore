// Objectif 3D Copyright


#include "CableurActor.h"
#include <RobotHunter/UI/HUD/GameHUD.h>
#include <NiagaraFunctionLibrary.h>
#include <RobotHunter/Utils/CustomMathBlueprintLibrary.h>
#include "RobotHunter/CCC/Character/MainCharacter.h"


void ACableurActor::InitMember()
{
	Super::InitMember();
	camera = mainCharacter->GetCameraComponent()->GetCamera();
}

void ACableurActor::PressBehavior()
{
	//if (!canUse || !canShot) return;
	Super::PressBehavior();

	FHitResult _result;
	FPositionViewing _viewPoint = FPositionViewing(camera->GetComponentLocation(), camera->GetForwardVector());
	//onShoot.Broadcast(_viewPoint.direction);
	bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _viewPoint.position, _viewPoint.position + rayLength * _viewPoint.direction, layer, true, {}, EDrawDebugTrace::ForDuration, _result, true);
	
	if (_hit)
	{
		CallOnShoot(memberMesh->GetSocketLocation("Shoot_Loc"), (_result.Location - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());
		if (!hasFirstLocation)
		{
			SpawnFirstNail(_result.ImpactPoint);
			if (HasAuthority())
				ClientRpc_SpawnFirstCable(_result.ImpactPoint);
			else
				ServerRpc_SpawnFirstCable(_result.ImpactPoint);
		}
		else
		{
			SpawnSecondNail(_result.ImpactPoint);
			if (HasAuthority())
				ClientRpc_SpawnSecondCable(_result.ImpactPoint);
			else
				ServerRpc_SpawnSecondCable(_result.ImpactPoint);
		}
		
	}
	//canUse = false;
}

void ACableurActor::InitCable(ACableurProjectile* _cable)
{
	_cable->SetStartLocation(firstPosition);
	_cable->SetEndLocation(secondPosition);
	_cable->SetNail1(firstNail);
	_cable->SetNail2(secondNail);
	_cable->Init();
}

void ACableurActor::ReleaseBehavior()
{
	//	canUse = true;
	isPressed = false;
}

void ACableurActor::SpawnFirstNail(const FVector& _position)
{
	firstPosition = _position + FVector(0, 0, 100);
	LOG(true, firstPosition.ToString());
	hasFirstLocation = true;
	firstNail = GetWorld()->SpawnActor<ACableurNail>(nailProjectile, _position, FRotator::ZeroRotator);
}

void ACableurActor::SpawnSecondNail(const FVector& _position)
{
	secondPosition = _position + FVector(0, 0, 100);
	if (UCustomMathBlueprintLibrary::IsFloatInRange(FVector::Dist(firstPosition, secondPosition), minDistance, maxDistance))
	{
		FVector _location = FVector((firstPosition.X + secondPosition.X) / 2, (firstPosition.Y + secondPosition.Y) / 2, (firstPosition.Z + secondPosition.Z) / 2);
		secondNail = GetWorld()->SpawnActor<ACableurNail>(nailProjectile, _position, FRotator::ZeroRotator);
		ACableurProjectile* _cable = GetWorld()->SpawnActor<ACableurProjectile>(cableProjectile, _location, FRotator::ZeroRotator);
		InitCable(_cable);
		hasFirstLocation = false;
		if (mainCharacter->GetIsOwner())
			StartCooldown();
	}
}


void ACableurActor::ServerRpc_SpawnFirstCable_Implementation(const FVector& _position)
{
	SpawnFirstNail(_position);
}


bool ACableurActor::ClientRpc_SpawnFirstCable_Validate(const FVector& _position)
{
	return !(HasAuthority());
}
void ACableurActor::ClientRpc_SpawnFirstCable_Implementation(const FVector& _position)
{
	SpawnFirstNail(_position);
}


void ACableurActor::ServerRpc_SpawnSecondCable_Implementation(const FVector& _position)
{
	SpawnSecondNail(_position);
}

bool ACableurActor::ClientRpc_SpawnSecondCable_Validate(const FVector& _position)
{
	return !(HasAuthority());
}
void ACableurActor::ClientRpc_SpawnSecondCable_Implementation(const FVector& _position)
{
	SpawnSecondNail(_position);
}