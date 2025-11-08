#include "ToDownTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToDownTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetIsDown() && !player->GetDisableInput();
}
