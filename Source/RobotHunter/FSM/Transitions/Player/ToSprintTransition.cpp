#include "ToSprintTransition.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../Game/GPE/Members/ThighActors/OffRoadRollerdrives/OffRoadRollerdrivesActor.h"

bool UToSprintTransition::IsValidTransition()
{
	RetrievePlayer(); 
	
	bool _isRunning = false;

	AOffRoadRollerdrivesActor* _roller = Cast<AOffRoadRollerdrivesActor>(player->GetUsingThigh());

	if (_roller)
		_isRunning = _roller->GetIsRunning();

	return _isRunning && !player->GetDisableInput();
}