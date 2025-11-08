#include "ToMenuTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToMenuTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetInMenu();
}
