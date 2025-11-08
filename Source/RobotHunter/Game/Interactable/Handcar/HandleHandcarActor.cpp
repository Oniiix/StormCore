#include "HandleHandcarActor.h"
#include "../../../Utils/DebugUtils.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../GPE/Handcar/CustomHandcarActor.h"
#include <Net/UnrealNetwork.h>
#include "EnhancedInputComponent.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"

AHandleHandcarActor::AHandleHandcarActor()
{
#pragma region Interaction

#pragma region Trigger
	trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	trigger->SetupAttachment(RootComponent);

	playerInTrigger = nullptr;
#pragma endregion


#pragma region Player
	secondInteractionFeedback = CreateDefaultSubobject<UTextRenderComponent>("SecondInteractionFeedback");
	secondInteractionFeedback->SetupAttachment(RootComponent);


	firstPlayerLocation = CreateDefaultSubobject<USceneComponent>("First Player Location");
	firstPlayerLocation->SetupAttachment(mesh);

	secondPlayerLocation = CreateDefaultSubobject<USceneComponent>("Second Player Location");
	secondPlayerLocation->SetupAttachment(mesh);


	firstPlayer = nullptr;
	secondPlayer = nullptr;


	firstSideDetected = false;
	secondSideDetected = false;
#pragma endregion

#pragma endregion


#pragma region Rotation
	handleMesh = CreateDefaultSubobject<UStaticMeshComponent>("HandleMesh");
	handleMesh->SetGenerateOverlapEvents(true);
	handleMesh->SetupAttachment(mesh);

	maxRotation = 45.0f;
	rotationValue = 50.0f;

	firstPlayerHeightRapport = 0.5f;
	secondPlayerHeightRapport = 0.5f;

	firstPlayerLastHeightRapport = 0.5f;
	secondPlayerLastHeightRapport = 0.5f;
#pragma endregion


#pragma region Handcar
	handcar = nullptr;
#pragma endregion


	inputsSetup = false;

	/*bReplicates = true;
	SetReplicateMovement(true);*/
}


void AHandleHandcarActor::SetupPlayerInputs(AMainCharacter* _player)
{
	if (inputsSetup)
		return;

	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
	UInputConfig* _inputConfig = _player->GetInputConfig();

	if (_input && _inputConfig)
	{
		_input->BindAction(_inputConfig->GetHandcarAccelerationInput(), ETriggerEvent::Started, this,
		                   &AHandleHandcarActor::StartAcceleration);
		_input->BindAction(_inputConfig->GetHandcarAccelerationInput(), ETriggerEvent::Completed, this,
		                   &AHandleHandcarActor::ReleaseAcceleration);
		inputsSetup = true;
	}
}


#pragma region Interaction

#pragma region Trigger
void AHandleHandcarActor::OnEnterTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor,
                                         UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep,
                                         const FHitResult& _SweepResult)
{
	AMainCharacter* _character = Cast<AMainCharacter>(_OtherActor);

	if (_character && _character->GetIsOwner())
		playerInTrigger = _character;
}

void AHandleHandcarActor::OnExitTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor,
                                        UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
	AMainCharacter* _character = Cast<AMainCharacter>(_OtherActor);

	if (_character && _character->GetIsOwner())
	{
		playerInTrigger = nullptr;
		firstSideDetected = false;
		secondSideDetected = false;
		UpdateInteractionFeedbackVisibility();
	}
}
#pragma endregion


#pragma region Acceleration
void AHandleHandcarActor::StartAcceleration(const FInputActionValue& _value)
{
	if (handcar)
	{
		if (HasAuthority())
		{
			if (firstPlayer && firstSideDetected)
				handcar->FirstPlayerStartAcceleration();
			else if (secondPlayer && secondSideDetected)
				handcar->SecondPlayerStartAcceleration();
		}
		//else
		//{
		//	if (firstPlayer && firstSideDetected)
		//		networkComponent->ServerRPC_SetFirstPlayerAccelerate(handcar, true);
		//	else if (secondPlayer && secondSideDetected)
		//		networkComponent->ServerRPC_SetSecondPlayerAccelerate(handcar, true);
		//}
	}
}

void AHandleHandcarActor::ReleaseAcceleration(const FInputActionValue& _value)
{
	if (handcar)
	{
		if (HasAuthority())
		{
			if (firstPlayer && firstSideDetected)
				handcar->FirstPlayerReleaseAcceleration();
			else if (secondPlayer && secondSideDetected)
				handcar->SecondPlayerReleaseAcceleration();
		}
		//else
		//{
		//	if (firstPlayer && firstSideDetected)
		//		networkComponent->ServerRPC_SetFirstPlayerAccelerate(handcar, false);
		//	else if (secondPlayer && secondSideDetected)
		//		networkComponent->ServerRPC_SetSecondPlayerAccelerate(handcar, false);
		//}
	}
}
#pragma endregion


void AHandleHandcarActor::UpdateHandleDetected(AMainCharacter* _player)
{
	const FVector _playerLoc = _player->GetActorLocation();

	const float _firstSideDistance = FVector::Distance(_playerLoc, firstPlayerLocation->GetComponentLocation());
	const float _secondSideDistance = FVector::Distance(_playerLoc, secondPlayerLocation->GetComponentLocation());


	firstSideDetected = _firstSideDistance <= _secondSideDistance;
	secondSideDetected = !firstSideDetected;
}

void AHandleHandcarActor::UpdateInteractionFeedbackVisibility()
{
	interactionFeedback->SetVisibility(firstSideDetected);
	secondInteractionFeedback->SetVisibility(secondSideDetected);
}
#pragma endregion


#pragma region Rotation
void AHandleHandcarActor::RotateHandle(const float _maxRotation, const float _rotationValue, const float _deltaTime)
{
	const FRotator _currentRot = handleMesh->GetRelativeRotation();

	if (FMath::Abs(_currentRot.Pitch - _maxRotation) <= 1.0f)
		return;


	float _newPitch = _currentRot.Pitch + (_rotationValue * _deltaTime);
	/*_newPitch = _maxRotation <= 0.0f && _newPitch >= _maxRotation ? _newPitch :
		_maxRotation >= 0.0f && _newPitch <= _maxRotation ? _newPitch :
		_maxRotation;*/
	//_newPitch = FMath::Abs(_newPitch - _maxRotation) <= 1.0f ? _maxRotation : _newPitch;

	if (_maxRotation <= 0.0f && _newPitch > _maxRotation)
	{
		LOG_COLOR(
			false,
			"1st Passed _newpitch => " + FString::SanitizeFloat(_newPitch) + " | maxRotation => " + FString::
			SanitizeFloat(_maxRotation), BLUE);
		_newPitch = FMath::Abs(_newPitch - _maxRotation) <= 1.0f ? _maxRotation : _newPitch;
	}
	else if (_maxRotation >= 0.0f && _newPitch <= _maxRotation)
	{
		LOG_COLOR(
			false,
			"2nd Passed _newpitch => " + FString::SanitizeFloat(_newPitch) + " | maxRotation => " + FString::
			SanitizeFloat(_maxRotation), ORANGE);
		_newPitch = FMath::Abs(_newPitch - _maxRotation) <= 1.0f ? _maxRotation : _newPitch;
	}
	else
	{
		LOG_COLOR(
			false,
			"3th Passed _newpitch => " + FString::SanitizeFloat(_newPitch) + " | maxRotation => " + FString::
			SanitizeFloat(_maxRotation), PINK);
		_newPitch = _maxRotation;
	}


	handleMesh->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
}

void AHandleHandcarActor::UpdateHandleHeightRapports(const bool _firstPlayerAccelerates,
                                                     const bool _secondPlayerAccelerates)
{
	const FRotator _currentRot = handleMesh->GetRelativeRotation();
	const float _pitch = _currentRot.Pitch > 45 ? GetClampPitch(_currentRot.Pitch) : _currentRot.Pitch;

	const float _angleScope = maxRotation * 2.0f;
	const float _heightValueOnScope = 1.0f - (FMath::Abs(_pitch + maxRotation) / _angleScope);

	firstPlayerLastHeightRapport = firstPlayerHeightRapport;
	secondPlayerLastHeightRapport = secondPlayerHeightRapport;

	LOG_COLOR_TICK(false, "pitch => " + FString::SanitizeFloat(_pitch), YELLOW);
	LOG_COLOR_TICK(false, "last First => " + FString::SanitizeFloat(firstPlayerLastHeightRapport), PINK);
	LOG_COLOR_TICK(false, "last Second => " + FString::SanitizeFloat(secondPlayerLastHeightRapport), PINK);

	firstPlayerHeightRapport = _heightValueOnScope;
	secondPlayerHeightRapport = 1.0f - firstPlayerHeightRapport;
}
#pragma endregion


float AHandleHandcarActor::GetClampPitch(const float _pitch)
{
	float _ratio = _pitch / maxRotation;
	const float _floor = FMath::Floor(_ratio);
	if ((_ratio / 2) == FMath::Floor(_floor / 2))
		return _pitch;
	else
		return -(maxRotation * (1 - (_ratio - FMath::Floor(_ratio))));
}

void AHandleHandcarActor::BeginPlay()
{
	Super::BeginPlay();

	if (secondInteractionFeedback)
		secondInteractionFeedback->SetVisibility(false);

	if (trigger)
	{
		trigger->OnComponentBeginOverlap.AddDynamic(this, &AHandleHandcarActor::OnEnterTrigger);
		trigger->OnComponentEndOverlap.AddDynamic(this, &AHandleHandcarActor::OnExitTrigger);
	}

	networkComponent = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>();
	CHECK_NULL(networkComponent, "netWorkComponent is null!!");
}

void AHandleHandcarActor::TickUpdateInteractionFeedback()
{
	if (!isDetected)
		return;

	if (firstSideDetected)
		UpdateInteractionFeedback(interactionFeedback);
	else if (secondSideDetected)
		UpdateInteractionFeedback(secondInteractionFeedback);
}


void AHandleHandcarActor::Interacted(AMainCharacter* _character)
{
	if (firstSideDetected || (firstPlayer && firstPlayer == _character))
		FirstPlayerInteraction(firstPlayerLocation->GetComponentLocation(), _character);
	else if (secondSideDetected || (secondPlayer && secondPlayer == _character))
		SecondPlayerInteraction(secondPlayerLocation->GetComponentLocation(), _character);
}

void AHandleHandcarActor::InHandcarTick(const bool _firstPlayerAccelerates, const bool _secondPlayerAccelerates,
                                        const float _deltaTime)
{
	if (isRuntime)
	{
		if (playerInTrigger)
		{
			UpdateHandleDetected(playerInTrigger);
			UpdateInteractionFeedbackVisibility();
		}

		if (HasAuthority())
		{
			if (_firstPlayerAccelerates && !_secondPlayerAccelerates)
				RotateHandle(-maxRotation, -rotationValue, _deltaTime);
			else if (_secondPlayerAccelerates && !_firstPlayerAccelerates)
				RotateHandle(maxRotation, rotationValue, _deltaTime);
			else
			{
				const FRotator _currentRot = handleMesh->GetRelativeRotation();
				const float _currentPitch = _currentRot.Pitch;
				RotateHandle(0.0f, _currentPitch > 0.0f ? -rotationValue : rotationValue, _deltaTime);
			}

			LOG_COLOR_TICK(true, "Pitch => " + FString::SanitizeFloat(handleMesh->GetRelativeRotation().Pitch), YELLOW);
			UpdateHandleHeightRapports(_firstPlayerAccelerates, _secondPlayerAccelerates);
		}
	}
}


void AHandleHandcarActor::FirstPlayerInteraction(const FVector& _newPlayerLoc, AMainCharacter* _playerInteracting)
{
	UInteractionComponent* _interactionComponent;

	if (firstPlayer)
	{
		if (firstPlayer == _playerInteracting)
		{
			_interactionComponent = firstPlayer->GetInteractionComponent();

			if (_interactionComponent)
				_interactionComponent->SetIsInInteraction(false);

			firstPlayer->SetMeshRotation(firstPlayer->GetMesh()->GetComponentRotation(), true);

			//============Important=====
			//firstPlayer->SetIsUsingHandleHandcar(false);

			if (HasAuthority())
			{
				if (handcar)
					handcar->SetPlayerAccelerates(true, false);
				firstPlayer = nullptr;
			}
			//else
			//	networkComponent->ServerRPC_SetFirstPlayerExit(this, nullptr);
		}
	}
	else
	{
		if (HasAuthority())
		{
			firstPlayer = _playerInteracting;
			//============Important=====
			//firstPlayer->SetIsUsingHandleHandcar(true);
			_interactionComponent = firstPlayer->GetInteractionComponent();

			if (_interactionComponent)
				_interactionComponent->SetIsInInteraction(true);

			SetupPlayerInputs(firstPlayer);
			firstPlayer->GetCharacterMovement()->Velocity = FVector(0);
			firstPlayer->SetActorLocation(_newPlayerLoc);
			const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_newPlayerLoc, GetActorLocation());
			firstPlayer->SetMeshRotation(_lookAt, false);
		}
		//else
		//{
		//	networkComponent->ServerRPC_SetFirstPlayerEnter(this, _playerInteracting, _newPlayerLoc);
		//}
	}
}

void AHandleHandcarActor::SecondPlayerInteraction(const FVector& _newPlayerLoc, AMainCharacter* _playerInteracting)
{
	UInteractionComponent* _interactionComponent = nullptr;

	if (secondPlayer)
	{
		if (secondPlayer == _playerInteracting)
		{
			_interactionComponent = secondPlayer->GetInteractionComponent();
			if (_interactionComponent)
				_interactionComponent->SetIsInInteraction(false);

			secondPlayer->SetMeshRotation(secondPlayer->GetMesh()->GetComponentRotation(), true);

			//============Important=====
			//secondPlayer->SetIsUsingHandleHandcar(false);

			if (HasAuthority())
			{
				if (handcar)
					handcar->SetPlayerAccelerates(false, false);

				secondPlayer = nullptr;
			}
			//else
			//	networkComponent->ServerRPC_SetSecondPlayerExit(this, nullptr);
		}
	}
	else
	{
		if (HasAuthority())
		{
			secondPlayer = _playerInteracting;
			//============Important=====
			//secondPlayer->SetIsUsingHandleHandcar(true);
			_interactionComponent = secondPlayer->GetInteractionComponent();

			if (_interactionComponent)
				_interactionComponent->SetIsInInteraction(true);

			SetupPlayerInputs(secondPlayer);
			secondPlayer->GetCharacterMovement()->Velocity = FVector(0);
			secondPlayer->SetActorLocation(_newPlayerLoc);
			const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_newPlayerLoc, GetActorLocation());
			secondPlayer->SetMeshRotation(_lookAt, false);
		}
		//else
		//	networkComponent->ServerRPC_SetSecondPlayerEnter(this, _playerInteracting, _newPlayerLoc);
	}
}

void AHandleHandcarActor::UpdateValuesFromDataAsset(UHandcarDataAsset* _da)
{
	if (_da)
	{
		maxRotation = _da->GetHandleMaxRotation();
		rotationValue = _da->GetHandleRotationValue();
	}
}

#pragma region Network
void AHandleHandcarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHandleHandcarActor, firstPlayer);
	DOREPLIFETIME(AHandleHandcarActor, secondPlayer);
	DOREPLIFETIME(AHandleHandcarActor, firstPlayerHeightRapport);
	DOREPLIFETIME(AHandleHandcarActor, secondPlayerHeightRapport);
	DOREPLIFETIME(AHandleHandcarActor, firstPlayerLastHeightRapport);
	DOREPLIFETIME(AHandleHandcarActor, secondPlayerLastHeightRapport);
}

void AHandleHandcarActor::OnRep_FirstPlayerUpdate()
{
	if (firstPlayer && firstPlayer->GetIsOwner())
	{
		//============Important=====
		//firstPlayer->SetIsUsingHandleHandcar(true);

		UInteractionComponent* _interactionComponent = firstPlayer->GetInteractionComponent();

		if (_interactionComponent)
			_interactionComponent->SetIsInInteraction(true);

		SetupPlayerInputs(firstPlayer);

		FVector _playerLoc = firstPlayer->GetActorLocation();
		_playerLoc.Z = GetActorLocation().Z;
		const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_playerLoc, GetActorLocation());
		firstPlayer->SetMeshRotation(_lookAt, false);
	}
}

void AHandleHandcarActor::OnRep_SecondPlayerUpdate()
{
	if (secondPlayer && secondPlayer->GetIsOwner())
	{
		//============Important=====
		//secondPlayer->SetIsUsingHandleHandcar(true);

		UInteractionComponent* _interactionComponent = secondPlayer->GetInteractionComponent();

		if (_interactionComponent)
			_interactionComponent->SetIsInInteraction(true);

		SetupPlayerInputs(secondPlayer);

		FVector _playerLoc = secondPlayer->GetActorLocation();
		_playerLoc.Z = GetActorLocation().Z;
		const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_playerLoc, GetActorLocation());
		secondPlayer->SetMeshRotation(_lookAt, false);
	}
}
#pragma endregion
