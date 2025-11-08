// Fill out your copyright notice in the Description page of Project Settings.


#include "FirefliesActor.h"
#include <Kismet/KismetSystemLibrary.h>
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"

void AFirefliesActor::BeginPlay()
{
	Super::BeginPlay();
	gameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hasAuthority = HasAuthority();
}

void AFirefliesActor::InitMember()
{
	if (!projectileRef)
	{
		LOG_ERROR(true, "FireFlies : ProjectileRef is NULL !!!");
		canShot = false;
		return;
	}
	Super::InitMember();
	cam = mainCharacter->GetCameraComponent()->GetCamera();
	overheatMaxReticle = (percOverheatMinReticle * maxOverheated) / 100;
}

void AFirefliesActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (!mainCharacter || !mainCharacter->GetIsOwner())
		return;

	const bool _aim = mainCharacter->GetIsAimingLocal();

	if (_aim)
		AimMode();
	if (!_aim && currentEnemyView > 0)
		ClearAllDrone();
	if (canDownOverheat)
	{
		currentOverheat = FMath::FInterpConstantTo(currentOverheat, 0, _deltaTime, speedDownOverheat);
		if (currentOverheat < overheatMaxReticle)
			onFirefliesShoot.Broadcast((currentOverheat * 100) / overheatMaxReticle);
	}
	else if (canShot && isPressed && !isOverheat)
	{
		Press_Internal();
		currentShoot = 2;
	}

	if (!firstAim && !_aim)
		firstAim = true;

}

void AFirefliesActor::AimMode()
{
	//if (firstAim)
	//{
	//	firstAim = false;
	//	ClearAllDrone();
	//}
	TArray<FHitResult> _allEnemy;
	if (!cam)
	{
		LOG_ERROR(true, "FireFlies : cam is NULL !!!");
		return;
	}

	//const bool _hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), cam->GetComponentLocation(), range, layerShoot, nullptr, TArray<AActor*>(), _allEnemy);
	const bool _hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), cam->GetComponentLocation(), cam->GetComponentLocation(), range, layerShoot, true, TArray<AActor*>(), EDrawDebugTrace::None, _allEnemy, true);
		if (_hit)
			MarkMode(_allEnemy);
}

void AFirefliesActor::PressBehavior()
{
	canDownOverheat = false;
	GetWorldTimerManager().ClearTimer(timerOverheat);
	Super::PressBehavior();
	Press_Internal();
}

void AFirefliesActor::Press_Internal()
{
	StartCooldown();
	for (int i = 0; i < currentEnemyView; i++)
	{
		targetIndex = i;
		ShootWithTarget();
		if (isOverheat)
			break;
	}

	if (currentEnemyView == 0)
		ShootFirefliesWithoutTarget();

	if (canTimerSingleShoot)
	{
		canTimerSingleShoot = false;
		GetWorldTimerManager().SetTimer(timerSingleShoot, this, &AFirefliesActor::IsSingleShoot, timeSingleShoot);
	}

	TiltShoot();
}

void AFirefliesActor::ReleaseBehavior()
{
	if (!isPressed)
		return;

	if (currentShoot <2)
		onSingleShoot.Broadcast();
	else
		onAutoRelease.Broadcast();

	FTimerManager& _manager = GetWorldTimerManager();
	if (_manager.IsTimerActive(timerSingleShoot))
		_manager.ClearTimer(timerSingleShoot);
	
	if (!isOverheat)
	{
		FTimerDelegate _delegate;
		_delegate.BindLambda([&]() {canDownOverheat = true;});
		_manager.SetTimer(timerOverheat, _delegate, timeCanOverheat, FTimerManagerTimerParameters());
	}

	canTimerSingleShoot = true;
	currentShoot = 0;
	isPressed = false;
}

bool AFirefliesActor::CanUse()
{
	return Super::CanUse() && !isOverheat;
}

void AFirefliesActor::ResetMember()
{
	if (!isOverheat)
		canDownOverheat = true;

	if (currentShoot > 1)
		onAutoRelease.Broadcast();

	FTimerManager& _timerManager = GetWorldTimerManager();
	if (_timerManager.IsTimerActive(timerSingleShoot))
		_timerManager.ClearTimer(timerSingleShoot);

	canTimerSingleShoot = true;
	currentShoot = 0;

	Super::ResetMember();
}

void AFirefliesActor::ShootWithTarget()
{
	if (currentOverheat < overheatMaxReticle)
		onFirefliesShoot.Broadcast((currentOverheat * 100) / overheatMaxReticle);

	Overheat();

	const FVector& _direction = cam->GetForwardVector();

	CallOnShoot(memberMesh->GetSocketLocation("Shoot_Loc"), _direction);
	Shoot(_direction, targetIndex);
	targetIndex = -1;

	//mainCharacter->GetCharaAnim()->SetAnimWeight(1);
	onUseRightArm.Broadcast(true, FMath::RandRange(0, 3));
}

void AFirefliesActor::ShootFirefliesWithoutTarget()
{
	if (currentOverheat < overheatMaxReticle)
		onFirefliesShoot.Broadcast((currentOverheat * 100) / overheatMaxReticle);

	Overheat();

	const FVector& _direction = cam->GetForwardVector();

	CallOnShoot(memberMesh->GetSocketLocation("Shoot_Loc"), _direction);
	Shoot(_direction);

	//mainCharacter->GetCharaAnim()->SetAnimWeight(1);
	onUseRightArm.Broadcast(true, FMath::RandRange(0, 3));
}

AFireFliesProjectile* AFirefliesActor::InitProjectile(const FVector& _direction)
{
	const FVector& _memberLocation = memberMesh->GetSocketLocation("Shoot_Loc");
	const FRotator& _rotateProjectile = UKismetMathLibrary::FindLookAtRotation(_memberLocation, _memberLocation + _direction);
	AFireFliesProjectile* _projectile = GetWorld()->SpawnActor<AFireFliesProjectile>(projectileRef, _memberLocation, _rotateProjectile);
	_projectile->SetBonusDamage(bonusDamage);
	_projectile->SetDamage(damage);
	_projectile->SetBleedBonusParams(bleedBonusParams.value, bleedBonusParams.duration, bleedRate);
	_projectile->SetPlayer(mainCharacter);
	SongShootFireFlies();
	return _projectile;
}

void AFirefliesActor::SongShootFireFlies()
{
	//if (currentShoot > 1)
	//	onAutoShoot.Broadcast();
	//currentShoot++;
}

void AFirefliesActor::IsSingleShoot()
{
	GetWorldTimerManager().ClearTimer(timerSingleShoot);
	onAutoShoot.Broadcast();
	currentShoot = 2;
}

void AFirefliesActor::Shoot(const FVector& _direction)
{
	AFireFliesProjectile* _projectile = InitProjectile(_direction);

	if (hasAuthority)
		ClientRpc_SpawnFirefliesWithoutTarget(_direction);
	else
		ServerRpc_SpawnFirefliesWithoutTarget(_direction);

	_projectile->SetDirection(_direction);
	_projectile->InitActor(true);
}

void AFirefliesActor::Shoot(const FVector& _direction, const int _targetIndex)
{
	AFireFliesProjectile* _projectile = InitProjectile(_direction);
	TPair<float, float> _curveValue = _projectile->RandomValue();

	if (hasAuthority)
		ClientRpc_SpawnFirefliesWithTarget(_direction, _targetIndex, _curveValue.Key, _curveValue.Value);
	else
		ServerRpc_SpawnFirefliesWithTarget(_direction, _targetIndex, _curveValue.Key, _curveValue.Value);

	if (_targetIndex >= allEnemyInView.Num())
		return;

	_projectile->SetTarget(allEnemyInView[_targetIndex]);
	_projectile->InitActor(true);
}

void AFirefliesActor::Overheat()
{
	float _addOverheat = 0;
	if (currentMark != 1)
	{
		const float _percentage = (currentMark) / maxMark;
		_addOverheat = overheatMinToShoot + (_percentage * (overheatMaxToShoot - overheatMinToShoot));
	}
	else
		_addOverheat = overheatMinToShoot;

	currentOverheat += _addOverheat;

	if (currentOverheat > maxOverheated)
	{
		currentOverheat = maxOverheated;
		//isOverheat = true;
		//if (currentShoot > 1)
		//	onAutoRelease.Broadcast();
		//onOverheat.Broadcast(isOverheat);
		//GetWorldTimerManager().SetTimer(timerCooling, this, &AFirefliesActor::Cooling, timeCooling);
	}
}

void AFirefliesActor::MarkMode(const TArray<FHitResult>& _allResult)
{
	if (currentMark >= maxMark)
		return;

	const UWorld* _world = GetWorld();
	const FVector& _memberLocation = GetActorLocation();
	FHitResult _resultBlock;

	for (FHitResult _result : _allResult)
	{
		AActor* _actor = _result.GetActor();
		if (!_actor->GetComponentByClass<ULifeComponent>() || allEnemyMark.Contains(_actor))
			continue;
		if (listTargetAim.Contains(_actor) || !reticle->IsActorInTargetZone(_actor))
			continue;
		if (currentMark >= maxMark)
			return;

		const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(_world, _memberLocation, _result.ImpactPoint, layerObstacle, true, TArray<AActor*>(), EDrawDebugTrace::None, _resultBlock, true);
		if (_hit)
			continue;
		SpawnDrone(_actor);
		listTargetAim.Add(_actor);
		currentMark++;
	}
}

void AFirefliesActor::MarkEnemy(AActor* _actor, ADroneFireflies* _drone)
{
	allEnemyInView.Add(_actor);
	currentEnemyView++;
	listTargetAim.Remove(_actor);
	if (hasAuthority)
		ClientRpc_AddEnemyInView(_actor);
	else
		ServerRpc_AddEnemyInView(_actor);
	if(!allEnemyMark.Contains(_drone))
	{
		allEnemyMark.Add(_actor);
		_drone->OnOutView().AddUniqueDynamic(this, &AFirefliesActor::EnemyOutView);
		//_drone->OnInView().AddUniqueDynamic(this, &AFirefliesActor::EnemyInView);
	}
	onMark.Broadcast(currentEnemyView, _actor);
}

void AFirefliesActor::SpawnDrone(AActor* _target)
{
	float _timeMark = 0;
	const float _distanceAI = FVector::Dist(_target->GetActorLocation(), mainCharacter->GetActorLocation());
	if (_distanceAI <= distanceMin)
		_timeMark = timeMinDistance;
	else if (_distanceAI >= distanceMax)
		_timeMark = timeMaxDistance;
	else
	{
		const float _percentage = (_distanceAI - distanceMin) / (distanceMax - distanceMin);
		_timeMark = timeMinDistance + (_percentage * (timeMaxDistance - timeMinDistance));
	}
	ADroneFireflies* _drone = GetWorld()->SpawnActor<ADroneFireflies>(drone, _target->GetActorLocation(), GetActorRotation());
	_drone->InitDrone(_target, _timeMark, durationMark, this);
	_drone->AttachToActor(_target, FAttachmentTransformRules::KeepWorldTransform);
	_drone->OnDisableMark().AddUniqueDynamic(this, &AFirefliesActor::DisableMark);
	_drone->OnMarkConfirmed().AddDynamic(this, &AFirefliesActor::MarkEnemy);
	_drone->OnOutView().AddUniqueDynamic(this, &AFirefliesActor::EnemyOutView);
	//_drone->OnInView().AddUniqueDynamic(this, &AFirefliesActor::EnemyInView);
	allDrone.Add(_drone);
}

void AFirefliesActor::Cooling()
{
	isOverheat = false;
	currentOverheat = 0;
	onOverheat.Broadcast(isOverheat);
	onFirefliesShoot.Broadcast((currentOverheat * 100) / overheatMaxReticle);
	canTimerSingleShoot = true;
	currentShoot = 0;
}

void AFirefliesActor::DisableMark(AActor* _target, ADroneFireflies* _drone)
{
	reticle->RemoveActorDetection(_target);
	if (allDrone.Contains(_drone))
		allDrone.Remove(_drone);
	if (listTargetAim.Contains(_target))
		listTargetAim.Remove(_target);
	allEnemyMark.Remove(_target);
	currentMark--;
	//if (allEnemyInView.Contains(_target))
	//{
	//	allEnemyInView.Remove(_target);
	//	if (!hasAuthority)
	//		ServerRpc_RemoveEnemyInView(_target);
	//	else
	//		ClientRpc_RemoveEnemyInView(_target);

	//	currentEnemyView--;
	//}
	//currentMark--;
	//onMark.Broadcast(currentMark, _target);
	//onDisableEnemy.Broadcast(_target);
}
 
void AFirefliesActor::EnemyOutView(AActor* _target)
{
	allEnemyInView.Remove(_target);
	if (listTargetAim.Contains(_target))
		listTargetAim.Remove(_target);
	//allEnemyMark.Remove(_target);
	if (!hasAuthority)
		ServerRpc_RemoveEnemyInView(_target);
	else
		ClientRpc_RemoveEnemyInView(_target);
	//onDisableEnemy.Broadcast(_target);
	currentEnemyView--;
	onMark.Broadcast(currentEnemyView, _target);
}

void AFirefliesActor::EnemyInView(AActor* _target)
{
	//allEnemyInView.Add(_target);
	//if (!hasAuthority)
	//	ServerRpc_AddEnemyInView(_target);
	//else
	//	ClientRpc_AddEnemyInView(_target);
	//currentMark++;
	////allEnemyMark.Add(_target);

	//currentEnemyView++;
	//onMark.Broadcast(currentEnemyView, _target);

}

void AFirefliesActor::ClearAllDrone()
{
	const int _numDrone = allDrone.Num();
	const TArray<ADroneFireflies*> _allDrone = allDrone;
	for (ADroneFireflies* _drone : _allDrone)
	{
		_drone->OnOutView().Broadcast(_drone->GetEnemy());
		_drone->StopMark();
		_drone->DestroyDrone();
	}

	listTargetAim.Empty();
	allEnemyInView.Empty();
	currentEnemyView = 0;
	if (hasAuthority)
		ClientRpc_ClearEnemyInView();
	else
		ServerRpc_ClearEnemyInView();
	onMark.Broadcast(currentEnemyView, nullptr);
}

#pragma region Reseaux
bool AFirefliesActor::ClientRpc_SpawnFirefliesWithTarget_Validate(const FVector& _direction, const int _index, const float _topCurve, const float _angleCurve)
{
	return !(hasAuthority);
}

void AFirefliesActor::ClientRpc_SpawnFirefliesWithTarget_Implementation(const FVector& _direction, const int _index, const float _topCurve, const float _angleCurve)
{
	const int _test = allEnemyInView.Num();
	AFireFliesProjectile* _projectile = InitProjectile(_direction);
	_projectile->SetCurveValue(_topCurve, _angleCurve);

	_projectile->SetTarget(allEnemyInView[_index]);
	_projectile->InitActor(false);
	SongShootFireFlies();
}

void AFirefliesActor::ServerRpc_SpawnFirefliesWithTarget_Implementation(const FVector& _direction, const int _index, const float _topCurve, const float _angleCurve)
{
	AFireFliesProjectile* _projectile = InitProjectile(_direction);
	_projectile->SetCurveValue(_topCurve, _angleCurve);
	const int _test = allEnemyInView.Num();
	_projectile->SetTarget(allEnemyInView[_index]);
	_projectile->InitActor(false);
	SongShootFireFlies();
}

bool AFirefliesActor::ClientRpc_SpawnFirefliesWithoutTarget_Validate(const FVector& _direction)
{
	return !(hasAuthority);
}

void AFirefliesActor::ClientRpc_SpawnFirefliesWithoutTarget_Implementation(const FVector& _direction)
{
	AFireFliesProjectile* _projectile = InitProjectile(_direction);
	_projectile->SetDirection(_direction);
	_projectile->InitActor(false);
	SongShootFireFlies();
}

void AFirefliesActor::ServerRpc_SpawnFirefliesWithoutTarget_Implementation(const FVector& _direction)
{
	AFireFliesProjectile* _projectile = InitProjectile(_direction);
	_projectile->SetDirection(_direction);
	_projectile->InitActor(false);
	SongShootFireFlies();
}




void AFirefliesActor::ServerRpc_AddEnemyInView_Implementation(AActor* _actor)
{
	allEnemyInView.Add(_actor);
	currentEnemyView++;
}

void AFirefliesActor::ServerRpc_RemoveEnemyInView_Implementation(AActor* _actor)
{
	allEnemyInView.Remove(_actor);
	currentEnemyView--;
}

void AFirefliesActor::ServerRpc_ClearEnemyInView_Implementation()
{
	allEnemyInView.Empty();
	currentEnemyView = 0;
}

bool AFirefliesActor::ClientRpc_AddEnemyInView_Validate(AActor* _actor)
{
	return !(hasAuthority);
}

void AFirefliesActor::ClientRpc_AddEnemyInView_Implementation(AActor* _actor)
{
	allEnemyInView.Add(_actor);
	currentEnemyView++;
}

bool AFirefliesActor::ClientRpc_RemoveEnemyInView_Validate(AActor* _actor)
{
	return !(hasAuthority);
}

void AFirefliesActor::ClientRpc_RemoveEnemyInView_Implementation(AActor* _actor)
{
	allEnemyInView.Remove(_actor);
	currentEnemyView--;
}

bool AFirefliesActor::ClientRpc_ClearEnemyInView_Validate()
{
	return !(hasAuthority);
}

void AFirefliesActor::ClientRpc_ClearEnemyInView_Implementation()
{
	allEnemyInView.Empty();
	currentEnemyView = 0;
}
#pragma endregion





void AFirefliesActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
