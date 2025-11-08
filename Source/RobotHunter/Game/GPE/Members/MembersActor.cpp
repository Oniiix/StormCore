#include "MembersActor.h"
#include <RobotHunter/CCC/Character/MainCharacter.h>
#include <Net/UnrealNetwork.h>
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"


AMembersActor::AMembersActor()
{
	useCharge = false;
	maxChargeTime = 1.5f;

	useCooldown = false;
	cooldown = 1.0f;
	uses = 2;

	cameraKey = FString();

	hasDeactivatedStatus = false;
	canShot = true;
	currentChargeTime = 0.0f;
	cooldownMultiplier = 1.0f;
	cooldownBonus = 1.0f;
	currentUses = 2;

	bonusDamage = 1.0f;

	bleedRate = 0.0f;
	bleedBonusParams = FStatusParams(EStatusType::BLEED, 0.0f, 0.0f);

	chargeTimer = FTimerHandle();
	cooldownTimer = FTimerHandle();
}

void AMembersActor::SetIsActivated(const bool& _status)
{
	const int _new = _status ? activationToken - 1 : activationToken + 1;
	activationToken = _new < 0 ? 0 : _new;
	LOG_COLOR(false, GetName() + " => " + FString::FromInt(activationToken), ORANGE);
	isActivated = activationToken == 0;
}

FString AMembersActor::GetInputMappingName(const bool _getGamepad) const
{
	if (!inputContext)
		return "None";

	static const FString GamepadStr = TEXT("gamepad");
	static const FString KeyboardStr = TEXT("keyboard");

	for (const FEnhancedActionKeyMapping& Mapping : inputContext->GetMappings())
	{
		if (Mapping.Action != inputAction)
			continue;

		const UPlayerMappableKeySettings* Settings = Mapping.GetPlayerMappableKeySettings();
		if (!Settings)
			continue;

		const FString MappingNameStr = Settings->GetMappingName().ToString();
		const FString LowerName = MappingNameStr.ToLower();

		if (_getGamepad)
		{
			if (LowerName.Contains(GamepadStr))
				return MappingNameStr;
		}
		else
		{
			if (LowerName.Contains(KeyboardStr))
				return MappingNameStr;
		}
	}
	return "None";
}


void AMembersActor::UpdateCurrentCharge()
{
	currentChargeTime += 0.1f;

	if (currentChargeTime >= maxChargeTime)
	{
		currentChargeTime = maxChargeTime;

		if (GetWorld()->GetTimerManager().IsTimerActive(chargeTimer))
			GetWorld()->GetTimerManager().ClearTimer(chargeTimer);
	}
}

void AMembersActor::EndCooldown()
{
	currentUses += 1;
	canShot = true;
	onCooldownEnd.Broadcast();
	/*if (GetWorld()->GetTimerManager().IsTimerActive(cooldownTimer))
		GetWorld()->GetTimerManager().ClearTimer(cooldownTimer);*/
}

void AMembersActor::StartCooldownAfterModification(const float _oldCooldown)
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(cooldownTimer))
	{
		const float _waitedCooldown = _oldCooldown - _tm.GetTimerRemaining(cooldownTimer);
		const float _newCooldown = cooldown * (1.0f - ((1.0f - cooldownBonus) + (1.0f - cooldownMultiplier)));
		_tm.ClearTimer(cooldownTimer);

		if (_waitedCooldown >= _newCooldown)
			EndCooldown();
		else
		{
			_tm.SetTimer(cooldownTimer, this, &AMembersActor::EndCooldown, _newCooldown - _waitedCooldown);
			onCooldownStart.Broadcast(_newCooldown);
		}
	}
}

void AMembersActor::StartCooldown()
{
	if (useCooldown && !hasDeactivatedStatus && isActivated)
	{
		currentUses -= 1;

		if (currentUses == 0)
			canShot = false;

		const float _cooldown = cooldown * (1.0f - ((1.0f - cooldownBonus) + (1.0f - cooldownMultiplier)));
		GetWorld()->GetTimerManager().SetTimer(cooldownTimer, this, &AMembersActor::EndCooldown, _cooldown);
		onCooldownStart.Broadcast(_cooldown);
	}
}

void AMembersActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMembersActor, mainCharacter);
}

void AMembersActor::BeginPlay()
{
	Super::BeginPlay();
	currentUses = uses;
}

void AMembersActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMembersActor::AddBleed(AActor* _actor)
{
	if (_actor)
	{
		if (bleedBonusParams.value > 0.0f && bleedBonusParams.duration > 0.0f && bleedRate > 0.0f)
		{
			UStatusComponent* _statusComp = _actor->GetComponentByClass<UStatusComponent>();

			if (_statusComp)
			{
				UBleedStatus* _bleed = Cast<UBleedStatus>(_statusComp->AddStatus(GetOwner(), bleedBonusParams, true));

				if (_bleed)
					_bleed->SetBleedRate(bleedRate);
			}
		}
	}
}


void AMembersActor::PressBehavior()
{
	isPressed = true;
	onPress.Broadcast();
	if (useCharge && !GetWorld()->GetTimerManager().IsTimerActive(chargeTimer))
		GetWorld()->GetTimerManager().SetTimer(chargeTimer, this, &AMembersActor::UpdateCurrentCharge, 0.1f, true,
		                                       0.1f);
}

void AMembersActor::ReleaseBehavior()
{
	isPressed = false;
	onRelease.Broadcast();

	//TODO MemberActor : verif if the code below is useful
	if (!canShot || hasDeactivatedStatus)
		return;

	if (useCharge)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(chargeTimer))
			GetWorld()->GetTimerManager().ClearTimer(chargeTimer);

		currentChargeTime = 0.0f;
	}

	StartCooldown();
}

bool AMembersActor::CanUse()
{
	return canShot && !hasDeactivatedStatus && isActivated && !mainCharacter->GetInteractionComponent()->
	                                                                          GetHasInteracted();
}

void AMembersActor::ResetMember()
{
	isPressed = false;
	onReset.Broadcast();
}

void AMembersActor::CallOnHit(const FHitResult& _result)
{
	onHit.Broadcast(_result);
	if (HasAuthority())
		ClientRpc_CallOnHitEvent(_result);
	else
		ServerRpc_CallOnHitEvent(_result);
}

void AMembersActor::CallOnShoot(const FVector& _startPosition, const FVector& _direction)
{
	onShoot.Broadcast(_startPosition, _direction);
	if (HasAuthority())
		ClientRpc_CallOnShootEvent(_startPosition, _direction);
	else
		ServerRpc_CallOnShootEvent(_startPosition, _direction);
}

void AMembersActor::ModifyCooldownMultiplier(const float _cooldownReducPercent)
{
	const float _keptPercent = (100.0f - _cooldownReducPercent);
	const float _newCooldownMultiplier = _keptPercent / 100.0f;

	if (cooldownMultiplier == _newCooldownMultiplier)
		return;

	const float _oldCooldown = cooldown * (1.0f - ((1.0f - cooldownBonus) + (1.0f - cooldownMultiplier)));
	cooldownMultiplier = _newCooldownMultiplier;

	StartCooldownAfterModification(_oldCooldown);
}

void AMembersActor::ModifyCooldownBonus(const float _cooldownReducPercent)
{
	const float _keptPercent = (100.0f - _cooldownReducPercent);
	const float _newCooldownBonus = _keptPercent / 100.0f;

	if (cooldownBonus == _newCooldownBonus)
		return;

	const float _oldCooldown = cooldown * (1.0f - ((1.0f - cooldownBonus) + (1.0f - cooldownMultiplier)));
	cooldownBonus = _newCooldownBonus;

	StartCooldownAfterModification(_oldCooldown);
}

void AMembersActor::SetHasDeactivatedStatus(const bool _hasStatus)
{
	hasDeactivatedStatus = _hasStatus;
	onDeactivatedStatus.Broadcast(hasDeactivatedStatus);
}

void AMembersActor::SetMainCharacter(AMainCharacter* _chara)
{
	mainCharacter = _chara;
	SetOwner(_chara);
}

void AMembersActor::ServerRpc_CallOnHitEvent_Implementation(const FHitResult& _result)
{
	onHit.Broadcast(_result);
}

bool AMembersActor::ClientRpc_CallOnHitEvent_Validate(const FHitResult& _result)
{
	return !(HasAuthority());
}

void AMembersActor::ClientRpc_CallOnHitEvent_Implementation(const FHitResult& _result)
{
	onHit.Broadcast(_result);
}


void AMembersActor::ServerRpc_CallOnShootEvent_Implementation(const FVector& _startPosition, const FVector& _direction)
{
	onShoot.Broadcast(_startPosition, _direction);
}

bool AMembersActor::ClientRpc_CallOnShootEvent_Validate(const FVector& _startPosition, const FVector& _direction)
{
	return !(HasAuthority());
}

void AMembersActor::ClientRpc_CallOnShootEvent_Implementation(const FVector& _startPosition, const FVector& _direction)
{
	onShoot.Broadcast(_startPosition, _direction);
}
