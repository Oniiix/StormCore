#include "ProjectileActor.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"

AProjectileActor::AProjectileActor()
{
	useVelocityDebug = false;

	damage = 5.0f;
	useCharge = false;

#pragma region No charge
	initialZVelocityBoost = 500.0f;
	initialSpeed = 2000.0f;

#pragma region Resistance
	distanceSwitch = 400.0f;
	initialResistance = 0.0f;
	finalResistance = 0.5f;
#pragma endregion

#pragma region Gravity
	initialGravityScale = 0.0f;
	finalGravityScale = 2.0f;
#pragma endregion

#pragma endregion

#pragma region Charge
	minInitialZVelocityBoost = 250.0f;
	maxInitialZVelocityBoost = 500.0f;

	minInitialSpeed = 1500.0f;
	maxInitialSpeed = 2000.0f;

#pragma region Resistance
	minDistanceSwitch = 300.0f;
	maxDistanceSwitch = 200.0f;

	minInitialResistance = 0.0f;
	maxInitialResistance = 0.0f;

	minFinalResistance = 0.4f;
	maxFinalResistance = 0.5f;
#pragma endregion

#pragma region Gravity
	minInitialGravityScale = 0.0f;
	maxInitialGravityScale = 0.0f;

	minFinalGravityScale = 1.5f;
	maxFinalGravityScale = 2.0f;
#pragma endregion

#pragma endregion


	mainCharacter = nullptr;


#pragma region Member
	chargeTime = 0.0f;
	maxChargeTime = 0.0f;
#pragma endregion

	canMove = true;
	currentDistanceSwitch = 0.0f;
	currentResistance = 0.0f;
	currentGravityScale = 0.0f;

	bonusDamage = 1.0f;
	bleedRate = 0.0f;
	bleedBonusParams = FStatusParams(EStatusType::BLEED, 0.0f, 0.0f);

	currentVelocity = FVector();
	startLocation = FVector();

	bReplicates = true;
	SetReplicateMovement(true);
}

void AProjectileActor::AddInitialZVelocityBoost(const float _chargeTime, const float _maxChargeTime)
{
	const FVector _forward = GetActorForwardVector();
	const float _angleCoefficient = 1.0f - FMath::Abs(_forward.Z);
	const float _initialZVelocityBoost = useCharge ? GetValueFromCharge(_chargeTime, _maxChargeTime, minInitialZVelocityBoost, maxInitialZVelocityBoost) : initialZVelocityBoost;
	const float _boost = _initialZVelocityBoost * _angleCoefficient;
	currentVelocity = currentVelocity + FVector(0.0f, 0.0f, _boost);
}

void AProjectileActor::UpdateParametersByDistance()
{
	const float _currentDistance = FVector::Distance(startLocation, GetActorLocation());

	if (_currentDistance >= currentDistanceSwitch)
	{
		if (useCharge)
		{
			currentResistance = GetValueFromCharge(chargeTime, maxChargeTime, minFinalResistance, maxFinalResistance);
			currentGravityScale = GetValueFromCharge(chargeTime, maxChargeTime, minFinalGravityScale, maxFinalGravityScale);
		}
		else
		{
			currentResistance = finalResistance;
			currentGravityScale = finalGravityScale;
		}
	}
}

void AProjectileActor::UpdateCurrentVelocity(const float _deltaTime)
{
	currentVelocity = currentVelocity + (FVector(-currentResistance, -currentResistance, -980 * currentGravityScale) * _deltaTime);
}

void AProjectileActor::Move(const float _deltaTime)
{
	if (!canMove)
		return;

	AddActorWorldOffset(currentVelocity * _deltaTime, true, &moveResult);

	if (moveResult.GetActor())
		StopIfHit();
}

float AProjectileActor::GetValueFromCharge(const float _chargeTime, const float _maxChargeTime, const float _minValue, const float _maxValue) const
{
	return _chargeTime <= 0.0f ? _minValue : ((_chargeTime / _maxChargeTime) * (_maxValue - _minValue)) + _minValue;
}

void AProjectileActor::AddBleed(AActor* _target)
{
	if (_target)
	{
		if (bleedBonusParams.value > 0.0f && bleedBonusParams.duration > 0.0f && bleedRate > 0.0f)
		{
			UStatusComponent* _statusComp = _target->GetComponentByClass<UStatusComponent>();

			if (_statusComp)
			{
				UBleedStatus* _bleed = Cast<UBleedStatus>(_statusComp->AddStatus(mainCharacter, bleedBonusParams, true));

				if (_bleed)
					_bleed->SetBleedRate(bleedRate);
			}
		}
	}
}

void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateParametersByDistance();
	UpdateCurrentVelocity(DeltaTime);
	Move(DeltaTime);
}

//void AProjectileActor::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//	canMove = false;
//	SetLifeSpan(0.1f);
//}

void AProjectileActor::StopIfHit()
{
	canMove = false;
	SetLifeSpan(0.1f);
}

void AProjectileActor::InitProjectile(AMainCharacter* _mainCharacter, const float _chargeTime, const float _maxChargeTime, const FVector _charaVelocity)
{
	mainCharacter = _mainCharacter;

	if (useCharge)
	{
		chargeTime = _chargeTime;
		maxChargeTime = _maxChargeTime;

		currentDistanceSwitch = GetValueFromCharge(_chargeTime, _maxChargeTime, minDistanceSwitch, maxDistanceSwitch);
		currentResistance = GetValueFromCharge(_chargeTime, _maxChargeTime, minInitialResistance, maxInitialResistance);
		currentGravityScale = GetValueFromCharge(_chargeTime, _maxChargeTime, minInitialGravityScale, maxInitialGravityScale);
		currentVelocity = GetValueFromCharge(_chargeTime, _maxChargeTime, minInitialSpeed, maxInitialSpeed) * GetActorForwardVector() + _charaVelocity;
	}
	else
	{
		currentDistanceSwitch = distanceSwitch;
		currentResistance = initialResistance;
		currentGravityScale = initialGravityScale;
		currentVelocity = initialSpeed * GetActorForwardVector() + _charaVelocity;
		LOG_COLOR_TIME(useVelocityDebug, "Velocity : [X] " + FString::SanitizeFloat(currentVelocity.X) + " / [Y] " + FString::SanitizeFloat(currentVelocity.Y), FColor::Blue, 8.0f);
	}

	startLocation = GetActorLocation();
	AddInitialZVelocityBoost(_chargeTime, _maxChargeTime);
}