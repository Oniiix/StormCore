#include "AimState.h"
#include "../../../CCC/Character/MainCharacter.h"

UAimState::UAimState()
{
	rightArm = nullptr;
}

void UAimState::RetrieveRightArm()
{
	if (player)
	{
		rightArm = player->GetUsingRightArm();
		cameraSettingsKey = rightArm ? rightArm->GetCameraKey() : FString();
	}
}

void UAimState::Enter()
{
	RetrievePlayer();
	RetrieveRightArm();
	UpdateCameraSettings();
	UpdateInputMappingContext();
	UpdateStateSpeed();
	UCustomState::Enter();
}


