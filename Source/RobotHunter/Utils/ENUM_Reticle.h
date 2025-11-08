#pragma once
/*
* Enum pour le HUD 
*/
UENUM(Blueprintable)
enum class EReticleType : uint8
{
	None,
	LEFT_ARM UMETA(DisplayName = "Reticle Left Arm"),
	RIGHT_ARM UMETA(DisplayName = "Reticle Right Arm")
};
