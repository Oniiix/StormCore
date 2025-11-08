#include "HandcarDataAsset.h"

UHandcarDataAsset::UHandcarDataAsset()
{
#pragma region Debug
	frictionDebug = false;
	brakeDebug = false;
	slopeAccelerationDebug = false;
	slopeAngleDebug = false;
	//pumpAccelerationDebug = false;
	nitroAccelerationDebug = false;
	totalAccelerationDebug = false;
	speedDebug = false;
#pragma endregion

#pragma region Handcar
	handcarRotationSpeed = 25.0f;

#pragma region Speed
	handcarMaxSpeed = 2.5f;

#pragma region Friction
	frictionValue = -1.0f;
	//frictionChangeAngle = 5.0f;
	frictionSpeedPercent = 0.1f;
#pragma endregion

#pragma endregion

#pragma endregion

#pragma region Handle

#pragma region Acceleration
	/*pumpMinForce = 500.0f;
	pumpMaxForce = 1000.0f;
	accelerationBoost = 1.5f;*/
#pragma endregion

#pragma region Rotation
	/*handleMaxRotation = 45.0f;
	handleRotationValue = 50.0f;*/
#pragma endregion

#pragma endregion

#pragma region Brake

#pragma region Position

#pragma region Angle
	brakeTightenedAngle = 45.0f;
	brakeReleasedAngle = 135.0f;
#pragma endregion

#pragma region Speed
	/*brakeMinInteractionValue = 7.0f;
	brakeMaxInteractionValue = 10.0f;
	brakeReleasedSpeed = 20.0f;*/
	brakeSpeedPercent = 0.1f;
#pragma endregion

#pragma endregion

	brakeValue = -5.0f;

#pragma endregion

#pragma region Nitro
	nitroAcceleration = 5.0f;
	//cursorSpeed = 5.0f;

#pragma region Zone
	/*greenZoneSize = 10.0f;
	yellowZoneSize = 40.0f;*/
#pragma endregion

#pragma region Movement

#pragma region Angle
	/*nitroMinMovementAngle = -50.0f;
	nitroMaxMovementAngle = 50.0f;*/
#pragma endregion

#pragma region Speed
	/*nitroMinMovementSpeed = 5.0f;
	nitroMaxMovementSpeed = 10.0f;*/
#pragma endregion

#pragma region Time
	//nitroMinMovementTime = 2.0f;
	//nitroMaxMovementTime = 5.0f;
#pragma endregion

#pragma endregion

#pragma region LifeConsumption
	//greenLifeConsumption = 5.0f;
	//yellowLifeConsumption = 10.0f;
	//redLifeConsumption = 15.0f;
#pragma endregion

#pragma endregion

#pragma region LifeStock
	lifeExchangeValue = 10.0f;
	lifeExchangeRate = 1.0f;
#pragma endregion
}

bool UHandcarDataAsset::GetDebug(const EHandcarDebugEnum& _debugEnum) const
{
	switch (_debugEnum)
	{
	case Friction:
		return frictionDebug;
	case Brake:
		return brakeDebug;
	case SlopeAcceleration:
		return slopeAccelerationDebug;
	case SlopeAngle:
		return slopeAngleDebug;
	/*case Pump:
		return pumpAccelerationDebug;*/
	case Nitro:
		return nitroAccelerationDebug;
	case TotalAcceleration:
		return totalAccelerationDebug;
	case Speed:
		return speedDebug;
	default:
		return false;
	};
}

//float UHandcarDataAsset::GetZoneSize(const ESliderColor& _color) const
//{
//	switch (_color)
//	{
//	case GreenSlider:
//		return greenZoneSize;
//	case YellowSlider:
//		return yellowZoneSize;
//	default:
//		return 0.0f;
//	};
//}
//
//float UHandcarDataAsset::GetLifeConsumption(const ESliderColor& _color)
//{
//	switch (_color)
//	{
//	case GreenSlider:
//		return greenLifeConsumption;
//	case YellowSlider:
//		return yellowLifeConsumption;
//	case RedSlider:
//		return redLifeConsumption;
//	default:
//		return 0.0f;
//	};
//}
