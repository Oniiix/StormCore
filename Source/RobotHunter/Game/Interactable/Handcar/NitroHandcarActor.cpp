#include "NitroHandcarActor.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../UI/HUD/MainHUD.h"
#include "../../../UI/ATH/Draisine/DraisineATH.h"
#include "../../GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "RobotHunter/Game/GPE/Transmitter/TargetActor/RailsTargetActor.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"

ANitroHandcarActor::ANitroHandcarActor()
{
	/*valveMesh = CreateDefaultSubobject<UStaticMeshComponent>("ValveMesh");
	valveMesh->SetGenerateOverlapEvents(true);
	valveMesh->SetupAttachment(mesh);*/

	driverSeat = CreateDefaultSubobject<USkeletalMeshComponent>("DriverSeat");
	driverSeat->SetupAttachment(mesh);
	nitroAcceleration = 5.0f;
	//cursorSpeed = 5.0f;

#pragma region Zone
	/*greenZoneSize = 10.0f;
	yellowZoneSize = 40.0f;*/
#pragma endregion

#pragma region Movement

#pragma region Angle
	//minMovementAngle = -50.0f;
	//maxMovementAngle = 50.0f;

	//greenInitialAngularOffset = 0.0f;
	//yellowInitialAngularOffset = 0.0f;
#pragma endregion

#pragma region Speed
	/*minMovementSpeed = 5.0f;
	maxMovementSpeed = 10.0f;*/
#pragma endregion

#pragma region Time
	/*minMovementTime = 2.0f;
	maxMovementTime = 5.0f;*/
#pragma endregion

#pragma endregion

#pragma region Life
	/*greenLifeConsumption = 5.0f;
	yellowLifeConsumption = 10.0f;
	redLifeConsumption = 15.0f;*/
#pragma endregion

#pragma region Handcar
	handcar = nullptr;
#pragma endregion

	player = nullptr;

	//ath = nullptr;

	/*nitroIsActive = false;
	slidersAreMoving = false;*/
	inputsSetup = false;
	//isInitialized = false;

	/*currentSpeed = 0.0f;
	targetAngle = 0.0f;
	currentSliderAngle = 0.0f;
	currentCursorAngle = 30.0f;
	currentLifeConsumption = 0.0f;*/

	canChangeHandcarDirection = true;
	useNegativeNitro = false;
	updatedNitroAcceleration = 0.0f;

	//movementTimer = FTimerHandle();
}

void ANitroHandcarActor::SetupPlayerInputs(AMainCharacter* _player)
{
	if (inputsSetup)
		return;

	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
	UInputConfig* _inputConfig = _player->GetInputConfig();


	if (_input && _inputConfig)
	{
		/*_input->BindAction(_inputConfig->GetHandcarNitroRightInput(), ETriggerEvent::Triggered, this, &ANitroHandcarActor::MoveRightCursor);
		_input->BindAction(_inputConfig->GetHandcarNitroLeftInput(), ETriggerEvent::Triggered, this, &ANitroHandcarActor::MoveLeftCursor);*/
		_input->BindAction(_inputConfig->GetHandcarNitroRightInput(), ETriggerEvent::Triggered, this,
		                   &ANitroHandcarActor::UpdateNitroAcceleration);
		_input->BindAction(_inputConfig->GetHandcarNitroRightInput(), ETriggerEvent::Completed, this,
		                   &ANitroHandcarActor::UpdateNitroAcceleration);

		_input->BindAction(_inputConfig->GetHandcarNitroLeftInput(), ETriggerEvent::Triggered, this,
		                   &ANitroHandcarActor::ChangeHandcarDirection);
		_input->BindAction(_inputConfig->GetHandcarNitroLeftInput(), ETriggerEvent::Completed, this,
		                   &ANitroHandcarActor::ResetCanChangeHandcarDirection);

		inputsSetup = true;
	}

	_player->OnChangeMappingContext().AddUniqueDynamic(this, &ANitroHandcarActor::PauseHandcar);
}

void ANitroHandcarActor::PauseHandcar(UInputMappingContext* _current)
{
	if (!player || _current == player->GetInputConfig()->GetNitroHandcarContextInput()) return;

	updatedNitroAcceleration = 0.0f;

	if (!HasAuthority())
		networkComponent->ServerRpc_SetNitroAcceleration(this, updatedNitroAcceleration);
}

//void ANitroHandcarActor::GetATH()
//{
//	AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
//
//	if (_hud)
//	{
//		UDraisineATH* _ath = Cast<UDraisineATH>(_hud->GetUI(EStateUI::DraisineUI));
//
//		if (_ath)
//			ath = _ath;
//	}
//}
//
//void ANitroHandcarActor::UpdateCursorColor()
//{
//	if (!ath)
//		GetATH();
//
//	if (ath)
//	{
//		const float _cursorValue = ath->GetProgressAngularOffset();
//
//		if (CheckCursorIsInZone(_cursorValue, ESliderColor::GreenSlider))
//		{
//			ath->SetProgressColor(ESliderColor::GreenSlider);
//			nitroIsActive = true;
//			currentLifeConsumption = greenLifeConsumption;
//		}
//		else if (CheckCursorIsInZone(_cursorValue, ESliderColor::YellowSlider))
//		{
//			ath->SetProgressColor(ESliderColor::YellowSlider);
//			nitroIsActive = true;
//			currentLifeConsumption = yellowLifeConsumption;
//		}
//		else if (CheckCursorIsInZone(_cursorValue, ESliderColor::RedSlider))
//		{
//			ath->SetProgressColor(ESliderColor::RedSlider);
//			nitroIsActive = true;
//			currentLifeConsumption = redLifeConsumption;
//		}
//		else if (CheckCursorIsInZone(_cursorValue, ESliderColor::BlackSlider))
//		{
//			ath->SetProgressColor(ESliderColor::BlackSlider);
//			nitroIsActive = false;
//			currentLifeConsumption = 0.0f;
//		}
//	}
//}
//
//bool ANitroHandcarActor::CheckCursorIsInZone(const float _cursorValue, const ESliderColor& _color)
//{
//	if (!ath)
//		GetATH();
//
//	if (ath)
//	{
//		const float _zoneSize = ath->GetMaxSlider(_color) - ath->GetMinSlider(_color);
//		const float _zoneAngle = ath->GetSliderAngularOffset(_color);
//
//		const float _zoneStart = ath->GetMinSlider(_color) + _zoneAngle;
//		const float _zoneEnd = _zoneStart + _zoneSize;
//
//		if (_cursorValue >= _zoneStart && _cursorValue <= _zoneEnd)
//			return true;
//	}
//
//	return false;
//}
//
//void ANitroHandcarActor::InitializeSliders()
//{
//	if (!ath)
//		GetATH();
//
//	if (ath)
//	{
//		const float _minSlider = ath->GetMinSlider(ESliderColor::BlackSlider);
//		const float _maxSlider = ath->GetMaxSlider(ESliderColor::BlackSlider);
//		const float _totalZoneSize = _maxSlider - _minSlider;
//
//		const float _greenZoneSize = FMath::CeilToFloat(_totalZoneSize * (greenZoneSize / 100.0f));
//		const float _yellowZoneSize = FMath::CeilToFloat(_totalZoneSize * (yellowZoneSize / 100.0f));
//
//		ath->SetAngle(ESliderColor::GreenSlider, false, 0.0f, _greenZoneSize);
//		ath->SetAngle(ESliderColor::YellowSlider, false, 0.0f, _yellowZoneSize);
//
//		greenInitialAngularOffset = 180.0f - (_greenZoneSize / 2.0f);
//		yellowInitialAngularOffset = 180.0f - (_yellowZoneSize / 2.0f);
//
//		ath->SetRotationAngle(ESliderColor::GreenSlider, greenInitialAngularOffset);
//		ath->SetRotationAngle(ESliderColor::YellowSlider, yellowInitialAngularOffset);
//
//		if (HasAuthority())
//		{
//			currentSpeed = FMath::FRandRange(minMovementSpeed, maxMovementSpeed);
//			targetAngle = FMath::FRandRange(minMovementAngle, maxMovementAngle);
//			GetWorld()->GetTimerManager().SetTimer(movementTimer, this, &ANitroHandcarActor::StartMoveSliders, FMath::FRandRange(minMovementTime, maxMovementTime), false);
//
//		}
//
//		if (valveMesh)
//			valveMesh->SetWorldRotation(FRotator(0.0f, 0.0f, currentCursorAngle));
//
//		isInitialized = true;
//	}
//}
//
//void ANitroHandcarActor::StartMoveSliders()
//{
//		slidersAreMoving = true;
//}
//
//void ANitroHandcarActor::MoveSliders(const float _deltaTime)
//{
//	if (slidersAreMoving)
//	{
//		currentSliderAngle += targetAngle < currentSliderAngle ? -currentSpeed * _deltaTime : currentSpeed * _deltaTime;
//
//		if (FMath::Abs(targetAngle - currentSliderAngle) <= 1.0f)
//		{
//			currentSliderAngle = targetAngle;
//			slidersAreMoving = false;
//
//			currentSpeed = FMath::FRandRange(minMovementSpeed, maxMovementSpeed);
//			targetAngle = FMath::FRandRange(minMovementAngle, maxMovementAngle);
//			GetWorld()->GetTimerManager().SetTimer(movementTimer, this, &ANitroHandcarActor::StartMoveSliders, FMath::FRandRange(minMovementTime, maxMovementTime), false);
//		}
//	}
//
//	if (!ath)
//		GetATH();
//
//	if (ath)
//	{
//		ath->SetRotationAngle(ESliderColor::GreenSlider, greenInitialAngularOffset + currentSliderAngle);
//		ath->SetRotationAngle(ESliderColor::YellowSlider, yellowInitialAngularOffset + currentSliderAngle);
//	}
//}
//
//void ANitroHandcarActor::MoveCursor(const float _inputValue)
//{
//	if (!ath)
//		GetATH();
//
//	if (ath)
//	{
//		ath->SetProgressAngularOffset(currentCursorAngle);
//
//		if (_inputValue != 0.0f)
//		{
//			currentCursorAngle = ath->GetProgressAngularOffset() + ((cursorSpeed / 10.0f) * _inputValue);
//			ath->SetProgressAngularOffset(currentCursorAngle);
//		}
//
//		if (valveMesh)
//			valveMesh->SetWorldRotation(FRotator(0.0f, 0.0f, currentCursorAngle));
//	}
//
//}
//
//void ANitroHandcarActor::MoveRightCursor(const FInputActionValue& _value)
//{
//	const float _inputValue = _value.Get<float>();
//	MoveCursor(_inputValue);
//}
//
//void ANitroHandcarActor::MoveLeftCursor(const FInputActionValue& _value)
//{
//	const float _inputValue = _value.Get<float>();
//	MoveCursor(_inputValue);
//}

void ANitroHandcarActor::UpdateNitroAcceleration(const FInputActionValue& _value)
{
	if (!player || player->GetInPause())
		return;

	const float _inputValue = _value.Get<float>();
	updatedNitroAcceleration = nitroAcceleration * _inputValue;

	if (!HasAuthority())
		networkComponent->ServerRpc_SetNitroAcceleration(this, updatedNitroAcceleration);

	if (handcar)
		handcar->SetInAutoMode(false);
}

void ANitroHandcarActor::ChangeHandcarDirection(const FInputActionValue& _value)
{
	if (!canChangeHandcarDirection)
		return;

	if (player && handcar)
	{
		ARailsTargetActor* _currentTarget = handcar->GetCurrentTarget();

		if (_currentTarget)
		{
			const float _inputValue = _value.Get<float>();

			if (_inputValue > 0.5f)
			{
				canChangeHandcarDirection = false;
				_currentTarget->UpdateRailIsBlocked(true, true, false);
			}
			else if (_inputValue < -0.5f)
			{
				canChangeHandcarDirection = false;
				_currentTarget->UpdateRailIsBlocked(false, true, false);
			}

			if (!canChangeHandcarDirection)
			{
				if (handcar)
				{
					_currentTarget->CheckRailsOnHandcarPath(handcar);
					handcar->SetInAutoMode(false);
				}
			}
		}
	}
}

void ANitroHandcarActor::ResetCanChangeHandcarDirection(const FInputActionValue& _value)
{
	canChangeHandcarDirection = true;
}

void ANitroHandcarActor::OnRep_UpdatePlayer(AMainCharacter* _oldCharacter)
{
	if (player)
	{
		player->SetIsUsingNitroHandcar(true);
		onUseControls.Broadcast(true);
		player->AttachToComponent(driverSeat, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "DS_Socket");
		player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		updatedNitroAcceleration = 0.0f;
		onUseControls.Broadcast(false);
		CHECK_NULL(_oldCharacter, "OnRep_UpdatePlayer => _oldPlayer is null")
		_oldCharacter->SetIsUsingNitroHandcar(false);
		_oldCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		_oldCharacter->AttachToComponent(handcar->GetHandcarMesh(), FAttachmentTransformRules::KeepWorldTransform);
		_oldCharacter->SetMeshRotation(FRotator(0, 0, 0), false, false);
		_oldCharacter->SetCanTurn(true);
	}
}

void ANitroHandcarActor::BeginPlay()
{
	Super::BeginPlay();

	networkComponent = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>();

	/*FTimerHandle _initializeTimer;
	GetWorld()->GetTimerManager().SetTimer(_initializeTimer, this, &ANitroHandcarActor::InitializeSliders, 2.0f, false);*/
}

void ANitroHandcarActor::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	if (player)
	{
		UCharaAnim* _anim = player->GetCharaAnim();

		if (_anim)
			_anim->SetHandcarSpeed(handcar->GetCurrentSpeed());
	}
}

void ANitroHandcarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANitroHandcarActor, player)
	DOREPLIFETIME(ANitroHandcarActor, updatedNitroAcceleration)
}

void ANitroHandcarActor::Interacted(AMainCharacter* _character)
{
	if (handcar)
		handcar->SetInAutoMode(false);

	if (player && player != _character)
		return;

	//Super::Interacted(_character);
	UInteractionComponent* _interactionComponent = _character->GetInteractionComponent();
	//UCustomCameraComponent* _camComp = _character->GetCameraComponent();

	if (driverSeat)
		[[maybe_unused]] FRotator _seatRot = driverSeat->GetSocketRotation("DS_Socket");

	if (!player)
	{
		if (HasAuthority())
		{
			if (handcar)
			{
				ABrakeHandcarActor* _brake = handcar->GetBrakeActor();

				if (_brake)
				{
					_brake->SetReleased(true);
					_brake->InstantBrake(true);
				}
			}


			if (_character)
			{
				_character->SetMeshRotation(FRotator(0.0f), false, false);
				_character->SetCanTurn(true);
				_character->SetIsUsingNitroHandcar(true);
				onUseControls.Broadcast(true);

				_character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

				UCustomCameraComponent* _camera = _character->GetCameraComponent();

				if (_camera)
				{
					_camera->GetSpringArm()->SetRelativeRotation(FRotator(0.0f, handcar->GetActorRotation().Yaw, 0.0f));
					_camera->DesynchRoll(true);
					_camera->DesynchPitch(true);
					_camera->DesynchYaw(true);
				}

				_character->SetCanMove(false);
				//_character->SetCanJump(false);
				_character->GetCustomMovementComponent()->Velocity = FVector(0.0f);

				if (driverSeat)
				{
					_character->AttachToComponent(driverSeat, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					                              "DS_Socket");
				}
			}


			player = _character;
		}
		else
			networkComponent->ServerRpc_SetNitroPlayer(this, _character);

		if (_interactionComponent)
			_interactionComponent->SetIsInInteraction(true);

		/*if (_camComp)
			_camComp->DesynchYaw(true);*/

		SetupPlayerInputs(_character);

		if (AGameHUD* _hud = Cast<AGameHUD>(GETFPC->GetHUD()))
		{
			_hud->SetState(EStateUI::DraisineUI);
		}
	}
	else
	{
		if (HasAuthority())
		{
			if (handcar)
			{
				ABrakeHandcarActor* _brake = handcar->GetBrakeActor();

				if (_brake)
				{
					_brake->SetReleased(false);
					_brake->InstantBrake(false);
				}
			}

			_character->SetIsUsingNitroHandcar(false);
			updatedNitroAcceleration = 0.0f;
			onUseControls.Broadcast(false);
			_character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

			if (handcar)
				_character->AttachToComponent(handcar->GetHandcarMesh(), FAttachmentTransformRules::KeepWorldTransform);

			const FRotator _playerRot = _character->GetActorRotation();
			_character->SetActorRotation(FRotator(0.0f, _playerRot.Yaw, 0.0f));
			player->SetCanMove(true);
			//player->SetCanJump(false);

			UCustomCameraComponent* _camera = _character->GetCameraComponent();

			if (_camera)
			{
				_camera->DesynchRoll(false);
				_camera->DesynchPitch(false);
				_camera->DesynchYaw(false);
				_camera->GetSpringArm()->SetRelativeRotation(FRotator(0.0f, _camera->GetSpringArm()->GetRelativeRotation().Yaw - player->GetActorRotation().Yaw, 0.0f));
			}

			player = nullptr;
			if (AGameHUD* _hud = Cast<AGameHUD>(GETFPC->GetHUD()))
			{
				_hud->SetState(EStateUI::PlayerGameUI);
			}
		}
		else
			networkComponent->ServerRpc_SetNitroPlayer(this, nullptr);

		if (_interactionComponent)
			_interactionComponent->SetIsInInteraction(false);

		/*if (_camComp)
			_camComp->DesynchYaw(false);*/
	}
}

//void ANitroHandcarActor::InHandcarTick(const float _deltaTime)
//{
//	if (isRuntime)
//	{
//		if (!isInitialized)
//			return;
//
//		MoveSliders(_deltaTime);
//		UpdateCursorColor();
//	}
//}

void ANitroHandcarActor::UpdateValuesFromDataAsset(UHandcarDataAsset* _da)
{
	if (_da)
	{
		nitroAcceleration = _da->GetNitroAcceleration();
		/*cursorSpeed = _da->GetCursorSpeed();

		greenZoneSize = _da->GetZoneSize(ESliderColor::GreenSlider);
		yellowZoneSize = _da->GetZoneSize(ESliderColor::YellowSlider);

		minMovementAngle = _da->GetNitroMovementAngle(true);
		maxMovementAngle = _da->GetNitroMovementAngle(false);

		minMovementSpeed = _da->GetNitroMovementSpeed(true);
		maxMovementSpeed = _da->GetNitroMovementSpeed(false);

		minMovementTime = _da->GetNitroMovementTime(true);
		maxMovementTime = _da->GetNitroMovementTime(false);

		greenLifeConsumption = _da->GetLifeConsumption(ESliderColor::GreenSlider);
		yellowLifeConsumption = _da->GetLifeConsumption(ESliderColor::YellowSlider);
		redLifeConsumption = _da->GetLifeConsumption(ESliderColor::RedSlider);*/
	}
}

float ANitroHandcarActor::GetUpdatedNitroAcceleration() const
{
	if (handcar)
	{
		if (handcar->GetInAutoMode())
			return useNegativeNitro ? -nitroAcceleration * 1.0f : nitroAcceleration * 1.0f;
	}

	return updatedNitroAcceleration;
}
