#include "TrackingSystemComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"

UTrackingSystemComponent::UTrackingSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	target = nullptr;
	isInitializationGood = false;
}

void UTrackingSystemComponent::InitTracking()
{
	/// Check Owner availability
	owner = Cast<AAI_Base>(GetOwner());
	if (!owner)
	{
		LOG_ERROR(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Failed to cast Owner in AI_Base !!");
		return;
	}

	/// Check anim availability
	AIBaseAnimInstance = Cast<UAIBaseAnimInstance>(owner->GetAnimInstance());
	if (!AIBaseAnimInstance)
	{
		LOG_ERROR(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Failed to cast AnimInstance in AIBaseAnimInstance !!");
		return;
	}

	/// Check damageComponent availability
	damageSystem = owner->GetDamagePerceptionComponent();
	if (!damageSystem)
	{
		LOG_WARNING(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Failed to get PerceptionComponents !!");
		return;
	}
	damageSystem->OnDetected().AddUniqueDynamic(this, &UTrackingSystemComponent::TakeDamage);

	/// Disable tracking behaviour
	if (disableTracking)
	{
		SetComponentTickEnabled(false);
		LOG_WARNING(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Warning : TrackingSystem disable");
		return;
	}

	isLookingAtTarget = false;
	isInitializationGood = true;
}

void UTrackingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isInitializationGood)
		return;

	LookAtTarget(DeltaTime);
	DrawDebug();
}

void UTrackingSystemComponent::LookAtTarget(const float _deltaTime)
{
	if (!target || !isLookingAtTarget)
		return;

	const FRotator& _ownerRotation = owner->GetActorRotation();
	const FRotator& _lookAtTarget = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), target->GetActorLocation());

	const float _lerpYaw = FMath::FixedTurn(_ownerRotation.Yaw, _lookAtTarget.Yaw, _deltaTime * owner->GetRotationSpeed() * 100.f);
	FRotator _targetRotation = _ownerRotation;
	_targetRotation.Yaw = _lerpYaw;
	owner->SetActorRotation(_targetRotation);

	/// Update rotation animation
	const float _cross = FVector::CrossProduct(_ownerRotation.Vector(), _lookAtTarget.Vector()).Z;
	const float _rotationAngle = 180.f * _cross;
	AIBaseAnimInstance->SetCurrentAngle(_rotationAngle);

	if (_targetRotation.Equals(_ownerRotation, 2.f))
		onEndLookAtTarget.Broadcast();
}

void UTrackingSystemComponent::SetLookAtTarget(const bool _value)
{
	isLookingAtTarget = _value;
}

void UTrackingSystemComponent::SetTarget(AMainCharacter* _target)
{
	target = _target;
	UAIDebugInfoWidgetComponent* _debugInfoComponent = owner->GetDebugInfoComponent();
	if (_debugInfoComponent)
		_debugInfoComponent->SetTargetInfo(target->ACTOR_NAME);
	onTargetAcquire.Broadcast(target);
}

void UTrackingSystemComponent::TakeDamage(AMainCharacter* _player)
{
	LOG_COLOR(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Take damage from " + _player->ACTOR_NAME, TURQUOISE);
	onDetection.Broadcast();
}

void UTrackingSystemComponent::LoseTarget()
{
	if (!target)
		return;

	LOG_COLOR(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Lost " + target->ACTOR_NAME, TURQUOISE);
	onTargetLost.Broadcast();
	target = nullptr;
	isLookingAtTarget = false;
	UAIDebugInfoWidgetComponent* _debugInfoComponent = owner->GetDebugInfoComponent();
	if (_debugInfoComponent)
		_debugInfoComponent->SetTargetInfo("None");
}

void UTrackingSystemComponent::DrawDebug()
{
	if (!owner->GetDrawDebug() || !owner->IsEnable() || !drawDebug || !target)
		return;

	DRAW_LINE(GetOwnerLocation(), target->GetActorLocation(), FColor::Black);
}

void UTrackingSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTrackingSystemComponent, drawDebug);
}
