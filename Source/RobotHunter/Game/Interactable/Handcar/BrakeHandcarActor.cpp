#include "BrakeHandcarActor.h"
#include "../../GPE/Handcar/CustomHandcarActor.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include <Net/UnrealNetwork.h>
#include "EnhancedInputComponent.h"

ABrakeHandcarActor::ABrakeHandcarActor()
{
#pragma region Position
	baseLever = CreateDefaultSubobject<USceneComponent>("BaseLever");
	baseLever->SetupAttachment(mesh);

	leverMesh = CreateDefaultSubobject<UStaticMeshComponent>("LeverMesh");
	leverMesh->SetupAttachment(baseLever);

#pragma region Angle
	tightenedAngle = 45.0f;
	releasedAngle = 135.0f;
#pragma endregion

#pragma region Speed
	/*minInteractionValue = 7.0f;
	maxInteractionValue = 10.0f;
	releasedSpeed = 20.0f;*/
	speedPercent = 0.1f;
#pragma endregion

	isReleased = false;
	positionMultiplier = 1.0f;
#pragma endregion

	brakeValue = -5.0f;

#pragma region Handcar
	handcar = nullptr;

	handcarMaxSpeed = 0.0f;
	speedMultiplier = 0.0f;
	//startingCoefficientFriction = 0.0f;
#pragma endregion

	player = nullptr;

	//inputsSetup = false;

	/*bReplicates = true;
	SetReplicateMovement(true);*/
}

void ABrakeHandcarActor::TightenRotationLever(const FInputActionValue& _value)
{
	/*if (HasAuthority())
	{
		const FRotator _currentRot = baseLever->GetRelativeRotation();
		const float _interactionValue = FMath::Lerp(minInteractionValue, maxInteractionValue, GetPositionOnAngleScope());
		const float _newPitch = _currentRot.Pitch + _interactionValue;

		if (_newPitch > tightenedAngle)
		{
			baseLever->SetRelativeRotation(FRotator(tightenedAngle, _currentRot.Yaw, _currentRot.Roll));
			isReleased = false;
			return;
		}
		
		baseLever->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
	}
	else
		networkComponent->ServerRpc_TightenRotationLever(this, 0, FRotator::ZeroRotator);*/
}

//void ABrakeHandcarActor::SetupPlayerInputs(AMainCharacter* _player)
//{
//	if (inputsSetup)
//		return;
//
//	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
//	UInputConfig* _inputConfig = _player->GetInputConfig();
//
//	if (_input && _inputConfig)
//	{
//		/*_input->BindAction(_inputConfig->GetHandcarBrakeInput(), ETriggerEvent::Started, this, &ABrakeHandcarActor::TightenRotationLever);
//		_input->BindAction(_inputConfig->GetHandcarReleaseBrakeInput(), ETriggerEvent::Started, this, &ABrakeHandcarActor::ReleaseLever);*/
//		inputsSetup = true;
//	}
//}

#pragma region Position
//void ABrakeHandcarActor::ReleaseLever(const FInputActionValue& _value)
//{
//	if (HasAuthority())
//		isReleased = true;
//	else
//		networkComponent->ServerRpc_SetReleasedBrakeLever(this, true);
//}
//
//
//void ABrakeHandcarActor::ReleaseRotationLever(const float _deltaTime)
//{
//	const FRotator _currentRot = baseLever->GetRelativeRotation();
//	const float _releasedAngle = -(180 - releasedAngle);
//
//	if (FMath::Abs(_currentRot.Pitch - _releasedAngle) <= 1.0f)
//	{
//		baseLever->SetRelativeRotation(FRotator(_releasedAngle, _currentRot.Yaw, _currentRot.Roll));
//		return;
//	}
//
//	float _newPitch = _currentRot.Pitch - (releasedSpeed /** positionMultiplier*/ * _deltaTime);
//	_newPitch = _releasedAngle <= 0.0f && _newPitch >= _releasedAngle ? _newPitch :
//		_currentRot.Pitch;
//
//	baseLever->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
//}

void ABrakeHandcarActor::InstantBrake(const bool _isReleased)
{
	const FRotator _currentRot = baseLever->GetRelativeRotation();
	float _newPitch = 0.0f;

	if (_isReleased)
		_newPitch = -(180 - releasedAngle);
	else
		_newPitch = tightenedAngle;

	baseLever->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
	UpdatePositionMultiplier();
}

void ABrakeHandcarActor::UpdatePositionMultiplier()
{
	positionMultiplier = FMath::Lerp(1.0f, 0.0f, GetPositionOnAngleScope());
}

float ABrakeHandcarActor::GetPositionOnAngleScope()
{
	const FRotator _currentRot = baseLever->GetRelativeRotation();
	//LOG_COLOR(true, "currentRot => " + _currentRot.ToString(), YELLOW);
	const float _angleScope = releasedAngle - tightenedAngle;
	const float _positionOnScope = FMath::Abs(_currentRot.Pitch - tightenedAngle) / _angleScope;
	return _positionOnScope;
}
#pragma endregion

//void ABrakeHandcarActor::UpdateSpeedMultiplier(const float _handcarCurrentSpeed)
//{
//	const float _speedRapport = _handcarCurrentSpeed / handcarMaxSpeed;
//	speedMultiplier = FMath::Lerp(1.0f, 0.1f, _speedRapport);
//}

//void ABrakeHandcarActor::UpdateHandcarCoefficientFriction()
//{
//	if (!handcar)
//		return;
//
//	const float _newCoefficient = FMath::Lerp(1.0f, startingCoefficientFriction, GetPositionOnAngleScope());
//	handcar->SetCoefficientFriction(_newCoefficient);
//}

void ABrakeHandcarActor::BeginPlay()
{
	Super::BeginPlay();

	canBeDetected = false;

	if (baseLever)
		baseLever->SetWorldRotation(FRotator(tightenedAngle, 0.0f, 0.0f));

	networkComponent = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>();
}

void ABrakeHandcarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABrakeHandcarActor, isReleased);
	DOREPLIFETIME(ABrakeHandcarActor, positionMultiplier);
}

void ABrakeHandcarActor::Interacted(AMainCharacter* _character)
{
	player = _character;

	//============Important=====
	//_character->SetIsUsingBrakeHandcar(true);
	//===============================


	/*if (player && player != _character)
		return;

	Super::Interacted(_character);
	UInteractionComponent* _interactionComponent = nullptr;

	if (firstInteraction)
	{
		_interactionComponent = _character->GetInteractionComponent();

		if (_interactionComponent)
			_interactionComponent->SetIsInInteraction(true);

		firstInteraction = false;
		SetupPlayerInputs(_character);

		if (HasAuthority())
		else
			networkComponent->ServerRpc_SetBrakePlayer(this, _character);
	}
	else
	{
		_interactionComponent = _character->GetInteractionComponent();

		if (_interactionComponent)
			_interactionComponent->SetIsInInteraction(false);

		_character->SetIsUsingBrakeHandcar(false);
		firstInteraction = true;

		if (HasAuthority())
			player = nullptr;
		else
			networkComponent->ServerRpc_SetBrakePlayer(this, nullptr);
	}*/

	if (HasAuthority())
	{
		if (handcar)
			handcar->SetInAutoMode(false);

		isReleased = !isReleased;
		InstantBrake(isReleased);

		onReleaseBrake.Broadcast(isReleased);
		_character->GetCharaAnim()->SetIsUsingBrake(isReleased); 
	}
	else
		networkComponent->ServerRpc_SetReleasedBrakeLever(this, !isReleased);
}

//void ABrakeHandcarActor::InHandcarTick(const float _deltaTime, const float _handcarCurrentSpeed)
//{
//	if (isRuntime)
//	{
//		UpdateSpeedMultiplier(_handcarCurrentSpeed);
//		//UpdateHandcarCoefficientFriction();
//		if (HasAuthority())
//		{
//			UpdatePositionMultiplier();
//
//			/*if (isReleased)
//				ReleaseRotationLever(_deltaTime);*/
//		}
//	}
//}

float ABrakeHandcarActor::GetUpdatedBrakeValue(const float _handcarCurrentSpeed)
{
	const float _brakeValue = _handcarCurrentSpeed >= 0.0f ? brakeValue : -brakeValue;
	const float _brakeValueFromPos = (_brakeValue - (_handcarCurrentSpeed * speedPercent)) * positionMultiplier;
	//LOG_COLOR(true, "brakeValueFromPos => " + FString::SanitizeFloat(_brakeValueFromPos), BLUE);
	return _brakeValueFromPos;
}

void ABrakeHandcarActor::UpdateValuesFromDataAsset(UHandcarDataAsset* _da)
{
	if (_da)
	{
		releasedAngle = _da->GetBrakeAngle(true);
		tightenedAngle = _da->GetBrakeAngle(false);

		/*minInteractionValue = _da->GetBrakeInteractionValue(true);
		maxInteractionValue = _da->GetBrakeInteractionValue(false);
		releasedSpeed = _da->GetBrakeReleasedSpeed();*/
		speedPercent = _da->GetBrakeSpeedPercent();

		brakeValue = _da->GetBrakeValue();
	}
}

void ABrakeHandcarActor::OnRep_UpdateIsReleased()
{
	onReleaseBrake.Broadcast(isReleased);
	if (player)
		onInteract.Broadcast(player);
	//============Important=====
	//player->SetIsUsingBrakeHandcar(false);
}
