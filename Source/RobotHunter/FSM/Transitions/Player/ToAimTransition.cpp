#include "ToAimTransition.h"
#include "../../../CCC/Character/MainCharacter.h"

bool UToAimTransition::IsValidTransition()
{
    RetrievePlayer();
    return player->GetIsAiming() && !player->GetDisableInput();
}
