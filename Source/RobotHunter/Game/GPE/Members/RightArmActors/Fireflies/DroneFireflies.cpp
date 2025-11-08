// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneFireflies.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/Fireflies/FirefliesActor.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"

void ADroneFireflies::BeginPlay()
{
	Super::BeginPlay();
	niagara = GetComponentByClass<UNiagaraComponent>();
}

void ADroneFireflies::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (!player || !cam)
		return;

	const FVector& _direction = (GetActorLocation() - cam->GetComponentLocation()).GetSafeNormal();
	const float _angle = UCustomMathBlueprintLibrary::Angle(cam->GetForwardVector(), _direction);
	isOutView ? IsInView(_angle) : IsOutView(_angle);

	if (stopMark)
	{
		timeElapsed = FMath::FInterpConstantTo(timeElapsed, 0, _deltaTime, speedDestroyDrone);
		onProgress.Broadcast(timeElapsed);
		if (timeElapsed == 0)
			DestroyDrone();
		return;
	}

	const FTimerManager& _timerManager = GetWorldTimerManager();
	if (isCurrentMarkTimer)
	{
		const float _elapsed = _timerManager.GetTimerElapsed(timerMark) / timeMark;
		onProgress.Broadcast(_elapsed);
	}
	else
		elapsedTimeMark = _timerManager.GetTimerElapsed(timerDisableMark);

}

void ADroneFireflies::InitDrone(AActor* _target, const float _timeMark, const float _timeDisableDrone, AFirefliesActor* _arm)
{
	timeMark = _timeMark;
	target = _target;
	player = _arm->GetMainCharacter();
	arm = _arm;
	timeDisableDrone = _timeDisableDrone;

	niagara->SetVisibility(false);

	isCurrentMarkTimer = true;
	GetWorldTimerManager().SetTimer(timerMark, this, &ADroneFireflies::StartMark, _timeMark);

	cam = player->GetCameraComponent()->GetCamera();
	AAI_Base* _ai = Cast<AAI_Base>(_target);
	if (_ai)
	{
		targetSkeletal = _ai->GetMesh()->GetSkeletalMeshAsset();
		AAI_Hunter* _hunter = Cast<AAI_Hunter>(_ai);
		if (_hunter)
			_hunter->GetCamouflageComponent()->OnTransform().AddDynamic(this, &ADroneFireflies::DisableDefMark);
	}
	ACustomActor* _actor = Cast<ACustomActor>(_target);
	if (_actor)
		targetMesh = _actor->GetMesh()->GetStaticMesh();

	ULifeComponent* _lifeComponent = target->GetComponentByClass<ULifeComponent>();
	if (!_lifeComponent)
		return;
	_lifeComponent->OnDie().AddDynamic(this, &ADroneFireflies::DisableDefMark);
	onInit.Broadcast();
}

void ADroneFireflies::StartMark()
{
	niagara->SetVisibility(true);
	isCurrentMarkTimer = false;
	onMarkConfirmed.Broadcast(target, this);
	//GetWorldTimerManager().SetTimer(timerDisableMark, this, &ADroneFireflies::DisableMark, timeDisableDrone);
}

void ADroneFireflies::IsOutView(const float _angle)
{
	if (!arm->GetReticle()->IsActorInTargetZone(this) && !stopMark)
	{
		isOutView = true;
		DisableMark();

	}
}

void ADroneFireflies::IsInView(const float _angle)
{
	if (disableDefMark)
		return;
	if (arm->GetReticle()->IsActorInTargetZone(this))
	{
		isOutView = false;
		onInView.Broadcast(target);
		stopMark = false;
		isCurrentMarkTimer = true;
		GetWorldTimerManager().SetTimer(timerMark, this, &ADroneFireflies::StartMark, timeMark);
	}
}

void ADroneFireflies::DisableMark()
{
	if (stopMark)
		return;
	if (!isCurrentMarkTimer)
		onOutView.Broadcast(target);
	isOutView = true;
	stopMark = true;
	FTimerManager& _timerManager = GetWorldTimerManager();
	if (isCurrentMarkTimer)
	{
		timeElapsed = _timerManager.GetTimerElapsed(timerMark);
		_timerManager.ClearTimer(timerMark);
	}
	else
		timeElapsed = timeMark;
}

void ADroneFireflies::DisableDefMark()
{
	disableDefMark = true;
	DisableMark();
}

void ADroneFireflies::StopMark()
{
	FTimerManager& _manager = GetWorldTimerManager();
	if (GetWorldTimerManager().IsTimerActive(timerMark))
		_manager.ClearTimer(timerMark);
}

void ADroneFireflies::DestroyDrone()
{
	onDisableMark.Broadcast(target, this);
	GetWorldTimerManager().ClearTimer(timerDisableMark);
	Destroy();
}
