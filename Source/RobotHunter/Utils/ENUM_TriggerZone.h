#pragma once


UENUM()
enum class ETriggerTypeActivation
{
	ENTER_ZONE,
	EXIT_ZONE,
	BOTH,
};


UENUM()
enum class EShapeType
{
	SHAPE_NONE UMETA(DisplayName = "Default (None)"),
	BOX,
	SPHERE,
	CAPSULE,
};