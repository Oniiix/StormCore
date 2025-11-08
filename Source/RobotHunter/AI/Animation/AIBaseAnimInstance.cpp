// Fill out your copyright notice in the Description page of Project Settings.

#include "AIBaseAnimInstance.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Components/CapsuleComponent.h"

void UAIBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	InitAnimInstance();
}

void UAIBaseAnimInstance::InitAnimInstance()
{
	owner = Cast<AAI_Base>(GetOwningActor());
	if (!owner)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to cast owner in AAI_Base of " + GetOwningActor()->GetActorNameOrLabel());
		return;
	}
	owner->OnPlayDeath().AddUniqueDynamic(this, &UAIBaseAnimInstance::OnDead);

	characterMovementComponent = owner->GetCharacterMovement();
	if (!characterMovementComponent)
	{
		LOG_ERROR(AI_ANIM_USE_LOG, "Failed to get characterMovementComponent for animation on " + owner->ACTOR_NAME);
		return;
	}

	movableComponent = owner->GetMovableComponent();
	if (!movableComponent)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get UMovableComponent on " + owner->ACTOR_NAME);
		return;
	}
	movableComponent->OnLaunch().AddUniqueDynamic(this, &UAIBaseAnimInstance::Bump);

	UAIMovementComponent* _movementComponent = owner->GetAIMovementComponent();
	if (!_movementComponent)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get UAIMovementComponent on " + owner->ACTOR_NAME);
		return;
	}
	layersGround = _movementComponent->GetLayersGround();

	skeletalMesh = owner->GetMesh();
	if (!skeletalMesh)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get USkeletalMeshComponent on " + owner->ACTOR_NAME);
		return;
	}
	skeletalMesh->bEnableUpdateRateOptimizations = true;
	skeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;

	owner->OnEnable().AddUniqueDynamic(this, &UAIBaseAnimInstance::Enable);

	isInitializationGood = true;
	canPlayAnimation = true;
}

void UAIBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!isInitializationGood || owner->IsDead())
		return;

	currentSpeed = owner->GetVelocity().Length() / owner->GetBlendFactor();
	isOnGround = !characterMovementComponent->IsFalling();

	CalculateIK();
}

void UAIBaseAnimInstance::CalculateIK()
{
	if (!isOnGround)
		return;

	const FVector& _upOffset = owner->GetActorUpVector() * 20.f;
	const FVector& _downOffset = FVector::DownVector * 20.f;

	/// Forward left foot
	FVector _footLocation = skeletalMesh->GetSocketLocation(FLFOOT_SOCKET);
	FVector _start = _footLocation + _upOffset;
	FVector _end = _footLocation + _downOffset;
	FHitResult _footResult;
	bool _footHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _start, _end, layersGround, false, {},
		EDrawDebugTrace::None, _footResult, true);
	if (_footHit)
	{
		const FVector& _FLFootDiff = _footResult.ImpactPoint - _footLocation;
		//LOG_COLOR_TIME(true, "Front left : " + _FLFootDiff.ToString(), GREEN, 0.1f);
		if (!stopIK)
			forwardLeftFootLocation = _FLFootDiff;
	}
	/// Forward right foot
	_footLocation = skeletalMesh->GetSocketLocation(FRFOOT_SOCKET);
	_start = _footLocation + _upOffset;
	_end = _footLocation + _downOffset;
	_footResult;
	_footHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _start, _end, layersGround, false, {},
		EDrawDebugTrace::None, _footResult, true);
	if (_footHit)
	{
		const FVector& _FRFootDiff = _footResult.ImpactPoint - _footLocation;
		//LOG_COLOR(true, "Front right : " + _FRFootDiff.ToString(), GREEN);
		if (!stopIK)
			forwardRightFootLocation = _FRFootDiff;
	}
	/// Backward left foot
	_footLocation = skeletalMesh->GetSocketLocation(BLFOOT_SOCKET);
	_start = _footLocation + _upOffset;
	_end = _footLocation + _downOffset;
	_footResult;
	_footHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _start, _end, layersGround, false, {},
		EDrawDebugTrace::None, _footResult, true);
	if (_footHit)
	{
		const FVector& _BLFootDiff = _footResult.ImpactPoint - _footLocation;
		//LOG_COLOR(true, "Back left : " + _BLFootDiff.ToString(), GREEN);
		if (!stopIK)
			backwardLeftFootLocation = _BLFootDiff;
	}
	/// Backward right foot
	_footLocation = skeletalMesh->GetSocketLocation(BRFOOT_SOCKET);
	_start = _footLocation + _upOffset;
	_end = _footLocation + _downOffset;
	_footResult;
	_footHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _start, _end, layersGround, false, {},
		EDrawDebugTrace::None, _footResult, true);
	if (_footHit)
	{
		const FVector& _BRFootDiff = _footResult.ImpactPoint - _footLocation;
		//LOG_COLOR(true, "Back right : " + _BRFootDiff.ToString(), GREEN);
		if (!stopIK)
			backwardRightFootLocation = _BRFootDiff;
	}
}

void UAIBaseAnimInstance::LaunchAnimation(const EAIAnimation& _animation)
{
	if (!canPlayAnimation)
		return;

	switch (_animation)
	{
	case Spawn:
		canPlayAnimation = false;
		isSpawning = true;
		break;
	case BattleCry:
		canPlayAnimation = false;
		isCrying = true;
		break;
	case Flung_GetUp:
		canPlayAnimation = false;
		break;
	case Null:
		canPlayAnimation = false;
		break;
	}
}

void UAIBaseAnimInstance::EndAnimation()
{
	onEndAnimation.Broadcast();

	isJumping = false;
	jumpAttack = false;
	isCrying = false;
	isSpawning = false;
	isStun = false;
	isBumping = false;

	canPlayAnimation = true;
}

void UAIBaseAnimInstance::EnableHitBox()
{
	onEnableHitBox.Broadcast();
}

void UAIBaseAnimInstance::DisableHitBox()
{
	onDisableHitBox.Broadcast();
}

void UAIBaseAnimInstance::JumpAttack()
{
	jumpAttack = false;
	onJumpAttack.Broadcast();
}

void UAIBaseAnimInstance::Jump()
{
	isJumping = true;
	canPlayAnimation = false;
}

void UAIBaseAnimInstance::EndJump()
{
	isJumping = false;
	onEndJump.Broadcast();
}

void UAIBaseAnimInstance::StartStun(const float _stunTime)
{
	isStun = true;
	isJumping = false;
	jumpAttack = false;
	isCrying = false;
	isSpawning = false;
	canPlayAnimation = false;

	FTimerHandle StartStun;
	GetWorld()->GetTimerManager().SetTimer(StartStun, this, &UAIBaseAnimInstance::EndStun, _stunTime);
}

void UAIBaseAnimInstance::EndStun()
{
	isStun = false;
	canPlayAnimation = true;
	onEndStun.Broadcast();
}

void UAIBaseAnimInstance::Bump()
{
	EndAnimation();

	isBumping = true;
	canPlayAnimation = false;
}

void UAIBaseAnimInstance::OnDead(AAI_Base* _owner)
{
	isDead = true;
}

void UAIBaseAnimInstance::Enable(bool _enable)
{
	owner->GetMesh()->SetComponentTickEnabled(_enable);
}
