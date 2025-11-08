#include "InputConfig.h"

UInputConfig::UInputConfig()
{
#pragma region Context

#pragma region Handcar
	//handcarContext = nullptr;
	brakeHandcarContext = nullptr;
	handleHandcarContext = nullptr;
	nitroHandcarContext = nullptr;
#pragma endregion

#pragma endregion Context

	wagonStop = nullptr;

#pragma region Handcar
	handcarAcceleration = nullptr;

	handcarBrake = nullptr;
	handcarReleaseBrake = nullptr;

	handcarNitroLeft = nullptr;
	handcarNitroRight = nullptr;

	/*handcarChangeWay = nullptr;
	handcarHorn = nullptr;*/
#pragma endregion Handcar
}
