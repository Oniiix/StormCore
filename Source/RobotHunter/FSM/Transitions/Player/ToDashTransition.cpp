#include "ToDashTransition.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

bool UToDashTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetIsDashing() && !player->GetDisableInput();
}
