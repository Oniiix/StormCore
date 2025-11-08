#pragma once

#include "CoreMinimal.h"
#include "../../../CustomActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "CustomCameraComponent.generated.h"

#pragma region Camera settings structures
USTRUCT()
struct FCameraRotationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool usePitch;

	UPROPERTY(EditAnywhere)
	bool useYaw;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "usePitch", EditConditionHides))
	float pitch;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "useYaw", EditConditionHides))
	float yaw;

	FCameraRotationSettings()
	{
		usePitch = true;
		useYaw = true;
		pitch = 0.0f;
		yaw = 0.0f;
	}
};

USTRUCT()
struct FCameraLagSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool enableLag;

	UPROPERTY(EditAnywhere)
	bool enableRotation;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f, EditCondition = "enableLag"))
	float speed;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f, EditCondition = "enableRotation"))
	float rotationSpeed;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "enableLag || enableRotation"))
	float maxDistance;

	FCameraLagSettings()
	{
		enableLag = false;
		enableRotation = false;
		speed = 10.0f;
		rotationSpeed = 10.0f;
		maxDistance = 0.0f;
	}
};

USTRUCT()
struct FCameraOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float offsetVertical;

	UPROPERTY(EditAnywhere)
	float offsetHorizontal;

	FCameraOffsetSettings()
	{
		offsetVertical = 0.0f;
		offsetHorizontal = 0.0f;
	}
};

USTRUCT()
struct FCameraBlendSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> blendCurve;

	UPROPERTY(EditAnywhere)
	float blendDuration;

	FCameraBlendSettings()
	{
		blendCurve = nullptr;
		blendDuration = 1.0f;
	}
};

USTRUCT()
struct FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool useRotation;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float distance;

	UPROPERTY(EditAnywhere, meta = (UIMin = 5.0f, ClampMin = 5.0f, UIMax = 170.0f, ClampMax = 170.0f))
	float fieldOfView;

	UPROPERTY(EditAnywhere)
	FCameraBlendSettings blendSettings;

	UPROPERTY(EditAnywhere)
	TMap<FString, FCameraBlendSettings> specificBlendSettings;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "useRotation", EditConditionHides))
	FCameraRotationSettings rotation;

	UPROPERTY(EditAnywhere)
	FCameraLagSettings lag;

	UPROPERTY(EditAnywhere)
	FCameraOffsetSettings offset;

	FCameraSettings()
	{
		lag = FCameraLagSettings();
		offset = FCameraOffsetSettings();
		useRotation = true;
		rotation = FCameraRotationSettings();
		distance = 300.0f;
		fieldOfView = 90.f;
		blendSettings = FCameraBlendSettings();
		specificBlendSettings = TMap<FString, FCameraBlendSettings>();
	}

	FORCEINLINE FCameraBlendSettings GetBlendSettings(const FString& _lastSettings) const
	{
		if (specificBlendSettings.Contains(_lastSettings))
			return specificBlendSettings[_lastSettings];

		return blendSettings;
	}

	FORCEINLINE float GetBlendDuration(const FString& _lastSettings) const
	{
		return GetBlendSettings(_lastSettings).blendDuration;
	}
};

USTRUCT()
struct FCameraLerp
{
	GENERATED_BODY()

	bool isLerping;

	float totalDuration;
	float currentDuration;

	float time;
	float alpha;

	float baseValueFloat;
	float finalValueFloat;

	FVector baseValueVector;
	FVector finalValueVector;

	FCameraLerp()
	{
		isLerping = false;

		totalDuration = 0.0f;
		currentDuration = 0.0f;

		time = 0.0f;
		alpha = 0.0f;

		baseValueFloat = 0.0f;
		finalValueFloat = 0.0f;

		baseValueVector = FVector(0.0f);
		finalValueVector = FVector(0.0f);
	}

	void UpdateAlpha(const float _deltaTime, UCurveFloat* _lerpCurve = nullptr)
	{
		currentDuration += _deltaTime;
		time = FMath::Lerp(0.0f, 1.0f, currentDuration / totalDuration);
		alpha = _lerpCurve ? _lerpCurve->GetFloatValue(time) : time;
	}

	void ResetAlpha()
	{
		currentDuration = time = alpha = 0.0f;
	}
};
#pragma endregion

UCLASS()
class ROBOTHUNTER_API UCustomCameraComponent : public UCustomActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShoot, float, tiltRatio);

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnShoot onShoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> camera;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FString currentSettingsKey;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<FString, FCameraSettings> allSettings;

	bool useLerp;
	/*bool isLerpingLag;
	bool isLerpingFOV;
	bool isLerpingOffset;
	bool isLerpingDistance;
	bool isLerpingPitch;
	bool isLerpingYaw;*/

	UPROPERTY()
	TObjectPtr<UCurveFloat> blendCurve;

	FString lastSettingsKey;
	FCameraSettings currentSettings;

	FCameraLerp offsetLerp;
	FCameraLerp distanceLerp;

	bool onPitchCameraLerpReturn;
	float shootAddedPitch;
	float shootReturnLerpDuration;
	float shootMaxPitch;
	FCameraLerp pitchCameraLerp;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return springArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return camera; }
	FORCEINLINE FCameraSettings GetCameraSettings() const { return currentSettings; }

	FORCEINLINE float GetBlendDuration() const { return currentSettings.GetBlendDuration(lastSettingsKey); }
	FORCEINLINE float GetFOV() const { return currentSettings.fieldOfView; } //Add by Romain
	FORCEINLINE FString GetCurrentSettingsKey() const { return currentSettingsKey; }

	FORCEINLINE FOnShoot& GetOnShoot() { return onShoot; }

public:
	UCustomCameraComponent();
	void SetFOV(const float _fov); //Add by Romain

private:
#pragma region Update
	void UpdateLag(const FCameraLagSettings& _newLagSettings);
	void UpdateFOV(const float _newFOV);
	void UpdateOffset(const FCameraOffsetSettings& _newOffsetSettings);
	void UpdateDistance(const float _newDistance);
	void UpdatePitch(const float _newPitch);
	void UpdateYaw(const float _newYaw);
#pragma endregion

#pragma region Lerp
	/*void LerpLag(const float _blendSpeed, const FCameraLagSettings _newLagSettings);
	void LerpFOV(const float _blendSpeed, const float _newFOV);*/
	void LerpOffset();
	void LerpDistance();
	void LerpCameraPitch();
	//void LerpYaw(const float _blendSpeed, const float _newYaw);
#pragma endregion

	bool CheckValuesAlmostEquals(const float _firstValue, const float _secondValue);

protected:
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void UpdateCurrentSettings(const FString& _key);
	virtual void SetupAttachment(USceneComponent* _root) override;

	void DesynchYaw(const bool _desynch);
	void DesynchRoll(const bool _desynch);
	void DesynchPitch(const bool _desynch);

	void Shoot(const float _maxPitch, const float _addedPitch, const float _lerpDuration);
	void EndShoot(const float _addedPitch, const float _lerpDuration);
	void SetShootTiltParameters(const float _shootAddedPitch, const float _shootReturnLerpDuration);

	UFUNCTION(CallInEditor)
	void TestShoot();
	UFUNCTION(CallInEditor)
	void TestEndShoot();
};
