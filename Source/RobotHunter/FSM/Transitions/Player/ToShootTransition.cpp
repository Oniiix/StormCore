#include "ToShootTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToShootTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetIsShooting();
}
