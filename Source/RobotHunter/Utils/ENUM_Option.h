#pragma once

UENUM()
enum class ECategoryOption
{
	None UMETA(DisplayName = "Choose an options"),
	ACCESSIBILITY UMETA(DisplayName = "Accessibility"),
	GAMEPLAY UMETA(DisplayName = "Gameplay"),
	GRAPHICS UMETA(DisplayName = "Graphics"),
	SOUND UMETA(DisplayName = "Sound"),
	DEBUG UMETA(DisplayName = "Debug"),
	KEYBIND UMETA(DisplayName = "Keybind"),
	GAMEPAD UMETA(DisplayName = "Gamepad"),
};
