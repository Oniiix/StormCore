#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../../UI/ATH/Draisine/DraisineATH.h"
#include "HandcarDataAsset.generated.h"

UENUM()
enum EHandcarDebugEnum
{
	Friction,
	Brake,
	SlopeAcceleration,
	SlopeAngle,
	//Pump,
	Nitro,
	TotalAcceleration,
	Speed
};

UCLASS()
class ROBOTHUNTER_API UHandcarDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool frictionDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool brakeDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool slopeAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool slopeAngleDebug;

	/*UPROPERTY(EditAnywhere, Category = "Debug")
	bool pumpAccelerationDebug;*/

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool nitroAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool totalAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool speedDebug;
#pragma endregion

#pragma region Handcar
	UPROPERTY(EditAnywhere, Category = "Handcar|Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float handcarRotationSpeed;

#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Handcar|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float handcarMaxSpeed;

#pragma region Friction
	UPROPERTY(EditAnywhere, Category = "Handcar|Speed|Friction", meta = (UIMax = 0.0f, ClampMax = 0.0f))
	float frictionValue;

	/*UPROPERTY(EditAnywhere, Category = "Handcar|Speed|Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float frictionChangeAngle;*/

	UPROPERTY(EditAnywhere, Category = "Handcar|Speed|Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float frictionSpeedPercent;
#pragma endregion	

#pragma endregion

#pragma endregion

#pragma region Handle

#pragma region Acceleration
	/*UPROPERTY(EditAnywhere, Category = "Handle|Acceleration|Pump", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float pumpMinForce;

	UPROPERTY(EditAnywhere, Category = "Handle|Acceleration|Pump", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float pumpMaxForce;

	UPROPERTY(EditAnywhere, Category = "Handle|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float accelerationBoost;*/
#pragma endregion

#pragma region Rotation
	//UPROPERTY(EditAnywhere, Category = "Handle|Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	//float handleMaxRotation;

	//UPROPERTY(EditAnywhere, Category = "Handle|Rotation", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	//float handleRotationValue;
#pragma endregion

#pragma endregion

#pragma region Brake

#pragma region Position

#pragma region Angle
	UPROPERTY(EditAnywhere, Category = "Brake|Position|Angle", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float brakeTightenedAngle;

	UPROPERTY(EditAnywhere, Category = "Brake|Position|Angle", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float brakeReleasedAngle;
#pragma endregion

#pragma region Speed
	/*UPROPERTY(EditAnywhere, Category = "Brake|Position|Speed", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float brakeMinInteractionValue;

	UPROPERTY(EditAnywhere, Category = "Brake|Position|Speed", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float brakeMaxInteractionValue;

	UPROPERTY(EditAnywhere, Category = "Brake|Position|Speed", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float brakeReleasedSpeed;*/

	UPROPERTY(EditAnywhere, Category = "Brake|Position|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float brakeSpeedPercent;
#pragma endregion

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Brake|Acceleration", meta = (UIMax = 0.0f, ClampMax = 0.0f))
	float brakeValue;

#pragma endregion

#pragma region Nitro
	UPROPERTY(EditAnywhere, Category = "Nitro|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroAcceleration;

	/*UPROPERTY(EditAnywhere, Category = "Nitro|Cursor", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float cursorSpeed;*/

#pragma region Zone
	/*UPROPERTY(EditAnywhere, Category = "Nitro|Zone", meta = (UIMin = 0.0f, UIMax = 100.0f, ClampMin = 0.0f, ClampMax = 100.0f))
	float greenZoneSize;

	UPROPERTY(EditAnywhere, Category = "Nitro|Zone", meta = (UIMin = 0.0f, UIMax = 100.0f, ClampMin = 0.0f, ClampMax = 100.0f))
	float yellowZoneSize;*/
#pragma endregion

#pragma region Movement

#pragma region Angle
	/*UPROPERTY(EditAnywhere, Category = "Nitro|Movement|Angle", meta = (UIMin = -90.0f, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float nitroMinMovementAngle;

	UPROPERTY(EditAnywhere, Category = "Nitro|Movement|Angle", meta = (UIMin = -90.0, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float nitroMaxMovementAngle;*/
#pragma endregion

#pragma region Speed
	/*UPROPERTY(EditAnywhere, Category = "Nitro|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroMinMovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Nitro|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroMaxMovementSpeed;*/
#pragma endregion

#pragma region Time
	/*UPROPERTY(EditAnywhere, Category = "Nitro|Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroMinMovementTime;

	UPROPERTY(EditAnywhere, Category = "Nitro|Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroMaxMovementTime;*/
#pragma endregion

#pragma endregion

#pragma region LifeConsumption
	/*UPROPERTY(EditAnywhere, Category = "Nitro|LifeConsumption")
	float greenLifeConsumption;

	UPROPERTY(EditAnywhere, Category = "Nitro|LifeConsumption")
	float yellowLifeConsumption;

	UPROPERTY(EditAnywhere, Category = "Nitro|LifeConsumption")
	float redLifeConsumption;*/
#pragma endregion

#pragma endregion

#pragma region LifeStock
	UPROPERTY(EditAnywhere, Category = "LifeStock", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float lifeExchangeValue;

	UPROPERTY(EditAnywhere, Category = "LifeStock", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float lifeExchangeRate;
#pragma endregion

public:
	FORCEINLINE float GetHandcarRotationSpeed() const { return handcarRotationSpeed; }
	FORCEINLINE float GetHandcarMaxSpeed() const { return handcarMaxSpeed; }

	FORCEINLINE float GetFrictionValue() const { return frictionValue; }
	//FORCEINLINE float GetFrictionChangeAngle() const { return frictionChangeAngle; }
	FORCEINLINE float GetFrictionSpeedPercent() const { return frictionSpeedPercent; }

	/*FORCEINLINE float GetPumpForce(const bool _min) const { return _min ? pumpMinForce : pumpMaxForce; }
	FORCEINLINE float GetAccelerationBoost() const { return accelerationBoost; }*/


	FORCEINLINE float GetHandleMaxRotation() const { return 0.0f; }
	FORCEINLINE float GetHandleRotationValue() const { return 0.0f; }


	FORCEINLINE float GetBrakeAngle(const bool _isReleased) { return _isReleased ? brakeReleasedAngle : brakeTightenedAngle; }
	/*FORCEINLINE float GetBrakeInteractionValue(const bool _min) const { return _min ? brakeMinInteractionValue : brakeMaxInteractionValue; }
	FORCEINLINE float GetBrakeReleasedSpeed() const { return brakeReleasedSpeed; }*/
	FORCEINLINE float GetBrakeSpeedPercent() const { return brakeSpeedPercent; }
	FORCEINLINE float GetBrakeValue() const { return brakeValue; }


	FORCEINLINE float GetNitroAcceleration() const { return nitroAcceleration; }
	//FORCEINLINE float GetCursorSpeed() const { return cursorSpeed; }

	/*FORCEINLINE float GetNitroMovementAngle(const bool _min) const { return _min ? nitroMinMovementAngle : nitroMaxMovementAngle; }
	FORCEINLINE float GetNitroMovementSpeed(const bool _min) const { return _min ? nitroMinMovementSpeed : nitroMaxMovementSpeed; }
	FORCEINLINE float GetNitroMovementTime(const bool _min) const{ return _min ? nitroMinMovementTime : nitroMaxMovementTime; }*/

	FORCEINLINE float GetLifeExchangeValue() const { return lifeExchangeValue; }
	FORCEINLINE float GetLifeExchangeRate() const { return lifeExchangeRate; }


public:
	UHandcarDataAsset();

public:
	bool GetDebug(const EHandcarDebugEnum& _debugEnum) const;
	/*float GetZoneSize(const ESliderColor& _color) const;
	float GetLifeConsumption(const ESliderColor& _color);*/
};
