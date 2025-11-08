// Objectif 3D Copyright


#include "CadenceShotgunActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/ATH/Reticle/RightArm/Shotgun/ShotgunReticle.h"


void ACadenceShotgunActor::PressBehavior()
{
	Super::PressBehavior();

	const float _ratio = GetReductionRatio();
	if (_ratio > rhythmMeasureMin / 100 && _ratio < rhythmMeasureMax / 100)
	{
		Shoot();
		onUseRightArm.Broadcast(true, FMath::RandRange(0, 2));
		SetStacks(stacks + 1);

		if (statusComp)
			statusComp->AddStatus(this, cooldownModifierStatus);

		LOG_COLOR(CADENCE_SHOTGUN_USE_LOG, "++ BONUS", GREEN);
		GetWorldTimerManager().SetTimer(expirationTimer, this, &ACadenceShotgunActor::ExpirationFinish, expirationTime);
	}
	else
	{
		LOG_COLOR(CADENCE_SHOTGUN_USE_LOG, "Failed Cooldown Start", RED);
		canShot = false;
		SetActorTickEnabled(false);

		onFailed.Broadcast();
		if (HasAuthority())
			ClientRpc_CallonFailedEvent();
		else
			ServerRpc_CallonFailedEvent();


		FTimerHandle _failedTimer;
		FTimerDelegate _failedDelegate;
		_failedDelegate.BindLambda([&]() { 
			canShot = true; 
			SetActorTickEnabled(true);
			LOG_COLOR(CADENCE_SHOTGUN_USE_LOG, "Failed Cooldown Finish", ORANGE);
			});

		GetWorldTimerManager().SetTimer(_failedTimer, _failedDelegate, failedCooldown, FTimerManagerTimerParameters());
	}
	currentTime = 0;
	onReductionUpdate.Broadcast(GetReductionRatio());
}

void ACadenceShotgunActor::InitMember()
{
	Super::InitMember();
	gameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	camera = mainCharacter->GetCameraComponent()->GetCamera();
	statusComp = mainCharacter->GetComponentByClass<UStatusComponent>();
}

void ACadenceShotgunActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (!mainCharacter || !mainCharacter->GetIsOwner())
		return;

	LOG_COLOR_TICK(CADENCE_SHOTGUN_USE_LOG, "expirationTime => " + FString::SanitizeFloat(GetWorldTimerManager().GetTimerElapsed(expirationTimer)), BLACK);
	currentTime += _deltaTime;

	const float _ratio = GetReductionRatio();
	if (_ratio > rhythmMeasureMin / 100 && _ratio < rhythmMeasureMax / 100)
	{
		LOG_COLOR_TICK(CADENCE_SHOTGUN_USE_LOG, "CurrentTime => " + FString::SanitizeFloat(currentTime), YELLOW);
	}
	else
	{
		LOG_COLOR_TICK(CADENCE_SHOTGUN_USE_LOG, "CurrentTime => " + FString::SanitizeFloat(currentTime), BLUE);
	}
	
	if (currentTime >= reductionDuration)
	{
		currentTime = 0;
		SetStacks(stacks > 0 ? stacks - 1 : 0);

		if (statusComp)
		{
			if (stacks >= 0)
				statusComp->RefreshStatus(this, cooldownModifierStatus, true);
			else
				statusComp->RemoveStatus(this, cooldownModifierStatus.type);
		}

		LOG_COLOR(CADENCE_SHOTGUN_USE_LOG, "Reduction finish => Restart", PINK);
	}
	onReductionUpdate.Broadcast(GetReductionRatio());
	DrawDebug();
}

void ACadenceShotgunActor::ExpirationFinish()
{
	if (statusComp)
	{
		for (int i = 0; i < stacks; i++)
			statusComp->RefreshStatus(this, cooldownModifierStatus, true);
	}
	SetStacks(0);
}

void ACadenceShotgunActor::Shoot()
{
	const FVector& _circlePosition = camera->GetComponentLocation() + camera->GetForwardVector() * shootRange;

	float _randRadius = 0.0f;
	int _randAngle = 0;
	FVector _shootLocation;
	FHitResult _result;

	TMap<ULifeComponent*, int> _damages;

	CallOnShoot(memberMesh->GetSocketLocation("Shoot_Loc"), (_circlePosition - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());

	for (int i = 0; i < nbOfShot; i++)
	{
		_randRadius = RANDOM_FLOAT_RANGE(0.0f, shootCircleRadius);
		_randAngle = RANDOM_INT_RANGE(0, 360);
		_shootLocation = _circlePosition + UCustomMathBlueprintLibrary::GetLocalTrigoPointZY(_randAngle, _randRadius, camera->GetUpVector(), camera->GetRightVector());

		if (UKismetSystemLibrary::LineTraceSingleForObjects(this, camera->GetComponentLocation(), _shootLocation, shootLayers, false, { mainCharacter }, EDrawDebugTrace::ForDuration, _result, true, RED, GREEN, 2))
		{
			if (ULifeComponent* _life = _result.GetActor()->GetComponentByClass<ULifeComponent>())
			{
				CallOnHit(_result);
				if (_damages.Contains(_life))
					_damages[_life] = _damages[_life] + damage;
				else
					_damages.Add(_life, damage);
			}
		}
	}

	for (TPair<ULifeComponent*, int> _pair : _damages)
	{
		_pair.Key->Damage(_pair.Value * bonusDamage, mainCharacter);
		AddBleed(_pair.Key->GetOwner());
	}
}

void ACadenceShotgunActor::DrawDebug()
{
	if (!camera) return;
	DRAW_CIRCLE(camera->GetComponentLocation() + camera->GetForwardVector() * shootRange, shootCircleRadius, FColor::Orange, camera->GetUpVector(), camera->GetRightVector());
}


#if WITH_EDITOR
void ACadenceShotgunActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ACadenceShotgunActor, rhythmMeasureMin))
	{
		rhythmMeasureMin = CLAMP(rhythmMeasureMin, 0, rhythmMeasureMax);
		bonusTime = reductionDuration * (rhythmMeasureMax / 100 - rhythmMeasureMin / 100);
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ACadenceShotgunActor, rhythmMeasureMax))
	{
		rhythmMeasureMax = CLAMP(rhythmMeasureMax, rhythmMeasureMin, 100);
		bonusTime = reductionDuration * (rhythmMeasureMax / 100 - rhythmMeasureMin / 100);
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ACadenceShotgunActor, reductionDuration))
		bonusTime = reductionDuration * (rhythmMeasureMax / 100 - rhythmMeasureMin / 100);
}
#endif





void ACadenceShotgunActor::ServerRpc_CallonFailedEvent_Implementation()
{
	onFailed.Broadcast();
}

bool ACadenceShotgunActor::ClientRpc_CallonFailedEvent_Validate()
{
	return !(HasAuthority());
}
void ACadenceShotgunActor::ClientRpc_CallonFailedEvent_Implementation()
{
	onFailed.Broadcast();
}