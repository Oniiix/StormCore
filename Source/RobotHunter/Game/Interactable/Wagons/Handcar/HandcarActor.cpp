#include "HandcarActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"

AHandcarActor::AHandcarActor()
{
	movementSpeed = 0.0f;
	rotationSpeed = 10.0f;
	isStopped = false;

	bReplicates = true;
	SetReplicateMovement(true);

#pragma region Inputs
	handcarContext = nullptr;
	gameContext = nullptr;

	serverInputsSetup = false;
	clientInputsSetup = false;
#pragma endregion Inputs

#pragma region Spawn
	firstPlayerSpawnLocation = CreateDefaultSubobject<USceneComponent>("First Player Spawn Location");
	firstPlayerSpawnLocation->SetupAttachment(RootComponent);

	secondPlayerSpawnLocation = CreateDefaultSubobject<USceneComponent>("Second Player Spawn Location");
	secondPlayerSpawnLocation->SetupAttachment(RootComponent);
#pragma endregion Spawn

#pragma region Interaction
	secondInteractionFeedback = CreateDefaultSubobject<UTextRenderComponent>("SecondInteractionFeedback");
	secondInteractionFeedback->SetupAttachment(RootComponent);

	triggerZone = CreateDefaultSubobject<UBoxComponent>("TriggerZone");
	triggerZone->SetupAttachment(RootComponent);

	inputTime = 1.0f;
	hornCooldown = 1.0f;

	firstPlayer = nullptr;
	secondPlayer = nullptr;

	inputTimer = FTimerHandle();
	hornTimer = FTimerHandle();
#pragma endregion Interaction

#pragma region Handle
	handleMesh = CreateDefaultSubobject<UStaticMeshComponent>("HandleMesh");
	handleMesh->SetGenerateOverlapEvents(true);
	handleMesh->SetupAttachment(mesh);

	firstHandlePlayerLocation = CreateDefaultSubobject<USceneComponent>("First Handle Player Location");
	firstHandlePlayerLocation->SetupAttachment(mesh);

	secondHandlePlayerLocation = CreateDefaultSubobject<USceneComponent>("Second Handle Player Location");
	secondHandlePlayerLocation->SetupAttachment(mesh);

	maxHandleRotation = 45.0f;
	handleRotationSpeed = 50.0f;

	firstHandleDetected = false;
	secondHandleDetected = false;

	firstPlayerHandleHeightRapport = 0.5f;
	secondPlayerHandleHeightRapport = 0.5f;

	accelerationStartingHeight = 0.0f;
#pragma endregion Handle

#pragma region Speed
	normalSpeed = 10.0f;
	boostedSpeed = 20.0f;

	accelerationForce = 5.0f;
	decelerationForce = 2.0f;
	brakeDecelerationForce = 5.0f;

	firstPlayerAccelerates = false;
	secondPlayerAccelerates = false;

	accelerationStartingSpeed = 0.0f;
	finalSpeed = 0.0f;

	naturalDecelerationTimer = FTimerHandle();
	brakeDecelerationTimer = FTimerHandle();
#pragma endregion Speed
}

#pragma region Inputs
void AHandcarActor::SetupPlayerInputs(AMainCharacter* _player)
{
	/*const bool _hasAuthority = _player->HasAuthority();

	if ((_hasAuthority && serverInputsSetup) || (!_hasAuthority && clientInputsSetup))
		return;

	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
	UInputConfig* _inputConfig = _player->GetInputConfig();

	if (_input && _inputConfig)
	{
		_input->BindAction(_player->GetInputConfig()->GetHandcarHornInput(), ETriggerEvent::Started, this, &AHandcarActor::Horn);

		if (_hasAuthority)
		{
			_input->BindAction(_player->GetInputConfig()->GetHandcarAccelerationInput(), ETriggerEvent::Started, this, &AHandcarActor::ServerStartAcceleration);
			_input->BindAction(_player->GetInputConfig()->GetHandcarAccelerationInput(), ETriggerEvent::Completed, this, &AHandcarActor::ServerReleaseAcceleration);

			_input->BindAction(_player->GetInputConfig()->GetHandcarBrakeInput(), ETriggerEvent::Started, this, &AHandcarActor::StartBrakeDeceleration);
			_input->BindAction(_player->GetInputConfig()->GetHandcarBrakeInput(), ETriggerEvent::Completed, this, &AHandcarActor::ReleaseBrakeDeceleration);

			_input->BindAction(_player->GetInputConfig()->GetHandcarChangeWayInput(), ETriggerEvent::Started, this, &AHandcarActor::ChangeWay);

			serverInputsSetup = true;
		}
		else
		{
			_input->BindAction(_player->GetInputConfig()->GetHandcarAccelerationInput(), ETriggerEvent::Started, this, &AHandcarActor::ClientStartAcceleration);
			_input->BindAction(_player->GetInputConfig()->GetHandcarAccelerationInput(), ETriggerEvent::Completed, this, &AHandcarActor::ClientReleaseAcceleration);

			_input->BindAction(_player->GetInputConfig()->GetHandcarBrakeInput(), ETriggerEvent::Started, this, &AHandcarActor::ClientStartBrakeDeceleration);
			_input->BindAction(_player->GetInputConfig()->GetHandcarBrakeInput(), ETriggerEvent::Completed, this, &AHandcarActor::ClientReleaseBrakeDeceleration);

			_input->BindAction(_player->GetInputConfig()->GetHandcarChangeWayInput(), ETriggerEvent::Started, this, &AHandcarActor::ClientChangeWay);

			clientInputsSetup = true;
		}
	}*/

}

void AHandcarActor::ChangeContext(AMainCharacter* _player, UInputMappingContext* _toDisable, UInputMappingContext* _toEnable)
{
	if (_toDisable && _toEnable)
	{
		_player->DisablePlayerContext(_toDisable);
		_player->EnablePlayerContext(_toEnable);
	}
}
#pragma endregion

#pragma region Interaction
void AHandcarActor::EnterHandcar(AMainCharacter* _player, const FVector& _handleLoc)
{
	SetupPlayerInputs(_player);
	ChangeContext(_player, gameContext, handcarContext);

	_player->SetActorLocation(_handleLoc);

	const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_player->GetActorLocation(), _handleLoc == firstHandlePlayerLocation->GetComponentLocation() ? secondHandlePlayerLocation->GetComponentLocation() : firstHandlePlayerLocation->GetComponentLocation());
	_player->SetMeshRotation(_lookAt, true);

	_player->GetCameraComponent()->UpdateCurrentSettings("D");
}

void AHandcarActor::ExitHandcar(AMainCharacter* _player, const FVector& _spawnLoc)
{
	_player->SetActorLocation(_spawnLoc);

	_player->GetCameraComponent()->UpdateCurrentSettings("Exploration");

	ChangeContext(_player, handcarContext, gameContext);
}

//

void AHandcarActor::FirstHandleInteraction(AMainCharacter* _chara)
{
	//UDraisineComponent* _cmp = _chara->GetDraisineComponent();

	if (firstPlayer)
	{
		if (firstPlayer != _chara)
			return;

		/*if (_cmp)
			_cmp->ServerRPC_SetFirstPlayer(_chara, firstPlayerSpawnLocation->GetComponentLocation(), this);*/

		ExitHandcar(_chara, firstPlayerSpawnLocation->GetComponentLocation());
		firstPlayer = nullptr;
	}
	else
	{

		firstPlayer = _chara;
		EnterHandcar(firstPlayer, firstHandlePlayerLocation->GetComponentLocation());

		/*if (_cmp)
			_cmp->ServerRPC_SetFirstPlayer(_chara, firstHandlePlayerLocation->GetComponentLocation(), this);*/
	}
}

void AHandcarActor::SecondHandleInteraction(AMainCharacter* _chara)
{
	//UDraisineComponent* _cmp = _chara->GetDraisineComponent();

	if (secondPlayer)
	{
		if (secondPlayer != _chara)
			return;

		/*if (!HasAuthority() && _cmp)
			_cmp->ServerRPC_SetSecondPlayer(_chara, secondPlayerSpawnLocation->GetComponentLocation(), this);*/

		ExitHandcar(secondPlayer, secondPlayerSpawnLocation->GetComponentLocation());
		secondPlayer = nullptr;
	}
	else
	{
		/*if (!HasAuthority() && _cmp)
			_cmp->ServerRPC_SetSecondPlayer(_chara, secondHandlePlayerLocation->GetComponentLocation(), this);*/

		secondPlayer = _chara;
		EnterHandcar(secondPlayer, secondHandlePlayerLocation->GetComponentLocation());
	}
}

void AHandcarActor::ChangeWay(const FInputActionValue& _value)
{
	isOnReturn = !isOnReturn;
	changeWayDeceleration = true;
	firstPlayerAccelerates = false;
	secondPlayerAccelerates = false;
}

void AHandcarActor::ClientChangeWay(const FInputActionValue& _value)
{
	ChangeWay(_value);

	//UDraisineComponent* _cmp = firstPlayer->HasAuthority() ? secondPlayer->GetDraisineComponent() : firstPlayer->GetDraisineComponent();

	//if (_cmp)
	//{
	//	_cmp->ServerRpc_SetIsOnReturn(isOnReturn, this);
	//	//_cmp->ServerRPC_SetFirstPlayerAccelerates(firstPlayerAccelerates, this);
	//	//_cmp->ServerRPC_SetFirstPlayerAccelerates(secondPlayerAccelerates, this);
	//}
}

void AHandcarActor::Horn(const FInputActionValue& _value)
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if (_timerManager.IsTimerActive(hornTimer))
		return;

	LOG(HANDCAR_USE_LOG, "Pouet");

	_timerManager.SetTimer(hornTimer, hornCooldown, false);
}

void AHandcarActor::NoInputForTooMuchTime()
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if (_timerManager.IsTimerActive(brakeDecelerationTimer))
		return;

	firstPlayerAccelerates = false;
	secondPlayerAccelerates = false;
	_timerManager.SetTimer(naturalDecelerationTimer, this, &AHandcarActor::NaturalDeceleration, 1.0f, true);
}
#pragma endregion

#pragma region Handle
void AHandcarActor::UpdateHandleDetected(AMainCharacter* _chara)
{
	const FVector _charaLoc = _chara->GetActorLocation();

	const float _firstHandleDistance = FVector::Distance(_charaLoc, firstHandlePlayerLocation->GetComponentLocation());
	const float _secondHandleDistance = FVector::Distance(_charaLoc, secondHandlePlayerLocation->GetComponentLocation());

	firstHandleDetected = _firstHandleDistance <= _secondHandleDistance;
	secondHandleDetected = !firstHandleDetected;
}

void AHandcarActor::UpdateInteractionFeedbackVisibility()
{
	interactionFeedback->SetVisibility(firstHandleDetected);
	secondInteractionFeedback->SetVisibility(secondHandleDetected);
}

//

void AHandcarActor::RotateHandle(const float _maxHandleRotation, const float _handleRotationSpeed)
{
	const FRotator _currentRot = handleMesh->GetRelativeRotation();

	if (FMath::Abs(_currentRot.Pitch - _maxHandleRotation) <= 1.0f)
		return;

	float _newPitch = _currentRot.Pitch + (_handleRotationSpeed * GetWorld()->DeltaTimeSeconds);
	_newPitch = _maxHandleRotation <= 0.0f && _newPitch >= _maxHandleRotation ? _newPitch :
		_maxHandleRotation >= 0.0f && _newPitch <= _maxHandleRotation ? _newPitch :
		_currentRot.Pitch;

	handleMesh->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
}

void AHandcarActor::UpdateHandleHeightRapports()
{
	const FRotator _currentRot = handleMesh->GetRelativeRotation();
	const float _angleScope = maxHandleRotation * 2.0f;
	const float _heightValueOnScope = 1.0f - (FMath::Abs(_currentRot.Pitch + maxHandleRotation) / _angleScope);

	firstPlayerHandleHeightRapport = _heightValueOnScope;
	secondPlayerHandleHeightRapport = 1.0f - firstPlayerHandleHeightRapport;

	LOG_COLOR_TIME(HANDCAR_USE_LOG, "[First] => " + FString::SanitizeFloat(firstPlayerHandleHeightRapport), MAGENTA, 2);

	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if ((firstPlayerHandleHeightRapport > 0.9f && firstPlayerAccelerates) || (secondPlayerHandleHeightRapport > 0.9f && secondPlayerAccelerates))
	{
		if (!_timerManager.IsTimerActive(inputTimer))
			_timerManager.SetTimer(inputTimer, this, &AHandcarActor::NoInputForTooMuchTime, inputTime, false);
		else
			LOG(HANDCAR_USE_LOG, "input timer" + FString::SanitizeFloat(_timerManager.GetTimerElapsed(inputTimer)));
	}
	else if (_timerManager.IsTimerActive(inputTimer))
		_timerManager.ClearTimer(inputTimer); 
}
#pragma endregion

#pragma region Speed
void AHandcarActor::StartAcceleration(bool& _playerAccelerates, const float _handleHeightRapport)
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	_timerManager.ClearTimer(naturalDecelerationTimer);
	_timerManager.ClearTimer(inputTimer);

	if (_timerManager.IsTimerActive(brakeDecelerationTimer))
		return;

	_playerAccelerates = true;
	UpdateFinalSpeed();
	accelerationStartingSpeed = movementSpeed;
	accelerationStartingHeight = _handleHeightRapport;
}

void AHandcarActor::ReleaseAcceleration(bool& _playerAccelerates)
{
	_playerAccelerates = false;

	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if (!secondPlayerAccelerates && !firstPlayerAccelerates && !_timerManager.IsTimerActive(brakeDecelerationTimer))
		_timerManager.SetTimer(naturalDecelerationTimer, this, &AHandcarActor::NaturalDeceleration, 1.0f, true, 0.3f);
}

//

void AHandcarActor::FirstPlayerStartAcceleration()
{
	StartAcceleration(firstPlayerAccelerates, firstPlayerHandleHeightRapport);
}

void AHandcarActor::FirstPlayerReleaseAcceleration()
{
	ReleaseAcceleration(firstPlayerAccelerates);
}

//

void AHandcarActor::SecondPlayerStartAcceleration()
{
	StartAcceleration(secondPlayerAccelerates, secondPlayerHandleHeightRapport);
}

void AHandcarActor::SecondPlayerReleaseAcceleration()
{
	ReleaseAcceleration(secondPlayerAccelerates);
}

//

void AHandcarActor::ServerStartAcceleration(const FInputActionValue& _value)
{
	LOG_COLOR_TIME(HANDCAR_USE_LOG, "Coucou le serv Accelere", MAGENTA, 2);

	if (firstPlayer && firstHandleDetected)
		StartAcceleration(firstPlayerAccelerates, firstPlayerHandleHeightRapport);
	else if (secondPlayer && secondHandleDetected)
		StartAcceleration(secondPlayerAccelerates, secondPlayerHandleHeightRapport);
}

void AHandcarActor::ServerReleaseAcceleration(const FInputActionValue& _value)
{
	LOG_COLOR_TIME(HANDCAR_USE_LOG, "Coucou le serv freine", YELLOW, 2);

	if (firstPlayer && firstHandleDetected)
		ReleaseAcceleration(firstPlayerAccelerates);
	else if (secondPlayer && secondHandleDetected)
		ReleaseAcceleration(secondPlayerAccelerates);
}

void AHandcarActor::ClientStartAcceleration(const FInputActionValue& _value)
{
	//UDraisineComponent* _cmp = nullptr;

	LOG_COLOR_TIME(HANDCAR_USE_LOG, "Coucou le client Accelere", MAGENTA, 2);

	if (firstPlayer && firstHandleDetected)
	{
		//FirstPlayerStartAcceleration();
		//_cmp = firstPlayer->GetDraisineComponent();

		/*if (_cmp)
			_cmp->ServerRPC_FirstPlayerStartAcceleration(this);*/
	}
	else if (secondPlayer && secondHandleDetected)
	{
		//SecondPlayerStartAcceleration();
		//_cmp = secondPlayer->GetDraisineComponent();

		/*if (_cmp)
			_cmp->ServerRPC_SecondPlayerStartAcceleration(this);*/
	}

}

void AHandcarActor::ClientReleaseAcceleration(const FInputActionValue& _value)
{
	//UDraisineComponent* _cmp = nullptr;

	LOG_COLOR_TIME(HANDCAR_USE_LOG, "Coucou le client release", YELLOW, 10);

	if (firstPlayer && firstHandleDetected)
	{
		//FirstPlayerReleaseAcceleration();
		//_cmp = firstPlayer->GetDraisineComponent();

		/*if (_cmp)
			_cmp->ServerRPC_FirstPlayerReleaseAcceleration(this);*/
	}
	else if (secondPlayer && secondHandleDetected)
	{
		//SecondPlayerReleaseAcceleration();
		/*_cmp = secondPlayer->GetDraisineComponent();

		if (_cmp)
			_cmp->ServerRPC_SecondPlayerReleaseAcceleration(this);*/
	}
}

//

void AHandcarActor::SetBrakeDecelerationTimer()
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if (_timerManager.IsTimerActive(brakeDecelerationTimer))
		return;

	firstPlayerAccelerates = false;
	secondPlayerAccelerates = false;
	_timerManager.ClearTimer(naturalDecelerationTimer);
	_timerManager.SetTimer(brakeDecelerationTimer, this, &AHandcarActor::BrakeDeceleration, 1.0f, true);
}

void AHandcarActor::ClearBrakeDecelerationTimer()
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if (_timerManager.IsTimerActive(brakeDecelerationTimer))
		GetWorld()->GetTimerManager().ClearTimer(brakeDecelerationTimer);
}

void AHandcarActor::StartBrakeDeceleration(const FInputActionValue& _value)
{
	SetBrakeDecelerationTimer();
}

void AHandcarActor::ReleaseBrakeDeceleration(const FInputActionValue& _value)
{
	ClearBrakeDecelerationTimer();
}

//

void AHandcarActor::ClientStartBrakeDeceleration(const FInputActionValue& _value)
{
	//UDraisineComponent* _cmp = firstPlayer->HasAuthority() ? secondPlayer->GetDraisineComponent() : firstPlayer->GetDraisineComponent();

	//if (_cmp)
	//	_cmp->ServerRPC_StartBrakeDeceleration(this);
}

void AHandcarActor::ClientReleaseBrakeDeceleration(const FInputActionValue& _value)
{
	//UDraisineComponent* _cmp = firstPlayer->HasAuthority() ? secondPlayer->GetDraisineComponent() : firstPlayer->GetDraisineComponent();

	//if (_cmp)
	//	_cmp->ServerRPC_ReleaseBrakeDeceleration(this);
}

void AHandcarActor::UpdateFinalSpeed()
{
	if (!firstPlayerAccelerates && secondPlayerAccelerates)
		UpdateFinalSpeedFromHandleHeightRapports(secondPlayerHandleHeightRapport, firstPlayerHandleHeightRapport);
	else if (firstPlayerAccelerates && !secondPlayerAccelerates)
		UpdateFinalSpeedFromHandleHeightRapports(firstPlayerHandleHeightRapport, secondPlayerHandleHeightRapport);
}

void AHandcarActor::UpdateFinalSpeedFromHandleHeightRapports(const float _lowestHeightRapport, const float _highestHeightRapport)
{
	if (_lowestHeightRapport >= 0.1f && _lowestHeightRapport <= 0.3f)
	{
		if (_highestHeightRapport >= 0.7f && _highestHeightRapport <= 0.9f)
		{
			finalSpeed = boostedSpeed;
			return;
		}
	}

	finalSpeed = normalSpeed;
}

void AHandcarActor::UpdateMovementSpeed()
{
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();

	if (_timerManager.IsTimerActive(brakeDecelerationTimer))
		return;

	if (!firstPlayerAccelerates && secondPlayerAccelerates)
		Accelerates(accelerationStartingHeight, secondPlayerHandleHeightRapport, accelerationStartingSpeed);
	else if (firstPlayerAccelerates && !secondPlayerAccelerates)
		Accelerates(accelerationStartingHeight, firstPlayerHandleHeightRapport, accelerationStartingSpeed);
	else
	{
		if (movementSpeed > 0.0f && !_timerManager.IsTimerActive(naturalDecelerationTimer))
			_timerManager.SetTimer(naturalDecelerationTimer, this, &AHandcarActor::NaturalDeceleration, 1.0f, true, 0.3f);
	}
}

void AHandcarActor::Accelerates(const float _startingHeight, const float _currentHeight, const float _startingSpeed)
{
	if (movementSpeed >= finalSpeed)
		return;

	const float _forceMultiplier = FMath::Abs(_startingHeight - _currentHeight);

	if (_currentHeight <= _startingHeight)
		return;

	const float _forceToAdd = changeWayDeceleration ? -accelerationForce * _forceMultiplier : accelerationForce * _forceMultiplier;
	const float _newSpeed = _startingSpeed + _forceToAdd;
	movementSpeed = _newSpeed <= finalSpeed ? _newSpeed : finalSpeed;
	movementSpeed = _newSpeed >= 0.0f ? _newSpeed : 0.0f;

	if (movementSpeed == 0.0f)
		changeWayDeceleration = false;
}

void AHandcarActor::Decelerates(const float _decelerationForce)
{
	const float _newSpeed = movementSpeed - _decelerationForce;
	movementSpeed = _newSpeed >= 0.0f ? _newSpeed : 0.0f;
}

void AHandcarActor::NaturalDeceleration()
{
	Decelerates(decelerationForce);

	if (movementSpeed == 0.0f)
		GetWorld()->GetTimerManager().ClearTimer(naturalDecelerationTimer);
}

void AHandcarActor::BrakeDeceleration()
{
	LOG_COLOR_TIME(HANDCAR_USE_LOG, "Brake", RED, 2);
	Decelerates(brakeDecelerationForce);
}
#pragma endregion

void AHandcarActor::BeginPlay()
{
	Super::BeginPlay();
	secondInteractionFeedback->SetVisibility(false);
	triggerZone->OnComponentBeginOverlap.AddDynamic(this, &AHandcarActor::OnEnterTrigger);
	triggerZone->OnComponentEndOverlap.AddDynamic(this, &AHandcarActor::OnExitTrigger);
}

//void ARHHandcarActor::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	AMainCharacter* _character = Cast<AMainCharacter>(OtherActor);
//
//	if (_character && _character->GetIsOwner())
//	{
//		URHInteractionComponent* _interactionComponent = _character->GetInteractionComponent();
//
//		if (_interactionComponent)
//		{
//			_interactionComponent->AddActor(GetName(), this);
//			UpdateHandleDetected(_character);
//			UpdateInteractionFeedbackVisibility();
//			isDetected = true;
//		}
//	}
//}
//
//void ARHHandcarActor::NotifyActorEndOverlap(AActor* OtherActor)
//{
//	AMainCharacter* _character = Cast<AMainCharacter>(OtherActor);
//
//	if (_character && _character->GetIsOwner())
//	{
//		URHInteractionComponent* _interactionComponent = _character->GetInteractionComponent();
//
//		if (_interactionComponent)
//		{
//			_interactionComponent->RemoveActor(GetName());
//			isDetected = false;
//
//			firstHandleDetected = false;
//			secondHandleDetected = false;
//			UpdateInteractionFeedbackVisibility();
//		}
//	}
//}

void AHandcarActor::OnEnterTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	AMainCharacter* _character = Cast<AMainCharacter>(_OtherActor);

	if (_character && _character->GetIsOwner())
	{
		UInteractionComponent* _interactionComponent = _character->GetInteractionComponent();

		if (_interactionComponent)
		{
			_interactionComponent->AddActor(GetName(), this);
			UpdateHandleDetected(_character);
			UpdateInteractionFeedbackVisibility();
			isDetected = true;
		}
	}
}

void AHandcarActor::OnExitTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
	AMainCharacter* _character = Cast<AMainCharacter>(_OtherActor);

	if (_character && _character->GetIsOwner())
	{
		UInteractionComponent* _interactionComponent = _character->GetInteractionComponent();

		if (_interactionComponent)
		{
			_interactionComponent->RemoveActor(GetName());
			isDetected = false;

			firstHandleDetected = false;
			secondHandleDetected = false;
			UpdateInteractionFeedbackVisibility();
		}
	}
}

void AHandcarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRuntime && HasAuthority())
	{
		LOG_COLOR_TIME(HANDCAR_USE_LOG, "[First] => " + FString::FromInt(firstPlayerAccelerates), MAGENTA, 2);
		LOG_COLOR_TIME(HANDCAR_USE_LOG, "[Second] => " + FString::FromInt(secondPlayerAccelerates), YELLOW, 2);

		if (firstPlayerAccelerates && !secondPlayerAccelerates)
		{
			RotateHandle(-maxHandleRotation, -handleRotationSpeed);
		}
		else if (secondPlayerAccelerates && !firstPlayerAccelerates)
		{
			RotateHandle(maxHandleRotation, handleRotationSpeed);
		}
		else
		{
			const FRotator _handleCurrentRot = handleMesh->GetRelativeRotation();
			const float _handleCurrentPitch = _handleCurrentRot.Pitch;
			RotateHandle(0.0f, _handleCurrentPitch > 0.0f ? -handleRotationSpeed : handleRotationSpeed);
		}

		UpdateHandleHeightRapports();
		UpdateMovementSpeed();
	}
}

void AHandcarActor::TickUpdateInteractionFeedback()
{
	if (!isDetected)
		return;

	if (firstHandleDetected)
		UpdateInteractionFeedback(interactionFeedback);
	else if (secondHandleDetected)
		UpdateInteractionFeedback(secondInteractionFeedback);
}

void AHandcarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHandcarActor, firstPlayer);
	DOREPLIFETIME(AHandcarActor, secondPlayer);

	DOREPLIFETIME(AHandcarActor, firstPlayerAccelerates);
	DOREPLIFETIME(AHandcarActor, secondPlayerAccelerates);

	DOREPLIFETIME(AHandcarActor, changeWayDeceleration);

	DOREPLIFETIME(AHandcarActor, brakeDecelerationTimer);
}

void AHandcarActor::Interacted(AMainCharacter* _chara)
{
	SetOwner(_chara);

	if (firstHandleDetected)
		FirstHandleInteraction(_chara);
	else if (secondHandleDetected)
		SecondHandleInteraction(_chara);
}
