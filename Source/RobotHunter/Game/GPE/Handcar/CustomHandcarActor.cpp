#include "CustomHandcarActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Game/Interactable/Handcar/StopHandcarActor.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../Interactable/Lever/LeverActor.h"
#include "../Transmitter/TargetActor/RailsTargetActor.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "RobotHunter/Utils/FileLogger.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/CapsuleComponent.h"
#include "BumpBoxComponent.h"


ACustomHandcarActor::ACustomHandcarActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	mesh->SetupAttachment(RootComponent);

#pragma region Data Asset
	useDataAsset = true;
	dataAsset = nullptr;
#pragma endregion


#pragma region Debug
	frictionDebug = false;
	brakeDebug = false;
	slopeAccelerationDebug = false;
	slopeAngleDebug = false;
	//pumpAccelerationDebug = false;
	nitroAccelerationDebug = false;
	//frictionSlopeAccelerationDebug = false;
	totalAccelerationDebug = false;
	speedDebug = false;
#pragma endregion


#pragma region Detection
	/*detectionRange = 1000.0f;*/

	hitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	hitBox->SetupAttachment(mesh);
	hitBox->SetGenerateOverlapEvents(true);

	/*detectedRails = TArray<ARailActor*>(); */
#pragma endregion


#pragma region Movement
	currentRail = nullptr;


	rotationSpeed = 25.0f;


	canMove = true;
	isBlocked = false;
	isOnReturn = false;


	currentDistance = 0.0f;
#pragma endregion


#pragma region Handle
	//handle = nullptr;
#pragma endregion


#pragma region Brake
	brake = nullptr;

	brakeValue = 0.0f;
#pragma endregion


#pragma region Nitro
	nitro = nullptr;
#pragma endregion


#pragma region LifeStock
	lifeStocks = TArray<ALifeStockHandcarActor*>();

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	AddOwnedComponent(lifeComponent);
#pragma endregion


#pragma region EggStock

#pragma endregion


#pragma region Speed
	maxSpeed = 2.5f;


#pragma region Friction
	//coefficientFriction = 0.15f;
	frictionValue = -1.0f;
	//frictionChangeAngle = 5.0f;
	frictionSpeedPercent = 0.1f;
#pragma endregion


	currentSpeed = 0.0f;
	maxRotSpeed = 0.0f;
#pragma endregion


#pragma region Acceleration
	/*pumpMinForce = 500.0f;
	pumpMaxForce = 1000.0f;*/


	//accelerationBoost = 1.5f;
	slopeCurve = nullptr;


	currentAcceleration = 0.0f;
	//frictionSlopeAcceleration = 0.0f;
	slopeAcceleration = 0.0f;
	//pumpAcceleration = 0.0f;


	/*firstPlayerAccelerates = false;
	secondPlayerAccelerates = false;*/
#pragma endregion


#pragma region Stop
	stopTime = 30.0f;
	callingStops = TMap<FString, AStopHandcarActor*>();
	nextStop = nullptr;
	currentStop = nullptr;

	inAutoMode = false;
	isAtStop = false;
	isStopped = false;

	stopTimer = FTimerHandle();
#pragma endregion


	currentTargets = TArray<ARailsTargetActor*>();

	networkComp = nullptr;

	/*bReplicates = true;
	SetReplicateMovement(true);*/
}

void ACustomHandcarActor::SetBumpBoxArray(const TArray<UBoxComponent*>& _boxArray)
{
	bumpBoxs = _boxArray;
	for (UBoxComponent* _box : bumpBoxs)
	{
		_box->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACustomHandcarActor::OnBumpOverlap);
		_box->SetGenerateOverlapEvents(false);
	}
}

//void ACustomHandcarActor::UpdateDetectionSphere()
//{
//	detectionSphere->SetSphereRadius(detectionRange);
//}

void ACustomHandcarActor::LogDebug()
{
	//LOG_COLOR_TIME(frictionDebug, "[Friction] => " + FString::SanitizeFloat(coefficientFriction), FColor::Red, 0.1f);
	LOG_COLOR_TICK(frictionDebug, "[Friction] => " + FString::SanitizeFloat(currentSpeed >= 0.0f ? frictionValue - (currentSpeed * frictionSpeedPercent)
		: -frictionValue - (currentSpeed * frictionSpeedPercent)), FColor::Red);

	LOG_COLOR_TICK(brakeDebug, "[Brake] => " + FString::SanitizeFloat(brakeValue), FColor::Red);
	LOG_COLOR_TICK(slopeAccelerationDebug, "[SlopeAcceleration] => " + FString::SanitizeFloat(slopeAcceleration), FColor::Magenta);
	LOG_COLOR_TICK(slopeAngleDebug, "[SlopeAngle] => " + FString::SanitizeFloat(UCustomMathBlueprintLibrary::RoundToNumberAfterPoint(GetActorRotation().Pitch, 2)), FColor::Magenta);
	//LOG_COLOR_TICK(pumpAccelerationDebug, "[PumpAcceleration] => " + FString::SanitizeFloat(pumpAcceleration), FColor::Green);

	LOG_COLOR_TICK(nitroAccelerationDebug, "[NitroAcceleration] => " + (nitro ? FString::SanitizeFloat(nitro->GetUpdatedNitroAcceleration())
		: "No nitro found"), FColor::Green);

	//LOG_COLOR_TIME(frictionSlopeAccelerationDebug, "[FrictionSlopeAcceleration] => " + FString::SanitizeFloat(frictionSlopeAcceleration), FColor::Red);
	LOG_COLOR_TICK(totalAccelerationDebug, "[Acceleration] => " + FString::SanitizeFloat(currentAcceleration), FColor::Blue);
	LOG_COLOR_TICK(speedDebug, "[Speed] => " + FString::SanitizeFloat(currentSpeed / 10.0f), FColor::Blue);
}

void ACustomHandcarActor::GetDataAssetValues()
{
	if (dataAsset)
	{
		if (useDataAsset)
		{
			frictionDebug = dataAsset->GetDebug(EHandcarDebugEnum::Friction);
			brakeDebug = dataAsset->GetDebug(EHandcarDebugEnum::Brake);
			slopeAccelerationDebug = dataAsset->GetDebug(EHandcarDebugEnum::SlopeAcceleration);
			slopeAngleDebug = dataAsset->GetDebug(EHandcarDebugEnum::SlopeAngle);
			nitroAccelerationDebug = dataAsset->GetDebug(EHandcarDebugEnum::Nitro);
			//pumpAccelerationDebug = dataAsset->GetDebug(EHandcarDebugEnum::Pump);
			totalAccelerationDebug = dataAsset->GetDebug(EHandcarDebugEnum::TotalAcceleration);
			speedDebug = dataAsset->GetDebug(EHandcarDebugEnum::Speed);

			rotationSpeed = dataAsset->GetHandcarRotationSpeed();
			maxSpeed = dataAsset->GetHandcarMaxSpeed();

			frictionValue = dataAsset->GetFrictionValue();
			//frictionChangeAngle = dataAsset->GetFrictionChangeAngle();
			frictionSpeedPercent = dataAsset->GetFrictionSpeedPercent();

			/*pumpMinForce = dataAsset->GetPumpForce(true);
			pumpMaxForce = dataAsset->GetPumpForce(false);*/

			//accelerationBoost = dataAsset->GetAccelerationBoost();

			/*if (handle)
				handle->UpdateValuesFromDataAsset(dataAsset);*/

			if (brake)
				brake->UpdateValuesFromDataAsset(dataAsset);

			if (nitro)
				nitro->UpdateValuesFromDataAsset(dataAsset);

			const int _lifeStockNum = lifeStocks.Num();

			for (int i = 0; i < _lifeStockNum; i++)
				if (lifeStocks[i])
					lifeStocks[i]->UpdateValuesFromDataAsset(dataAsset);
		}
	}
}

void ACustomHandcarActor::CheckIfStartAutoMode()
{
	if (!inAutoMode)
	{
		if (isStopped)
		{
			if (nextStop)
			{
				canMove = false;
				SetInAutoMode(true);
				FTimerManager& _tm = GetWorld()->GetTimerManager();

				if (!_tm.IsTimerActive(stopTimer))
					_tm.SetTimer(stopTimer, this, &ACustomHandcarActor::AutoMode, stopTime, false);
			}
		}
	}
}

void ACustomHandcarActor::AutoMode()
{
	if (inAutoMode)
	{
		if (currentStop)
		{
			if (nitro)
				nitro->SetUseNegativeNitro(currentStop->GetNextRailWay(nextStop) == ERailWay::NegativeRailWay ? true : false);

			if (brake)
			{
				brake->SetReleased(true);
				brake->InstantBrake(true);
			}

			canMove = true;
		}
	}
}

void ACustomHandcarActor::UnlockLifeAndEggStocks(const bool _unlock)
{
	int _num = lifeStocks.Num();
	ALifeStockHandcarActor* _lifeStock = nullptr;

	for (int i = 0; i < _num; i++)
	{
		_lifeStock = lifeStocks[i];

		if (_lifeStock)
		{
			if (_lifeStock->GetIsAtExterior())
				_lifeStock->SetCanBeDetected(_unlock);
		}
	}

	_num = eggsStock.Num();
	AEggStockHandcarActor* _eggStock = nullptr;

	for (int i = 0; i < _num; i++)
	{
		_eggStock = eggsStock[i];

		if (_eggStock)
		{
			if (_eggStock->GetIsAtExterior())
				_eggStock->SetCanBeDetected(_unlock);
		}
	}
}

void ACustomHandcarActor::PlayerOverlaps(AMainCharacter* _player)
{
	//if (_player)
	//{
	//	_player->SetIsUsingHandcar(_isOverlapping);
	//	_player->GetCustomMovementComponent()->SetOnHandcar(_isOverlapping);
	//	_player->SetCanJump(!_isOverlapping);

	//	if (_isOverlapping)
	//	{
	//		_player->AttachToComponent(handcarMesh, FAttachmentTransformRules::KeepWorldTransform);
	//		players.AddUnique(_player);
	//	}
	//	else
	//	{
	//		_player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//		players.RemoveSingle(_player);
	//	}
	//}

	if (_player)
	{
		_player->SetIsUsingHandcar(true);
		
		_player->GetCustomMovementComponent()->SetOnHandcar(true, this);
		_player->AttachToComponent(handcarMesh, FAttachmentTransformRules::KeepWorldTransform);
		players.AddUnique(_player);

		if (!isAtStop)
		{
			_player->SetCanJump(false);
			_player->DeactivateMembers({ _player->GetUsingLeftArm(), _player->GetUsingRightArm() });
		}
	}
}


void ACustomHandcarActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* _chara = Cast<AMainCharacter>(OtherActor))
		PlayerOverlaps(_chara);
}

void ACustomHandcarActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMainCharacter* _chara = Cast<AMainCharacter>(OtherActor))
	{
		_chara->SetIsUsingHandcar(false);

		_chara->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		_chara->GetCustomMovementComponent()->SetOnHandcar(false, nullptr);

		const FRotator _playerRot = _chara->GetActorRotation();
		_chara->SetActorRotation(FRotator(0.0f, _playerRot.Yaw, 0.0f));

		players.RemoveSingle(_chara);

		if (!isAtStop)
		{
			_chara->SetCanJump(true);
			_chara->ReactivateMembers({ _chara->GetUsingLeftArm(), _chara->GetUsingRightArm() });
		}
	}
}


#pragma region Detection
//void ACustomHandcarActor::RefreshDetectedRails()
//{
//	for (ARailActor* _rail : detectedRails)
//		CheckIfRailIsOnPath(_rail);
//}

//bool ACustomHandcarActor::CheckIfRailIsOnPath(ARailActor* _rail)
//{
//	if (!_rail)
//		return false;
//
//	if (_rail == currentRail)
//	{
//		_rail->GetOnPathRailDetected().Broadcast();
//		return true;
//	}
//
//	//ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
//	ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);
//
//	if (CheckIfTargetContainsRail(_nextTarget, _rail))
//		return true;
//
//	ARailActor* _nextRail = _nextTarget->GetNextRail(isOnReturn);
//	_nextTarget = _nextRail->GetNextTarget(isOnReturn);
//
//	if (!CheckIfTargetContainsRail(_nextTarget, _rail))
//	{
//		_rail->GetRailNotDetectedAnymore().Broadcast();
//		return false;
//	}
//
//	return true;
//}

//bool ACustomHandcarActor::CheckIfLeverContainsRail(ALeverActor* _lever, ARailActor* _rail)
//{
//	if (!_lever)
//		return false;
//
//	if (!isOnReturn)
//	{
//		if (_lever->GetDepartureRails().Contains(_rail))
//		{
//			if (_rail->GetIsBlocked(isOnReturn, true))
//				_rail->GetBlockedRailDetected().Broadcast();
//			else
//				_rail->GetOnPathRailDetected().Broadcast();
//
//			return true;
//		}
//	}
//
//	return false;
//}

//bool ACustomHandcarActor::CheckIfTargetContainsRail(ARailsTargetActor* _target, ARailActor* _rail)
//{
//	if (!_target)
//		return false;
//
//	if (!isOnReturn)
//	{
//		if (_target->GetDepartureRails().Contains(_rail))
//		{
//			if (_rail->GetIsBlocked(isOnReturn, true))
//				_rail->GetBlockedRailDetected().Broadcast();
//			else
//				_rail->GetOnPathRailDetected().Broadcast();
//
//			return true;
//		}
//	}
//
//	return false;
//}
#pragma endregion


#pragma region Movement
void ACustomHandcarActor::Move(const float _deltaTime)
{
	if (!currentRail || !canMove)
		return;

	const FVector _locationOnExit = currentRail->GetStopLocationOnExit(isOnReturn);
	const float _distanceOnExit = currentRail->GetDistanceAtLocation(_locationOnExit);
	const bool _checkIfBlocked = !isOnReturn && currentDistance >= _distanceOnExit || isOnReturn && currentDistance <= _distanceOnExit;

	if (_checkIfBlocked)
	{
		ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);
		ERailWay _intersectionWay = ERailWay::DefaultRailWay;

		if (_nextTarget)
			_intersectionWay = _nextTarget->GetIntersectionWay();

		const bool _isAtIntersection = isOnReturn && _intersectionWay == ERailWay::NegativeRailWay || !isOnReturn && _intersectionWay == ERailWay::PositiveRailWay;

		if (_intersectionWay == ERailWay::DefaultRailWay || _isAtIntersection)
		{
			if (currentRail->GetIsBlocked(isOnReturn, false))
			{
				isBlocked = true;
				currentSpeed = 0.0f;
				return;
			}
			else if (!_nextTarget)
			{
				isBlocked = true;
				currentSpeed = 0.0f;
				return;
			}
			else
			{
				ARailActor* _nextRail = inAutoMode ? _nextTarget->GetNextRail(isOnReturn, nextStop) : _nextTarget->GetNextRail(isOnReturn);

				if (_nextRail)
				{
					if (_nextRail->GetIsLocked() || _nextRail->GetIsBlocked(isOnReturn, true))
					{
						isBlocked = true;
						currentSpeed = 0.0f;
						return;
					}
				}
			}
		}
	}

	isBlocked = false;

	const FVector _currentTargetLoc = currentRail->GetLocationAtDistance(currentDistance);
	SetActorLocation(_currentTargetLoc);
	position = _currentTargetLoc;

	currentDistance += _deltaTime * currentSpeed * 10.0f;
	if (isOnReturn)
	{
		if (currentDistance <= 0.0f)
			UpdateCurrentRail();
	}
	else
	{
		if (currentDistance >= currentRail->GetLength())
			UpdateCurrentRail();
	}
}

void ACustomHandcarActor::Rotate(const float _deltaTime)
{
	/*if (!currentRail || isBlocked)
		return;

	const FVector _actorLoc = GetActorLocation();
	const float _distance = isOnReturn ? (currentDistance + (_deltaTime * FMath::Abs(currentSpeed) * 20.0f)) : currentDistance;

	if (_distance > currentRail->GetLength())
		return;

	const FVector _currentTargetLoc = currentRail->GetLocationAtDistance(_distance);
	const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_actorLoc, _currentTargetLoc);
	const FRotator _rotation = FMath::Lerp(GetActorRotation(), _lookAt, _deltaTime * rotationSpeed);
	SetActorRotation(_rotation);*/

	if (!currentRail || isBlocked || !canMove || currentSpeed == 0.0f)
		return;

	const FVector _actorLoc = GetActorLocation();
	const float _distanceToAdd = isOnReturn ? (_deltaTime * FMath::Abs(currentSpeed) * 20.0f) : (_deltaTime * FMath::Abs(currentSpeed) * 40.0f);
	float _distance = currentDistance + _distanceToAdd;
	ARailActor* _lookAtRail = currentRail;

	if (_distance > currentRail->GetLength())
	{
		//ALeverActor* _nextLever = currentRail->GetNextLever(false);
		ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(false);

		if (_nextTarget)
		{
			_lookAtRail = inAutoMode ? _nextTarget->GetNextRail(false, nextStop) : _nextTarget->GetNextRail(false);

			if (_lookAtRail)
				_distance = _lookAtRail->GetDistanceAtLocation(_actorLoc) + _distanceToAdd;
		}
	}
	else if (_distance < 0.0f)
	{
		ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(true);

		if (_nextTarget)
		{
			_lookAtRail = inAutoMode ? _nextTarget->GetNextRail(true, nextStop) : _nextTarget->GetNextRail(true);

			if (_lookAtRail)
				_distance = _lookAtRail->GetDistanceAtLocation(_actorLoc) + _distanceToAdd;
		}
	}

	if (_lookAtRail)
	{
		FRotator _actorRotation = /*FRotator(UCustomMathBlueprintLibrary::RoundToNumberAfterPoint(GetActorRotation().Pitch, 2),
			UCustomMathBlueprintLibrary::RoundToNumberAfterPoint(GetActorRotation().Yaw, 2),
			UCustomMathBlueprintLibrary::RoundToNumberAfterPoint(GetActorRotation().Roll, 2));*/ GetActorRotation();
		_actorRotation.Roll = 0.0f;

		const FVector _currentTargetLoc = _lookAtRail->GetLocationAtDistance(_distance);

		/*DRAW_SPHERE(_currentTargetLoc, 250, FColor::Blue);*/

		const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_actorLoc, _currentTargetLoc);
		float _rotationSpeed = _deltaTime * rotationSpeed;
		_rotationSpeed = _rotationSpeed > maxRotSpeed ? maxRotSpeed : _rotationSpeed;
		const FRotator _rotation = FMath::Lerp(_actorRotation, _lookAt, _rotationSpeed);
		SetActorRotation(_rotation);
	}
}


void ACustomHandcarActor::UpdateCurrentRail()
{
	//ARailActor* _currentRailMemory = currentRail;
	//ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
	ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);

	if (_nextTarget)
	{
		/*if (!_nextTarget->GetIsLinkedToHandcar())
		{
			_nextTarget->GetOnUpdateRailIsBlocked().AddDynamic(this, &ACustomHandcarActor::RefreshDetectedRails);
			_nextTarget->SetIsLinkedToHandcar(true);
		}*/

		ARailActor* _nextRail = inAutoMode ? _nextTarget->GetNextRail(isOnReturn, nextStop) : _nextTarget->GetNextRail(isOnReturn);

		if (_nextRail)
		{
			const ERailWay _intersectionWay = _nextTarget->GetIntersectionWay();
			const bool _isAtIntersection = isOnReturn && _intersectionWay == ERailWay::NegativeRailWay || !isOnReturn && _intersectionWay == ERailWay::PositiveRailWay;

			if (_intersectionWay == ERailWay::DefaultRailWay || _isAtIntersection)
			{
				if (_nextRail->GetIsLocked() || _nextRail->GetIsBlocked(isOnReturn, true))
					return;
			}

			currentRail = _nextRail;

			/*if (detectedRails.Contains(_currentRailMemory))
				_currentRailMemory->GetRailNotDetectedAnymore().Broadcast();*/

			currentDistance = currentRail->GetDistanceAtLocation(GetActorLocation());
			//CheckIfRailIsOnPath(currentRail);
		}
	}
}
#pragma endregion


#pragma region Speed

#pragma region Acceleration
void ACustomHandcarActor::FirstPlayerStartAcceleration()
{
	//firstPlayerAccelerates = true;
}

void ACustomHandcarActor::FirstPlayerReleaseAcceleration()
{
	//firstPlayerAccelerates = false;
}


void ACustomHandcarActor::SecondPlayerStartAcceleration()
{
	//secondPlayerAccelerates = true;
}

void ACustomHandcarActor::SecondPlayerReleaseAcceleration()
{
	//secondPlayerAccelerates = false;
}


//void ACustomHandcarActor::UpdateCurrentAcceleration()
//{
//	if (handle)
//	{
//		const float _firstPlayerHeightRapport = handle->GetPlayerHeightRapport(true);
//		const float _secondPlayerHeightRapport = handle->GetPlayerHeightRapport(false);
//
//		if (!firstPlayerAccelerates && secondPlayerAccelerates)
//			UpdateCurrentAcceleration(_secondPlayerHeightRapport, _firstPlayerHeightRapport);
//		else if (firstPlayerAccelerates && !secondPlayerAccelerates)
//			UpdateCurrentAcceleration(_firstPlayerHeightRapport, _secondPlayerHeightRapport);
//		else
//			UpdateCurrentAcceleration(0.0f, 0.0f);
//	}
//}

void ACustomHandcarActor::UpdateCurrentAcceleration(/*const float _lowestHeightRapport, const float _highestHeightRapport*/)
{
	float _finalAcceleration = /*pumpAcceleration + */slopeAcceleration;

	if (nitro)
	{
		const float _nitroAcceleration = nitro->GetUpdatedNitroAcceleration();

		if (brake)
		{
			if (brake->GetIsReleased())
				_finalAcceleration += _nitroAcceleration;
		}
		else
			_finalAcceleration += _nitroAcceleration;
	}

	//const float _slopeAngle = FMath::Abs(GetActorRotation().Pitch);

	if (currentSpeed < 0.0f)
	{
		//if (_slopeAngle <= frictionChangeAngle)
		_finalAcceleration += /*-frictionSlopeAcceleration*/ -frictionValue - (currentSpeed * frictionSpeedPercent);
	}
	else if (currentSpeed >= 0.0f)
		_finalAcceleration += /*frictionSlopeAcceleration*/ frictionValue - (currentSpeed * frictionSpeedPercent);

	if (brake)
	{
		brakeValue = brake->GetUpdatedBrakeValue(currentSpeed);
		_finalAcceleration += brakeValue;
	}

	/*if (_lowestHeightRapport >= 0.1f && _lowestHeightRapport <= 0.3f)
	{
		if (_highestHeightRapport >= 0.7f && _highestHeightRapport <= 0.9f)
			_finalAcceleration += accelerationBoost;
	}*/

	currentAcceleration = _finalAcceleration;
}

//void ACustomHandcarActor::UpdateFrictionSlopeAcceleration()
//{
//	const float _slopeAngle = FMath::DegreesToRadians(GetActorRotation().Pitch);
//	const float _gravity = -GetWorld()->GetGravityZ() / 100.0f;
//	frictionSlopeAcceleration = _gravity * (FMath::Sin(_slopeAngle) - (coefficientFriction * FMath::Cos(_slopeAngle)));
//}

void ACustomHandcarActor::UpdateSlopeAcceleration()
{
	if (slopeCurve && currentSpeed != 0.0f)
	{
		const float _slopeAngle = UCustomMathBlueprintLibrary::RoundToNumberAfterPoint(GetActorRotation().Pitch, 0);
		const float _newSlopeAcceleration = currentSpeed > 0.0f ? slopeCurve->GetFloatValue(_slopeAngle) : slopeCurve->GetFloatValue(-_slopeAngle);
		slopeAcceleration = _slopeAngle >= 0.0f ? -_newSlopeAcceleration : _newSlopeAcceleration;
	}
}

//void ACustomHandcarActor::UpdatePumpAcceleration(const float _lastHeight, const float _currentHeight)
//{
//	if (_currentHeight <= 0.0f)
//	{
//		pumpAcceleration = 0.0f;
//		return;
//	}
//
//	const float _heightRapport = FMath::Abs(_lastHeight - _currentHeight);
//	const float _loss = currentSpeed <= 0.0f ? 0.0f : currentSpeed / (maxSpeed * 10.0f);
//	pumpAcceleration = _heightRapport * FMath::Lerp(pumpMaxForce, pumpMinForce, _loss);
//}
#pragma endregion


void ACustomHandcarActor::UpdateCurrentSpeed(const float _deltaTime)
{
	if (!canMove)
		return;

	/*if (handle)
	{
		const float _firstPlayerLastHeightRapport = handle->GetPlayerLastHeightRapport(true);
		const float _secondPlayerLastHeightRapport = handle->GetPlayerLastHeightRapport(false);

		const float _firstPlayerHeightRapport = handle->GetPlayerHeightRapport(true);
		const float _secondPlayerHeightRapport = handle->GetPlayerHeightRapport(false);

		if (!firstPlayerAccelerates && secondPlayerAccelerates)
			UpdatePumpAcceleration(_secondPlayerLastHeightRapport, _secondPlayerHeightRapport);
		else if (firstPlayerAccelerates && !secondPlayerAccelerates)
			UpdatePumpAcceleration(_firstPlayerLastHeightRapport, _firstPlayerHeightRapport);
		else
			pumpAcceleration = 0.0f;
	}*/

	//UpdateFrictionSlopeAcceleration();
	UpdateSlopeAcceleration();
	UpdateCurrentAcceleration();

	const float _absSlopeAcc = FMath::Abs(slopeAcceleration);
	const float _absBrakeValue = FMath::Abs(brakeValue);
	const float _absFrictionValue = FMath::Abs(frictionValue);
	float _nitroAcc = 0.0f;

	if (nitro)
	{
		if (brake)
		{
			if (brake->GetIsReleased())
				_nitroAcc = nitro->GetUpdatedNitroAcceleration();
		}
		else
			_nitroAcc = nitro->GetUpdatedNitroAcceleration();
	}

	const float _slopeAngle = FMath::Abs(GetActorRotation().Pitch);
	const float _newSpeed = currentSpeed + (currentAcceleration * _deltaTime);
	const float _maxSpeed = maxSpeed * 10.0f;
	const float _noMoveAcc = _absFrictionValue + _absBrakeValue + _absSlopeAcc;

	//if (FMath::Abs(currentAcceleration) * _deltaTime == _noMoveAcc)
	//{
	//	currentSpeed = _newSpeed <= _noMoveAcc && _newSpeed >= -_noMoveAcc ? 0.0f
	//		: _newSpeed >= _maxSpeed ? _maxSpeed
	//		: _newSpeed;
	//}
	//else
	//{
	//	if (FMath::Abs(slopeAcceleration) <= FMath::Abs(brakeValue))
	//	{
	//		_noMoveAcc += FMath::Abs(slopeAcceleration) * _deltaTime;
	//		//LOG_COLOR_TIME(true, "[NoMoveAcc => " + FString::SanitizeFloat(_noMoveAcc), FColor::Magenta, 0.1f);
	//		currentSpeed = _newSpeed <= _noMoveAcc && _newSpeed >= -_noMoveAcc ? 0.0f
	//			: _newSpeed >= _maxSpeed ? _maxSpeed
	//			: _newSpeed;
	//	}
	//	else
	//	{
	//		currentSpeed = _newSpeed >= _maxSpeed ? _maxSpeed
	//			: _newSpeed;
	//	}
	//}

	const bool _manualStopCondition = brakeValue >= 0.0f ? _absSlopeAcc <= _absBrakeValue + _absFrictionValue + _nitroAcc
		: _absSlopeAcc + _nitroAcc <= _absBrakeValue + _absFrictionValue;

	if (_manualStopCondition)
	{
		/*if (FMath::Abs(currentAcceleration) != _noMoveAcc)
		{
			if (_absSlopeAcc != 0.0f)
				_noMoveAcc += _absSlopeAcc;
		}*/

		currentSpeed = _newSpeed <= _noMoveAcc * _deltaTime && _newSpeed >= -_noMoveAcc * _deltaTime ? 0.0f
			: FMath::Abs(_newSpeed) >= _maxSpeed ? (isOnReturn ? -_maxSpeed : _maxSpeed)
			: _newSpeed;
	}
	else
	{
		currentSpeed = FMath::Abs(_newSpeed) >= _maxSpeed ? (isOnReturn ? -_maxSpeed : _maxSpeed)
			: _newSpeed;
	}

	if (currentSpeed < 0.0f)
		isOnReturn = true;
	else
		isOnReturn = false;


	//UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::GPE, GetClass(), FString(HasAuthority() ? "[Server] =>" : "[Client] =>") + "Current Speed = > " + FString::SanitizeFloat(slopeAcceleration));

	if (!isStopped)
	{
		if (currentSpeed == 0.0f)
		{
			if (isAtStop)
			{
				for (AMainCharacter* _chara : players)
				{
					if (_chara)
					{
						_chara->SetCanJump(true);
						_chara->ReactivateMembers({ _chara->GetUsingLeftArm(), _chara->GetUsingRightArm() });
					}
				}

				isStopped = true;
				UnlockLifeAndEggStocks(true);
				onStop.Broadcast(true, inAutoMode);

				if (currentStop)
					currentStop->GetOnHandcarStop().Broadcast(true);

				SetInAutoMode(false);
			}

			for (UBoxComponent* _box : bumpBoxs)
				_box->SetGenerateOverlapEvents(false);
		}
	}
	else
	{
		if (currentSpeed != 0.0f)
		{
			for (AMainCharacter* _chara : players)
			{
				if (_chara)
				{
					_chara->SetCanJump(false);
					_chara->DeactivateMembers({ _chara->GetUsingLeftArm(), _chara->GetUsingRightArm() });
				}
			}

			isStopped = false;
			UnlockLifeAndEggStocks(false);
			onStop.Broadcast(false, inAutoMode);

			if (currentStop)
				currentStop->GetOnHandcarStop().Broadcast(false);

			for (UBoxComponent* _box : bumpBoxs)
				_box->SetGenerateOverlapEvents(true);
		}
	}
}
#pragma endregion


void ACustomHandcarActor::BeginPlay()
{
	Super::BeginPlay();

	maxRotSpeed = 0.067f * rotationSpeed;

	ACustomGameState* _gs = GetWorld()->GetGameState<ACustomGameState>();

	if (_gs)
	{
		UStopHandcarManager* _manager = _gs->GetHandcarManager();

		if (_manager)
			_manager->SetHandcar(this);
	}

	APlayerController* _controller = GetWorld()->GetFirstPlayerController();

	if (_controller)
		networkComp = _controller->GetComponentByClass<UNetworkGPEComponent>();

	GetDataAssetValues();

	if (lifeComponent)
		lifeComponent->SetLife(lifeComponent->GetMaxLife(), 0, nullptr);

	/*if (handle)
	{
		handle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		handle->SetHandcar(this);
	}*/

	if (brake)
	{
		brake->SetHandcar(this);
		brake->SetHandcarMaxSpeed(maxSpeed * 10.0f);
		//brake->SetStartingCoefficientFriction(coefficientFriction);
	}

	if (nitro)
		nitro->SetHandcar(this);

	int _num = lifeStocks.Num();
	ALifeStockHandcarActor* _lifeStock = nullptr;

	for (int i = 0; i < _num; i++)
	{
		_lifeStock = lifeStocks[i];

		if (_lifeStock)
		{
			_lifeStock->SetLifeComponent(lifeComponent);

			if (_lifeStock->GetIsAtExterior())
				onStop.AddDynamic(_lifeStock, &ALifeStockHandcarActor::StopInteraction);
		}
	}

	_num = eggsStock.Num();
	AEggStockHandcarActor* _eggStock = nullptr;

	for (int i = 0; i < _num; i++)
	{
		_eggStock = eggsStock[i];

		if (_eggStock)
		{
			if (_eggStock->GetIsAtExterior())
				onStop.AddDynamic(_eggStock, &AEggStockHandcarActor::StopInteraction);

			_eggStock->SetHandcar(this);
		}
	}

	if (currentRail)
	{
		const FVector _location = currentRail->GetStopLocationOnExit(true);
		currentDistance = currentRail->GetSpline()->GetDistanceAlongSplineAtLocation(_location, ESplineCoordinateSpace::World);
		SetActorLocation(_location);

		const float _distance = currentDistance + 1.0f;
		const FVector _currentTargetLoc = currentRail->GetLocationAtDistance(_distance);
		const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_location, _currentTargetLoc);
		SetActorRotation(_lookAt);
		//LOG_COLOR_TIME(true, _lookAt.ToString(), FColor::Red, 2.0f);
	}

	//if (HasAuthority())
	//	SetPosition();

	if (hitBox)
	{
		hitBox->OnComponentBeginOverlap.AddDynamic(this, &ACustomHandcarActor::OnComponentBeginOverlap);
		hitBox->OnComponentEndOverlap.AddDynamic(this, &ACustomHandcarActor::OnComponentEndOverlap);
	}

	if (currentStop)
	{
		isAtStop = true;
		isStopped = true;
	}

	onStop.Broadcast(isStopped, inAutoMode);
}

void ACustomHandcarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRuntime)
	{
		/*if (handle)
			handle->InHandcarTick(firstPlayerAccelerates, secondPlayerAccelerates, DeltaTime);*/

		/*if (brake)
				brake->InHandcarTick(DeltaTime, currentSpeed);*/

		/*if (nitro)
					nitro->InHandcarTick(DeltaTime);*/

		UpdateCurrentSpeed(DeltaTime);

		if (HasAuthority())
			CheckIfStartAutoMode();

		Move(DeltaTime);
		Rotate(DeltaTime);
		LogDebug();
	}
}

void ACustomHandcarActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	/*if (OtherActor && OtherActor->GetClass()->IsChildOf<AMainCharacter>())
	{
		AMainCharacter* _mc = Cast<AMainCharacter>(OtherActor);

		if (_mc)
			_mc->SetCanJump(false);
	}*/
}

void ACustomHandcarActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	/*if (OtherActor && OtherActor->GetClass()->IsChildOf<AMainCharacter>())
	{
		AMainCharacter* _mc = Cast<AMainCharacter>(OtherActor);

		if (_mc)
			_mc->SetCanJump(true);
	}*/
}


void ACustomHandcarActor::AddCallingStop(AStopHandcarActor* _stop)
{
	if (networkComp)
	{
		if (HasAuthority())
			networkComp->MulticastRPC_AddCallingStop(this, _stop);
		else
			networkComp->ServerRPC_AddCallingStop(this, _stop);
	}
}

void ACustomHandcarActor::AddCallingStop_Internal(AStopHandcarActor* _stop)
{
	if (_stop)
	{
		/*if (!HasAuthority())
		{
			if (UNetworkGPEComponent* _net = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>())
				_net->ServerRpc_SetNextStop(this, _stop);
			return;
		}*/


		const FString _name = _stop->GetName();

		if (!callingStops.Contains(_name))
		{
			if (callingStops.IsEmpty())
			{

				nextStop = _stop;
			}

			callingStops.Add(_name, _stop);
		}
	}
}

void ACustomHandcarActor::RemoveCallingStop(AStopHandcarActor* _stop)
{
	if (_stop)
	{
		callingStops.Remove(_stop->GetName());

		if (callingStops.IsEmpty())
			nextStop = nullptr;
	}
}

void ACustomHandcarActor::UpdateNextStop()
{
	if (nextStop)
		callingStops.Remove(nextStop->GetName());

	if (callingStops.IsEmpty())
		nextStop = nullptr;
	else
	{
		int _index = 0.0f;

		for (TPair<FString, AStopHandcarActor*> _pair : callingStops)
		{
			if (_index == 1.0f)
				break;
			nextStop = _pair.Value;
			_index = 1.0f;
		}
	}
}


void ACustomHandcarActor::AddCurrentTarget(ARailsTargetActor* _target)
{
	if (!currentTargets.Contains(_target))
		currentTargets.Add(_target);
}

void ACustomHandcarActor::RemoveCurrentTarget(ARailsTargetActor* _target)
{
	if (currentTargets.Contains(_target))
		currentTargets.Remove(_target);
}


void ACustomHandcarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACustomHandcarActor, position);
	DOREPLIFETIME(ACustomHandcarActor, currentSpeed);
	DOREPLIFETIME(ACustomHandcarActor, inAutoMode);
	DOREPLIFETIME(ACustomHandcarActor, nextStop);
}


void ACustomHandcarActor::RestoreLife(const float _life)
{
	if (lifeComponent)
		lifeComponent->RestoreLife(_life);
}


void ACustomHandcarActor::SetInAutoMode(const bool _inAutoMode)
{
	inAutoMode = _inAutoMode;

	if (HasAuthority())
		OnRep_UpdateInAutoMode(_inAutoMode);
	else
	{
		if (networkComp)
			networkComp->ServerRPC_UpdateInAutoMode(this, _inAutoMode);
	}

	if (!inAutoMode)
	{
		FTimerManager& _tm = GetWorld()->GetTimerManager();

		if (_tm.IsTimerActive(stopTimer))
			_tm.ClearTimer(stopTimer);

		canMove = true;
	}
}

void ACustomHandcarActor::OnRep_UpdateInAutoMode(const bool _inAutoMode)
{
	inAutoMode = _inAutoMode;
}


void ACustomHandcarActor::OnRep_UpdatePosition()
{
	SetActorLocation(position);
	currentDistance = currentRail->GetDistanceAtLocation(GetActorLocation());
}

void ACustomHandcarActor::SetPosition()
{
	position = GetActorLocation();
	FTimerHandle _setPositionTimer;
	WORLD->GetTimerManager().SetTimer(_setPositionTimer, this, &ACustomHandcarActor::SetPosition, 0.01f);
}


void ACustomHandcarActor::OnBumpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (AMainCharacter* _chara = Cast<AMainCharacter>(OtherActor))
	{
		if (OverlappedComponent->IsA<UBumpBoxComponent>())
			Cast<UBumpBoxComponent>(OverlappedComponent)->Bump(_chara, (FMath::Abs(currentSpeed) / (maxSpeed * 10)));
	}
}