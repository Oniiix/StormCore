// Fill out your copyright notice in the Description page of Project Settings.


#include "NailGunActor.h"
#include <Kismet/KismetSystemLibrary.h>
#include <RobotHunter/CCC/Character/MainCharacter.h>
#include <RobotHunter/AI/AI_Base.h>
#include <RobotHunter/UI/HUD/GameHUD.h>
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "RobotHunter/AI/Component/Life/WeakPointComponent.h"


void ANailGunActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ANailGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!canShot) return;


	if (isScoped)
		ScopeShoot();
	else
		UnscopeShoot();
}

#pragma region Member_System
void ANailGunActor::InitMember()
{
	Super::InitMember();
	playerCamera = mainCharacter->GetCameraComponent()->GetCamera();
	springArm = mainCharacter->GetCameraComponent()->GetSpringArm();
}

void ANailGunActor::PressBehavior()
{
	Super::PressBehavior();
	SetActorTickEnabled(true);
}

void ANailGunActor::ReleaseBehavior()
{
	isPressed = false;
	SetActorTickEnabled(false);
}

bool ANailGunActor::CanUse()
{
	return !hasDeactivatedStatus && isActivated && !mainCharacter->GetInteractionComponent()->
		GetHasInteracted();
}
void ANailGunActor::ResetMember()
{
	Super::ResetMember();
	SetActorTickEnabled(false);
}
#pragma endregion

#pragma region Recoil
void ANailGunActor::Recoil()
{
	onUseRightArm.Broadcast(true, FMath::RandRange(0, 2));
	if (!HasAuthority())
		ServerRpc_Recoil();
	else
		ClientRpc_Recoil();

}
bool ANailGunActor::ClientRpc_Recoil_Validate()
{
	return !(HasAuthority());
}
void ANailGunActor::ClientRpc_Recoil_Implementation()
{
	onUseRightArm.Broadcast(true, FMath::RandRange(0, 2));
}
void ANailGunActor::ServerRpc_Recoil_Implementation()
{
	onUseRightArm.Broadcast(true, FMath::RandRange(0, 2));
}
#pragma endregion

#pragma region ProjectileGo
void ANailGunActor::CallProjectileGo(const FVector& _startPosition, const FVector& _direction)
{
	onProjectileGo.Broadcast(_startPosition, _direction);
	if (HasAuthority())
		ClientRpc_CallProjectileGo(_startPosition, _direction);
	else
		ServerRpc_CallProjectileGo(_startPosition, _direction);
}
bool ANailGunActor::ClientRpc_CallProjectileGo_Validate(const FVector& _startPosition, const FVector& _direction)
{
	return !(HasAuthority());
}
void ANailGunActor::ClientRpc_CallProjectileGo_Implementation(const FVector& _startPosition, const FVector& _direction)
{
	onProjectileGo.Broadcast(_startPosition, _direction);
}
void ANailGunActor::ServerRpc_CallProjectileGo_Implementation(const FVector& _startPosition, const FVector& _direction)
{
	onProjectileGo.Broadcast(_startPosition, _direction);
}
#pragma endregion

#pragma region Heavy_Recoil
void ANailGunActor::HeavyRecoil()
{
	onHeavyRecoil.Broadcast(true);
	if (HasAuthority())
		ClientRpc_HeavyRecoil();
	else
		ServerRpc_HeavyRecoil();
}
bool ANailGunActor::ClientRpc_HeavyRecoil_Validate()
{
	return !(HasAuthority());
}
void ANailGunActor::ClientRpc_HeavyRecoil_Implementation()
{
	onHeavyRecoil.Broadcast(true);
}
void ANailGunActor::ServerRpc_HeavyRecoil_Implementation()
{
	onHeavyRecoil.Broadcast(true);
}
#pragma endregion

#pragma region Shoot
void ANailGunActor::UnscopeShoot()
{
	FHitResult _result;
	FVector _endPosition;

	const float _currentArmLength = (playerCamera->GetComponentLocation() - springArm->GetComponentLocation()).Length();

	const FVector& _start = playerCamera->GetComponentLocation() + (_currentArmLength) * playerCamera->GetForwardVector();

	const FVector& _end = playerCamera->GetComponentLocation() + rayLength * playerCamera->GetForwardVector();
	_result.Location = playerCamera->GetComponentLocation() + rayLength * playerCamera->GetForwardVector();
	bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _start, _end, layer, false, {}, EDrawDebugTrace::None, _result, true);

	CallOnShoot(memberMesh->GetSocketLocation("Shoot_Loc"), _hit ? (_result.Location - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal() : (_end - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());
	if (_hit)
	{
		_endPosition = _result.Location;
		CallOnHit(_result);

		if (ULifeComponent* _life = _result.GetActor()->GetComponentByClass<ULifeComponent>())
		{
			float _damage = unscopeSettings.damage;
			if (_result.Component->IsA<UWeakPointComponent>())
				_damage *= Cast<UWeakPointComponent>(_result.Component)->GetDamageMultiplier();
			_life->Damage(_damage * bonusDamage, mainCharacter);

			AddBleed(_result.GetActor());
		}
	}
	else
		_endPosition = _end;
		
	if (FVector::Distance(_endPosition, memberMesh->GetSocketLocation("Shoot_Loc")) > minDistanceProjectileGo)
		CallProjectileGo(memberMesh->GetSocketLocation("Shoot_Loc"), (_endPosition - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());



	TiltShoot();
	Recoil();


	StartCooldown();

	//unscopeSettings.canShot = false;
	//GetWorldTimerManager().SetTimer(unscopeSettings.cooldownTimer, unscopeSettings.cooldownDelegate, unscopeSettings.cooldownTime, FTimerManagerTimerParameters());
}

void ANailGunActor::ScopeShoot()
{
	FHitResult _result;

	FVector _endPosition;

	const float _currentArmLength = (playerCamera->GetComponentLocation() - springArm->GetComponentLocation()).Length();
	const FVector& _start = playerCamera->GetComponentLocation() + (_currentArmLength) * playerCamera->GetForwardVector();

	const FVector& _end = playerCamera->GetComponentLocation() + rayLength * playerCamera->GetForwardVector();

	_result.Location = playerCamera->GetComponentLocation() + rayLength * playerCamera->GetForwardVector();
	bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _start, _end, layer, false, {}, EDrawDebugTrace::None, _result, true);

	CallOnShoot(memberMesh->GetSocketLocation("Shoot_Loc"), _hit ? (_result.Location - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal() : (_end - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());


	if (_hit)
	{
		_endPosition = _result.Location;

		ULifeComponent* _life = _result.GetActor()->GetComponentByClass<ULifeComponent>();
		float _damageMultiplier = 1.0f;
		if (_result.GetComponent()->IsA<UWeakPointComponent>())
			_damageMultiplier = Cast<UWeakPointComponent>(_result.GetComponent())->GetDamageMultiplier();

		if (HasAuthority())
		{
			ANailgunProjectile* _projectile = GetWorld()->SpawnActor<ANailgunProjectile>(nailRef, _result.ImpactPoint, UKismetMathLibrary::FindLookAtRotation(_result.TraceStart, _result.ImpactPoint) + FRotator(90.0f, 0.0f, 0.0f));
			_projectile->SetNailgun(this);
			_projectile->AttachToActor(_result.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
			_projectile->SetBonusDamage(bonusDamage);
			_projectile->SetBleedBonusParams(bleedBonusParams.value, bleedBonusParams.duration, bleedRate);
			_projectile->Init(_damageMultiplier, _life);
		}
		else
			ServerRpc_SpawnNail(_result);


		CallOnHit(_result);

		if (_life)
		{
			_life->Damage(scopeSettings.damage * _damageMultiplier * bonusDamage, mainCharacter);

			AddBleed(_result.GetActor());
		}

	}
	else
		_endPosition = _end;


	if (FVector::Distance(_endPosition, memberMesh->GetSocketLocation("Shoot_Loc")) > minDistanceProjectileGo)
		CallProjectileGo(memberMesh->GetSocketLocation("Shoot_Loc"), (_endPosition - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());

	TiltShoot();
	Recoil();

	StartCooldown();

	//scopeSettings.canShot = false;
	//GetWorldTimerManager().SetTimer(scopeSettings.cooldownTimer, scopeSettings.cooldownDelegate, scopeSettings.cooldownTime, FTimerManagerTimerParameters());
}

void ANailGunActor::ServerRpc_SpawnNail_Implementation(const FHitResult& _result)
{
	ANailgunProjectile* _projectile = GetWorld()->SpawnActor<ANailgunProjectile>(nailRef, _result.ImpactPoint, UKismetMathLibrary::FindLookAtRotation(_result.TraceStart, _result.ImpactPoint) + FRotator(90.0f, 0.0f, 0.0f));
	_projectile->SetNailgun(this);
	_projectile->AttachToActor(_result.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
	_projectile->SetBonusDamage(bonusDamage);
	_projectile->SetBleedBonusParams(bleedBonusParams.value, bleedBonusParams.duration, bleedRate);

	float _damageMultiplier = 1.0f;
	if (_result.GetComponent()->IsA<UWeakPointComponent>())
		_damageMultiplier = Cast<UWeakPointComponent>(_result.GetComponent())->GetDamageMultiplier();

	_projectile->Init(_damageMultiplier, _result.GetActor()->GetComponentByClass<ULifeComponent>());
}
#pragma endregion