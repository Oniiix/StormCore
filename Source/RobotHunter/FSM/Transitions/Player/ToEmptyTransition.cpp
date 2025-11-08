


#include "ToEmptyTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToEmptyTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetDisableInput();
}
