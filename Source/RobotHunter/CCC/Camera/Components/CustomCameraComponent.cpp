#include "CustomCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/SaveSystem/SettingsSave.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	currentSettingsKey = FString();
	//allSettings = TMap<FString, FCameraSettings>();

	useLerp = false;
	/*isLerpingLag = false;
	isLerpingFOV = false;
	isLerpingOffset = false;
	isLerpingDistance = false;
	isLerpingPitch = false;
	isLerpingYaw = false;*/

	blendCurve = nullptr;

	lastSettingsKey = FString();
	currentSettings = FCameraSettings();

	offsetLerp = FCameraLerp();
	distanceLerp = FCameraLerp();

	onPitchCameraLerpReturn = false;
	pitchCameraLerp = FCameraLerp();
}

//Add by Romain
void UCustomCameraComponent::SetFOV(const float _fov)
{
	currentSettings.fieldOfView = _fov;
	camera->SetFieldOfView(_fov);
}

#pragma region Update
void UCustomCameraComponent::UpdateLag(const FCameraLagSettings& _newLagSettings)
{
	springArm->bEnableCameraLag = _newLagSettings.enableLag;
	springArm->bEnableCameraRotationLag = _newLagSettings.enableRotation;
	springArm->CameraLagSpeed = _newLagSettings.speed;
	springArm->CameraRotationLagSpeed = _newLagSettings.rotationSpeed;
	springArm->CameraLagMaxDistance = _newLagSettings.maxDistance;
}

void UCustomCameraComponent::UpdateFOV(const float _newFOV)
{
	camera->FieldOfView = _newFOV;
}

void UCustomCameraComponent::UpdateOffset(const FCameraOffsetSettings& _newOffsetSettings)
{
	const FVector _currentSocketOffset = springArm->SocketOffset;
	const FVector _newSocketOffset = FVector(_currentSocketOffset.X, _newOffsetSettings.offsetHorizontal, _newOffsetSettings.offsetVertical);
	springArm->SocketOffset = _newSocketOffset;
}

void UCustomCameraComponent::UpdateDistance(const float _newDistance)
{
	springArm->TargetArmLength = _newDistance;
}

void UCustomCameraComponent::UpdatePitch(const float _newPitch)
{
	FRotator _currentRotator = springArm->GetRelativeRotation();
	springArm->SetRelativeRotation(FRotator(_newPitch, _currentRotator.Yaw, _currentRotator.Roll));
}

void UCustomCameraComponent::UpdateYaw(const float _newYaw)
{
	FRotator _currentRotator = springArm->GetRelativeRotation();
	springArm->SetRelativeRotation(FRotator(_currentRotator.Pitch, _newYaw, _currentRotator.Roll));

}
#pragma endregion

#pragma region Lerp
//void UCustomCameraComponent::LerpLag(const float _blendSpeed, const FCameraLagSettings _newLagSettings)
//{
//	if (!useLerp || !isLerpingLag)
//		return;
//
//	const bool _enableLag = _newLagSettings.enableLag;
//	springArm->bEnableCameraLag = _enableLag;
//	const bool _enableRotation = _newLagSettings.enableRotation;
//	springArm->bEnableCameraRotationLag = _enableRotation;
//
//	const float _settingsSpeed = _newLagSettings.speed;
//	float _speed = springArm->CameraLagSpeed;
//
//	const float _settingsRotationSpeed = _newLagSettings.rotationSpeed;
//	float _rotationSpeed = springArm->CameraRotationLagSpeed;
//
//	const float _settingsMaxDistance = _newLagSettings.maxDistance;
//	float _maxDistance = springArm->CameraLagMaxDistance;
//
//	if (_enableLag || _enableRotation)
//	{
//		bool _isLerpingSpeed = false;
//		bool _isLerpingRotationSpeed = false;
//
//		if (_enableLag)
//		{
//			_speed = springArm->CameraLagSpeed = UKismetMathLibrary::Lerp(_speed, _settingsSpeed, _blendSpeed);
//			_isLerpingSpeed = !CheckValuesAlmostEquals(_speed, _settingsSpeed);
//		}
//
//		if (_enableRotation)
//		{
//			_rotationSpeed = springArm->CameraRotationLagSpeed = UKismetMathLibrary::Lerp(_rotationSpeed, _settingsRotationSpeed, _blendSpeed);
//			_isLerpingRotationSpeed = !CheckValuesAlmostEquals(_rotationSpeed, _settingsRotationSpeed);
//		}
//
//		_maxDistance = springArm->CameraLagMaxDistance = UKismetMathLibrary::Lerp(_maxDistance, _settingsMaxDistance, _blendSpeed);
//		const bool _isLerpingMaxDistance = !CheckValuesAlmostEquals(_maxDistance, _settingsMaxDistance);
//
//		isLerpingLag = _isLerpingSpeed || _isLerpingRotationSpeed || _isLerpingMaxDistance;
//	}
//	else
//		isLerpingLag = false;
//
//	if (!isLerpingLag)
//	{
//		springArm->CameraLagSpeed = _settingsSpeed;
//		springArm->CameraRotationLagSpeed = _settingsRotationSpeed;
//		springArm->CameraLagMaxDistance = _settingsMaxDistance;
//	}
//
//}
//
//void UCustomCameraComponent::LerpFOV(const float _blendSpeed, const float _newFOV)
//{
//	if (!useLerp || !isLerpingFOV)
//		return;
//
//	camera->FieldOfView = UKismetMathLibrary::Lerp(camera->FieldOfView, _newFOV, _blendSpeed);
//	isLerpingFOV = !CheckValuesAlmostEquals(camera->FieldOfView, _newFOV);
//
//	if (!isLerpingFOV)
//		camera->FieldOfView = _newFOV;
//
//}

void UCustomCameraComponent::LerpOffset()
{
	if (!springArm || !useLerp || !offsetLerp.isLerping)
		return;

	FVector& _socketOffset = springArm->SocketOffset;

	const FVector _baseSocketOffset = FVector(_socketOffset.X, offsetLerp.baseValueVector.Y, offsetLerp.baseValueVector.Z);
	const FVector _newSocketOffset = FVector(_socketOffset.X, offsetLerp.finalValueVector.Y, offsetLerp.finalValueVector.Z);
	_socketOffset = UKismetMathLibrary::VLerp(_baseSocketOffset, _newSocketOffset, offsetLerp.alpha);

	offsetLerp.isLerping = offsetLerp.alpha < 1.0f;

	if (!offsetLerp.isLerping)
		_socketOffset = _newSocketOffset;
}

void UCustomCameraComponent::LerpDistance()
{
	if (!springArm || !useLerp || !distanceLerp.isLerping)
		return;

	float& _distance = springArm->TargetArmLength;
	_distance = UKismetMathLibrary::Lerp(distanceLerp.baseValueFloat, distanceLerp.finalValueFloat, distanceLerp.alpha);

	distanceLerp.isLerping = distanceLerp.alpha < 1.0f;

	if (!distanceLerp.isLerping)
		_distance = distanceLerp.finalValueFloat;
}

void UCustomCameraComponent::LerpCameraPitch()
{
	if (!camera || !useLerp || !pitchCameraLerp.isLerping)
		return;

	const FRotator _currentRotator = camera->GetRelativeRotation();
	const float _pitch = FMath::Lerp(pitchCameraLerp.baseValueFloat, pitchCameraLerp.finalValueFloat, pitchCameraLerp.alpha);
	camera->SetRelativeRotation(FRotator((double)_pitch, _currentRotator.Yaw, _currentRotator.Roll));

	pitchCameraLerp.isLerping = pitchCameraLerp.alpha < 1.0f;

	if (!pitchCameraLerp.isLerping)
	{
		camera->SetRelativeRotation(FRotator(pitchCameraLerp.finalValueFloat, _currentRotator.Yaw, _currentRotator.Roll));

		if (!onPitchCameraLerpReturn)
			EndShoot(shootAddedPitch, shootReturnLerpDuration);
		else
			onPitchCameraLerpReturn = false;
	}

	const float _tiltRatio = camera->GetRelativeRotation().Pitch / shootMaxPitch;
	onShoot.Broadcast(_tiltRatio);
}

//void UCustomCameraComponent::LerpYaw(const float _blendSpeed, const float _newYaw)
//{
//	if (!useLerp || !isLerpingYaw)
//		return;
//
//	FRotator _currentRotator = springArm->GetComponentRotation();
//	const float _yaw = FMath::Lerp((float)_currentRotator.Yaw, _newYaw, _blendSpeed);
//	springArm->SetRelativeRotation(FRotator(_currentRotator.Pitch, (double)_yaw, _currentRotator.Roll));
//
//	isLerpingYaw = !CheckValuesAlmostEquals(_yaw, _newYaw);
//
//	if (!isLerpingYaw)
//		springArm->SetRelativeRotation(FRotator(_currentRotator.Pitch, _newYaw, _currentRotator.Roll));
//}
#pragma endregion

bool UCustomCameraComponent::CheckValuesAlmostEquals(const float _firstValue, const float _secondValue)
{
	if (_firstValue > _secondValue)
		return (FMath::Abs(_firstValue - _secondValue) <= 0.1f);
	else
		return (FMath::Abs(_secondValue - _firstValue) <= 0.1f);
}

void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	//Add by Romain
	if (const USaveManagerSubsystem* _saveSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>())
	{
		if (const USettingsSave* _save = _saveSystem->GetSaveSettings())
			currentSettings.fieldOfView = _save->GetFOV();
	}

	if (camera)
		camera->SetFieldOfView(currentSettings.fieldOfView);

	lastSettingsKey = currentSettingsKey;

	useLerp = true;
}

#if WITH_EDITOR
void UCustomCameraComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool _canUpdateCamera = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UCustomCameraComponent, currentSettingsKey) ||
		PropertyChangedEvent.MemberProperty->GetName() == GET_MEMBER_NAME_CHECKED(UCustomCameraComponent, allSettings);

	if (_canUpdateCamera)
		UpdateCurrentSettings(currentSettingsKey);

	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (offsetLerp.isLerping || distanceLerp.isLerping)
	{
		offsetLerp.UpdateAlpha(DeltaTime, blendCurve);
		distanceLerp.UpdateAlpha(DeltaTime, blendCurve);
	}

	if (pitchCameraLerp.isLerping)
		pitchCameraLerp.UpdateAlpha(DeltaTime);

	//LerpLag(_blendSpeed, currentSettings.lag);
	//LerpFOV(_blendSpeed, currentSettings.fieldOfView);

	LerpOffset();
	LerpDistance();
	LerpCameraPitch();

	/*if (currentSettings.useRotation)
	{
		if (currentSettings.rotation.usePitch)
			LerpPitch(_blendSpeed, currentSettings.rotation.pitch);

		if (currentSettings.rotation.useYaw)
			LerpYaw(_blendSpeed, currentSettings.rotation.yaw);
	}*/
}


void UCustomCameraComponent::UpdateCurrentSettings(const FString& _key)
{
	if (!allSettings.Contains(_key))
		return;

	lastSettingsKey = currentSettingsKey;
	currentSettingsKey = _key;
	currentSettings = allSettings[_key];

	UpdateLag(currentSettings.lag);

	if (useLerp)
	{
		offsetLerp.ResetAlpha();
		distanceLerp.ResetAlpha();

		distanceLerp.baseValueFloat = springArm->TargetArmLength;
		distanceLerp.finalValueFloat = currentSettings.distance;

		offsetLerp.baseValueVector = springArm->SocketOffset;
		offsetLerp.finalValueVector = FVector(0.0f, currentSettings.offset.offsetHorizontal, currentSettings.offset.offsetVertical);

		const FCameraBlendSettings _blendSettings = currentSettings.GetBlendSettings(lastSettingsKey);

		offsetLerp.totalDuration = distanceLerp.totalDuration = _blendSettings.blendDuration;
		blendCurve = _blendSettings.blendCurve;

		//isLerpingFOV = true;

		offsetLerp.isLerping = true;
		distanceLerp.isLerping = true;

		/*if (currentSettings.useRotation)
		{
			if (currentSettings.rotation.usePitch)
				isLerpingPitch = true;

			if (currentSettings.rotation.useYaw)
				isLerpingYaw = true;
		}*/

		return;
	}

	//UpdateFOV(currentSettings.fieldOfView);
	UpdateOffset(currentSettings.offset);
	UpdateDistance(currentSettings.distance);

	if (currentSettings.useRotation)
	{
		if (currentSettings.rotation.usePitch)
			UpdatePitch(currentSettings.rotation.pitch);

		if (currentSettings.rotation.useYaw)
			UpdateYaw(currentSettings.rotation.yaw);
	}
}


void UCustomCameraComponent::SetupAttachment(USceneComponent* _root)
{
	springArm->SetupAttachment(_root);
	camera->SetupAttachment(springArm);
}

void UCustomCameraComponent::DesynchYaw(const bool _desynch)
{
	if (springArm)
		springArm->bInheritYaw = !_desynch;
}

void UCustomCameraComponent::DesynchRoll(const bool _desynch)
{
	if (springArm)
		springArm->bInheritRoll = !_desynch;
}

void UCustomCameraComponent::DesynchPitch(const bool _desynch)
{
	if (springArm)
		springArm->bInheritPitch = !_desynch;
}


void UCustomCameraComponent::Shoot(const float _maxPitch, const float _addedPitch, const float _lerpDuration)
{
	onPitchCameraLerpReturn = false;
	pitchCameraLerp.isLerping = false;

	shootMaxPitch = _maxPitch;

	const FRotator _currentRotator = camera->GetRelativeRotation();

	pitchCameraLerp.ResetAlpha();
	pitchCameraLerp.totalDuration = _lerpDuration;

	pitchCameraLerp.baseValueFloat = (float)_currentRotator.Pitch;

	float _finalValue = pitchCameraLerp.baseValueFloat + _addedPitch;
	_finalValue = _finalValue > _maxPitch ? _maxPitch : _finalValue;

	pitchCameraLerp.finalValueFloat = _finalValue;

	pitchCameraLerp.isLerping = true;
}

void UCustomCameraComponent::EndShoot(const float _addedPitch, const float _lerpDuration)
{
	onPitchCameraLerpReturn = true;
	pitchCameraLerp.isLerping = false;

	const FRotator _currentRotator = camera->GetRelativeRotation();

	pitchCameraLerp.ResetAlpha();
	pitchCameraLerp.totalDuration = ((float)_currentRotator.Pitch / _addedPitch) * _lerpDuration;

	pitchCameraLerp.baseValueFloat = (float)_currentRotator.Pitch;
	pitchCameraLerp.finalValueFloat = 0.0f;

	pitchCameraLerp.isLerping = true;
}

void UCustomCameraComponent::SetShootTiltParameters(const float _shootAddedPitch, const float _shootReturnLerpDuration)
{
	shootAddedPitch = _shootAddedPitch;
	shootReturnLerpDuration = _shootReturnLerpDuration;
}

void UCustomCameraComponent::TestShoot()
{
	Shoot(5.0f, 0.5f, 0.05f);
}

void UCustomCameraComponent::TestEndShoot()
{
	EndShoot(0.5f, 0.1f);
}
