#pragma once


UENUM()
enum class EReceiverType
{
	Receiver_None UMETA(DisplayName = "Default (None)"),
	Switch UMETA(DisplayName = "Switch"),
	Timer UMETA(DisplayName = "Timer"),
	Endless UMETA(DisplayName = "Endless")
};
