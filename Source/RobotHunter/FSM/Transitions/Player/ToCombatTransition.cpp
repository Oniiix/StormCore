#include "ToCombatTransition.h"
#include "../../../CCC/Character/MainCharacter.h"
#include <RobotHunter/Game/GPE/Members/ThighActors/OffRoadRollerdrives/OffRoadRollerdrivesActor.h>
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"

bool UToCombatTransition::IsValidTransition()
{
	RetrievePlayer();

	//OPTI
	bool _isRunning = false;
	AOffRoadRollerdrivesActor* _roller = Cast<AOffRoadRollerdrivesActor>(player->GetUsingThigh());
	if (_roller)
		_isRunning = _roller->GetIsRunning();
	//

	const bool _isUsingHandcar = player->GetIsUsingNitroHandcar();
	return player->GetInCombat() && !player->GetIsAiming() && !_isUsingHandcar && !player->GetIsDown() && !player->GetCharaAnim()->GetIsBumped() && !_isRunning && !player->GetIsDashing() && !player->GetInPause() && !player->GetDisableInput();
}
