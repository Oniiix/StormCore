#include "WagonActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/Game/Interactable/Lever/LeverActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "../../GPE/Transmitter/TargetActor/RailsTargetActor.h"

AWagonActor::AWagonActor()
{
	currentRail = nullptr;

	detectionRange = 1000.0f;
	detectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	detectionSphere->SetupAttachment(RootComponent);
	detectionSphere->SetGenerateOverlapEvents(true);
	detectionSphere->SetSphereRadius(detectionRange);

	movementSpeed = 25.0f;
	rotationSpeed = 25.0f;

	isBlocked = false;
	isStopped = true;
	isOnReturn = false;

	changeWayBefore = false;
	changeWayAfter = false;

	inputsSetup = false;

	detectedRails = TArray<ARailActor*>();

	bReplicates = true;
	SetReplicateMovement(true);
}

void AWagonActor::UpdateDetectionSphere()
{
	detectionSphere->SetSphereRadius(detectionRange);
}

void AWagonActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ARailActor>())
	{
		ARailActor* _rail = Cast<ARailActor>(OtherActor);

		if (_rail)
		{
			if (!_rail->GetIsLocked())
			{
				CheckIfRailIsOnPath(_rail);

				if (!detectedRails.Contains(_rail))
					detectedRails.Add(_rail);
			}
		}
	}
}

void AWagonActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ARailActor>())
	{
		ARailActor* _rail = Cast<ARailActor>(OtherActor);

		if (_rail)
		{
			if (!_rail->GetIsLocked())
			{
				if (detectedRails.Contains(_rail))
				{
					_rail->GetRailNotDetectedAnymore().Broadcast();
					detectedRails.Remove(_rail);
				}
			}
		}
	}
}

void AWagonActor::RefreshDetectedRails()
{
	for (ARailActor* _rail : detectedRails)
		CheckIfRailIsOnPath(_rail, false);
}

bool AWagonActor::CheckIfRailIsOnPath(ARailActor* _rail, const bool _remove)
{
	if (!_rail)
		return false;

	if (_rail == currentRail)
	{
		_rail->GetOnPathRailDetected().Broadcast();
		return true;
	}

	//ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
	ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);

	if (CheckIfTargetContainsRail(_nextTarget, _rail))
		return true;

	ARailActor* _nextRail = _nextTarget->GetNextRail(isOnReturn);
	_nextTarget = _nextRail->GetNextTarget(isOnReturn);

	if (!CheckIfTargetContainsRail(_nextTarget, _rail))
	{
		_rail->GetRailNotDetectedAnymore().Broadcast();
		return false;
	}

	return true;
}

//bool AWagonActor::CheckIfLeverContainsRail(ALeverActor* _lever, ARailActor* _rail)
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

bool AWagonActor::CheckIfTargetContainsRail(ARailsTargetActor* _target, ARailActor* _rail)
{
	if (!_target)
		return false;

	if (!isOnReturn)
	{
		if (_target->GetDepartureRails().Contains(_rail))
		{
			if (_rail->GetIsBlocked(isOnReturn, true))
				_rail->GetBlockedRailDetected().Broadcast();
			else
				_rail->GetOnPathRailDetected().Broadcast();

			return true;
		}
	}

	return false;
}

void AWagonActor::Move(float DeltaTime)
{
	if (!currentRail || isStopped)
		return;

	if (FVector::Distance(GetActorLocation(), currentRail->GetStopLocationOnExit(isOnReturn)) <= 1.0f)
	{
		if (currentRail->GetIsBlocked(isOnReturn, false))
		{
			isBlocked = true;
			return;
		}

		//ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
		ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);

		if (!_nextTarget)
		{
			isBlocked = true;
			return;
		}
		else
		{
			ARailActor* _nextRail = _nextTarget->GetNextRail(isOnReturn);

			if (_nextRail->GetIsLocked() || _nextRail->GetIsBlocked(isOnReturn, true))
			{
				isBlocked = true;
				return;
			}
		}
	}

	isBlocked = false;

	const FVector _currentTargetLoc = currentRail->GetLocationAtDistance(currentDistance);
	SetActorLocation(_currentTargetLoc);

	if (!isOnReturn)
		currentDistance += DeltaTime * movementSpeed * 10.0f;
	else
		currentDistance -= DeltaTime * movementSpeed * 10.0f;

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

void AWagonActor::Rotate(float DeltaTime)
{
	if (!currentRail || isStopped || isBlocked)
		return;

	const FVector _actorLoc = GetActorLocation();
	const float _distanceToAdd = isOnReturn ? (DeltaTime * FMath::Abs(movementSpeed) * 20.0f) : (DeltaTime * FMath::Abs(movementSpeed) * 40.0f);
	float _distance = currentDistance + _distanceToAdd;
	ARailActor* _lookAtRail = currentRail;

	if (_distance > currentRail->GetLength())
	{
		//ALeverActor* _nextLever = currentRail->GetNextLever(false);
		ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(false);
		_lookAtRail = _nextTarget->GetNextRail(false);
		_distance = _lookAtRail->GetDistanceAtLocation(_actorLoc) + _distanceToAdd;
	}

	const FVector _currentTargetLoc = _lookAtRail->GetLocationAtDistance(_distance);
	const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_actorLoc, _currentTargetLoc);
	const FRotator _rotation = FMath::Lerp(GetActorRotation(), _lookAt, DeltaTime * rotationSpeed);
	SetActorRotation(_rotation);
}

void AWagonActor::UpdateCurrentRail()
{
	ARailActor* _currentRailMemory = currentRail;
	//ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
	ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);

	if (_nextTarget)
	{
		/*if (!_nextTarget->GetIsLinkedToHandcar())
		{
			_nextTarget->GetOnUpdateRailIsBlocked().AddDynamic(this, &AWagonActor::RefreshDetectedRails);
			_nextTarget->SetIsLinkedToHandcar(true);
		}*/

		ARailActor* _nextRail = _nextTarget->GetNextRail(isOnReturn);

		if (_nextRail->GetIsLocked() || _nextRail->GetIsBlocked(isOnReturn, true))
			return;

		/*if (changeWayBefore)
		{
			isOnReturn = !isOnReturn;
			changeWayBefore = false;
		}*/

		currentRail = _nextRail;

		if (detectedRails.Contains(_currentRailMemory))
			_currentRailMemory->GetRailNotDetectedAnymore().Broadcast();

		/*if (changeWayAfter)
		{
			isOnReturn = !isOnReturn;
			changeWayAfter = false;
		}*/

		currentDistance = currentRail->GetDistanceAtLocation(GetActorLocation());
		CheckIfRailIsOnPath(currentRail);
	}
	else
	{
		isStopped = true;

		/*if (!HasAuthority())
			ServerRpc_SetIsStopped(isStopped);*/
	}
}

void AWagonActor::Stop()
{
	isStopped = true;

	/*if (!HasAuthority())
		ServerRpc_SetIsStopped(isStopped);*/
}

#pragma region Network
//void AWagonActor::OnRep_UpdateIsStopped()
//{
//	LOG(WAGON_USE_LOG, isStopped ? "Stopped" : "Moving");
//}
//
//void AWagonActor::ServerRpc_SetIsStopped_Implementation(const bool _isStopped)
//{
//	LOG(WAGON_USE_LOG, "Rpc is Stopped");
//	isStopped = _isStopped;
//	OnRep_UpdateIsStopped();
//}
//
//void AWagonActor::OnRep_UpdateIsOnReturn()
//{
//	LOG(WAGON_USE_LOG, isOnReturn ? "On return" : "Departure");
//}
//
//void AWagonActor::ServerRpc_SetIsOnReturn_Implementation(const bool _isOnReturn)
//{
//	LOG(WAGON_USE_LOG, "Rpc is on return");
//	isOnReturn = _isOnReturn;
//	OnRep_UpdateIsOnReturn();
//}
#pragma endregion

void AWagonActor::SetupPlayerInputs(AMainCharacter* _player)
{
	if (inputsSetup)
		return;

	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
	UInputConfig* _inputConfig = _player->GetInputConfig();

	if (_input && _inputConfig)
	{
		_input->BindAction(_player->GetInputConfig()->GetWagonStop(), ETriggerEvent::Started, this, &AWagonActor::Stop);
		inputsSetup = true;
	}
}

void AWagonActor::BeginPlay()
{
	Super::BeginPlay();

	if (currentRail)
	{
		const FVector _location = currentRail->GetStopLocationOnExit(true);
		currentDistance = currentRail->GetSpline()->GetDistanceAlongSplineAtLocation(_location, ESplineCoordinateSpace::World);
		SetActorLocation(_location);
	}

	if (detectionSphere)
	{
		detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWagonActor::OnComponentBeginOverlap);
		detectionSphere->OnComponentEndOverlap.AddDynamic(this, &AWagonActor::OnComponentEndOverlap);
		CheckIfRailIsOnPath(currentRail);

		//ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
		ARailsTargetActor* _nextTarget = currentRail->GetNextTarget(isOnReturn);

		/*if (_nextTarget)
		{
			if (!_nextTarget->GetIsLinkedToHandcar())
			{
				_nextTarget->GetOnUpdateRailIsBlocked().AddDynamic(this, &AWagonActor::RefreshDetectedRails);
				_nextTarget->SetIsLinkedToHandcar(true);
			}
		}*/
	}
}

void AWagonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRuntime && HasAuthority())
	{
		Move(DeltaTime);
		Rotate(DeltaTime);
	}
}

/*void AWagonActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWagonActor, isStopped);
	DOREPLIFETIME(AWagonActor, isOnReturn);
}*/

#if WITH_EDITOR
void AWagonActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool _canUpdateDetectionSphere = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AWagonActor, detectionRange);

	if (_canUpdateDetectionSphere)
		UpdateDetectionSphere();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AWagonActor::Interacted(AMainCharacter* _character)
{
	if (!isStopped)
		isOnReturn = !isOnReturn;

	isStopped = false;

	/*if (!_character->HasAuthority())
	{
		UDraisineComponent* _comp = _character->GetDraisineComponent();
		_comp->ServerRpc_SetIsOnReturn(isOnReturn, this);
		_comp->ServerRpc_SetIsStopped(isStopped, this);
	}*/

	SetupPlayerInputs(_character);
}

void AWagonActor::ChangeWay()
{
	/*AChangeWayRailActor* _currentRail = Cast<AChangeWayRailActor>(currentRail);

	if (_currentRail)
	{
		changeWayBefore = true;
		return;
	}

	ALeverActor* _nextLever = currentRail->GetNextLever(isOnReturn);
	AChangeWayRailActor* _nextRail = Cast<AChangeWayRailActor>(_nextLever->GetNextRail(isOnReturn));

	if (_nextRail)
		changeWayAfter = true;*/
}
