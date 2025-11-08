


#include "ToBumpTransition.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"


bool UToBumpTransition::IsValidTransition()
{
	RetrievePlayer();

	return player->GetCharaAnim() && player->GetCharaAnim()->GetIsBumped() && !player->GetDisableInput();
}
