#include "OffRoadRollerdrivesActor.h"
#include "../../../../../CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Members/LeftArmActors/LeftArmActor.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/RightArmActor.h"
#include "RobotHunter/Game/GPE/Members/CalfActors/CalfActor.h"

void AOffRoadRollerdrivesActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	/*fullDecreaseTime = (maxStamina / decreaseValue) * decreaseTime;
	fullReloadingTime = ((maxStamina / reloadingValue) * reloadingTime) * cooldownMultiplier;*/
	currentStamina = maxStamina;
}

void AOffRoadRollerdrivesActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (canShot && isPressed)
	{
		if (mainCharacter->GetInputDirection() == FVector2D(0.0f) && isRunning)
			StartReloading();
		else if (isReloading && mainCharacter->GetInputDirection() != FVector2D(0.0f))
		{
			mainCharacter->GetCustomMovementComponent()->NetSetMaxAcceleration(startAcceleration);
			GetWorld()->GetTimerManager().SetTimer(accelerationTimer, FTimerDelegate::CreateLambda([&]()
			{
				mainCharacter->GetCustomMovementComponent()->NetSetMaxAcceleration(defaultAcceleration);
			}), startAccelerationTime, FTimerManagerTimerParameters());
			UseStamina();
		}
	}

	LOG_COLOR_TICK(OFFROAD_USE_LOG, "[Rollers] CurrentStamina => " + FLOATSTR(currentStamina), RED);
	if (GetWorld()->GetTimerManager().IsTimerActive(speedTimer))
	{
		LOG_COLOR_TICK(OFFROAD_USE_LOG, "[Rollers] CurrentTime => "
		               + FString::SanitizeFloat(GetWorld()->GetTimerManager().GetTimerElapsed(speedTimer))
		               + "/" + FLOATSTR(maxDecreaseTime), YELLOW);
		currentStamina = oldStamina * (1 - (GetWorld()->GetTimerManager().GetTimerElapsed(speedTimer) /
			maxDecreaseTime));
	}
	else if (isReloading && GetWorld()->GetTimerManager().IsTimerActive(reloadingTimer))
	{
		LOG_COLOR_TICK(OFFROAD_USE_LOG, "[Rollers] CurrentTime => "
		               + FString::SanitizeFloat(GetWorld()->GetTimerManager().GetTimerElapsed(reloadingTimer))
		               + "/" + FLOATSTR(maxReloadingTime), YELLOW);
		currentStamina = oldStamina + ((maxStamina - oldStamina) * (GetWorld()->GetTimerManager().
			GetTimerElapsed(reloadingTimer) / maxReloadingTime));
	}
	onUseStamina.Broadcast(currentStamina, maxStamina);
}

void AOffRoadRollerdrivesActor::PressBehavior()
{
	Super::PressBehavior();
	if (mainCharacter->GetCharacterMovement()->IsFalling())
	{
		mainCharacter->LandedDelegate.AddUniqueDynamic(this, &AOffRoadRollerdrivesActor::OnLanded);
		return;
	}

	mainCharacter->GetCustomMovementComponent()->NetSetMaxAcceleration(startAcceleration);
	GetWorld()->GetTimerManager().SetTimer(accelerationTimer, FTimerDelegate::CreateLambda([&]()
	{
		mainCharacter->GetCustomMovementComponent()->NetSetMaxAcceleration(defaultAcceleration);
	}), startAccelerationTime, FTimerManagerTimerParameters());
	UseStamina();
}

void AOffRoadRollerdrivesActor::UseStamina()
{
	mainCharacter->GetUsingLeftArm()->OnPress().AddUniqueDynamic(this, &AOffRoadRollerdrivesActor::ResetMember);
	mainCharacter->GetUsingLeftArm()->ResetMember();
	mainCharacter->GetUsingCalf()->OnPress().AddUniqueDynamic(this, &AOffRoadRollerdrivesActor::ResetMember);
	mainCharacter->GetUsingRightArm()->OnPress().AddUniqueDynamic(this, &AOffRoadRollerdrivesActor::ResetMember);
	mainCharacter->GetUsingRightArm()->ResetMember();
	mainCharacter->OnScope().AddUniqueDynamic(this, &AOffRoadRollerdrivesActor::SwitchSprintState);


	GetWorld()->GetTimerManager().ClearTimer(reloadingTimer);
	oldStamina = currentStamina;
	maxDecreaseTime = (currentStamina / decreaseValue) * decreaseTime;
	GetWorld()->GetTimerManager().SetTimer(speedTimer, this, &AOffRoadRollerdrivesActor::FinishDecrease,
	                                       maxDecreaseTime, false);
	isReloading = false;
	isRunning = true;
	SetActorTickEnabled(true);
}

void AOffRoadRollerdrivesActor::FinishDecrease()
{
	currentStamina = 0;
	SetSpeedMode(RUN);
	StartCooldown();
	onUseStamina.Broadcast(currentStamina, maxStamina);
	//SetActorTickEnabled(false);
}


void AOffRoadRollerdrivesActor::ReleaseBehavior()
{
	isPressed = false;
	if (canShot && isActivated)
		StartReloading();
}

void AOffRoadRollerdrivesActor::StartReloading()
{
	GetWorld()->GetTimerManager().ClearTimer(speedTimer);
	if (!isReloading)
	{
		if (isPressed)
		{
			mainCharacter->GetUsingLeftArm()->OnPress().RemoveDynamic(this, &AOffRoadRollerdrivesActor::ResetMember);
			mainCharacter->GetUsingCalf()->OnPress().RemoveDynamic(this, &AOffRoadRollerdrivesActor::ResetMember);
			mainCharacter->GetUsingRightArm()->OnPress().RemoveDynamic(this, &AOffRoadRollerdrivesActor::ResetMember);
			mainCharacter->OnScope().RemoveDynamic(this, &AOffRoadRollerdrivesActor::SwitchSprintState);
		}

		GetWorld()->GetTimerManager().ClearTimer(accelerationTimer);
		mainCharacter->GetCustomMovementComponent()->NetResetMaxacceleration();

		oldStamina = currentStamina;
		maxReloadingTime = (((maxStamina - currentStamina) / reloadingValue) * reloadingTime) * (1.0f - ((1.0f -
			cooldownBonus) + (1.0f - cooldownMultiplier)));
		GetWorld()->GetTimerManager().SetTimer(reloadingTimer, this, &AOffRoadRollerdrivesActor::FinishReloading,
		                                       maxReloadingTime, false);
		isRunning = false;
		isReloading = true;
		SetActorTickEnabled(true);
	}
}

void AOffRoadRollerdrivesActor::FinishReloading()
{
	if (!isPressed)
		isReloading = false;
	currentStamina = maxStamina;
	onUseStamina.Broadcast(currentStamina, maxStamina);
	onEndStaminaReload.Broadcast();
	//SetActorTickEnabled(false);
}


void AOffRoadRollerdrivesActor::SetSpeedMode(ESpeedMode _speedMode)
{
	if (HasAuthority())
	{
		mainCharacter->SetSpeedMode(_speedMode);
	}
	else
	{
		mainCharacter->ServerRpc_SetClientSpeedMode(_speedMode);
	}
}

void AOffRoadRollerdrivesActor::OnLanded(const FHitResult& _result)
{
	mainCharacter->LandedDelegate.RemoveDynamic(this, &AOffRoadRollerdrivesActor::OnLanded);

	if (!isPressed || !isActivated) return;

	mainCharacter->GetCustomMovementComponent()->NetSetMaxAcceleration(startAcceleration);
	GetWorld()->GetTimerManager().SetTimer(accelerationTimer, FTimerDelegate::CreateLambda([&]()
	{
		mainCharacter->GetCustomMovementComponent()->NetSetMaxAcceleration(defaultAcceleration);
	}), startAccelerationTime, FTimerManagerTimerParameters());
	UseStamina();
}


void AOffRoadRollerdrivesActor::StartReloadingTimerAfterModification(const float _oldMaxReloadingTime)
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(reloadingTimer))
	{
		const float _waitedCooldown = _oldMaxReloadingTime - _tm.GetTimerRemaining(reloadingTimer);
		_tm.ClearTimer(reloadingTimer);

		if (_waitedCooldown >= maxReloadingTime)
			FinishReloading();
		else
			_tm.SetTimer(reloadingTimer, this, &AOffRoadRollerdrivesActor::FinishReloading,
			             maxReloadingTime - _waitedCooldown);
	}
}

void AOffRoadRollerdrivesActor::SwitchSprintState(const bool _status)
{
	if (_status)
	{
		ResetMember();

	}
}


void AOffRoadRollerdrivesActor::StartCooldown()
{
	Super::StartCooldown();
	//mainCharacter->ReactivateMembers({this});
	isPressed = false;
	isRunning = false;
}

void AOffRoadRollerdrivesActor::EndCooldown()
{
	StartReloading();
	Super::EndCooldown();
}

void AOffRoadRollerdrivesActor::ModifyCooldownMultiplier(const float _cooldownReducPercent)
{
	const float _keptPercent = (100.0f - _cooldownReducPercent);
	const float _newCooldownMultiplier = _keptPercent / 100.0f;

	if (cooldownMultiplier == _newCooldownMultiplier)
		return;

	const float _oldMaxReloadingTime = maxReloadingTime;
	cooldownMultiplier = _newCooldownMultiplier;
	maxReloadingTime = (((maxStamina - currentStamina) / reloadingValue) * reloadingTime) * (1.0f - ((1.0f -
		cooldownBonus) + (1.0f - cooldownMultiplier)));

	StartReloadingTimerAfterModification(_oldMaxReloadingTime);
}

void AOffRoadRollerdrivesActor::ModifyCooldownBonus(const float _cooldownReducPercent)
{
	const float _keptPercent = (100.0f - _cooldownReducPercent);
	const float _newCooldownBonus = _keptPercent / 100.0f;

	if (cooldownBonus == _newCooldownBonus)
		return;

	const float _oldMaxReloadingTime = maxReloadingTime;
	cooldownBonus = _newCooldownBonus;
	maxReloadingTime = (((maxStamina - currentStamina) / reloadingValue) * reloadingTime) * (1.0f - ((1.0f -
		cooldownBonus) + (1.0f - cooldownMultiplier)));

	StartReloadingTimerAfterModification(_oldMaxReloadingTime);
}


void AOffRoadRollerdrivesActor::ResetMember()
{
	if (isPressed)
	{
		//mainCharacter->ReactivateMembers({this});
		StartReloading();
		SetSpeedMode(ESpeedMode::RUN);
	}
	Super::ResetMember();
}

bool AOffRoadRollerdrivesActor::CanUse()
{
	return Super::CanUse();
}
