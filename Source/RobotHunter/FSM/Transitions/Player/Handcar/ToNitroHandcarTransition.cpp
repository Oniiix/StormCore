#include "ToNitroHandcarTransition.h"
#include "../../../../CCC/Character/MainCharacter.h"

bool UToNitroHandcarTransition::IsValidTransition()
{
	RetrievePlayer();
	return player->GetIsUsingNitroHandcar() && !player->GetInPause() && !player->GetDisableInput();
}
