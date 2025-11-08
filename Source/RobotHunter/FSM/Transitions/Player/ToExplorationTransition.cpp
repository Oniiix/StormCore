#include "ToExplorationTransition.h"
#include "../../../CCC/Character/MainCharacter.h"
#include <RobotHunter/Game/GPE/Members/ThighActors/OffRoadRollerdrives/OffRoadRollerdrivesActor.h>
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"

bool UToExplorationTransition::IsValidTransition()
{
	RetrievePlayer();
	const bool _HUBStatus = player->GetCustomGameInstance()->GetGameStatus() == EGameStatus::HUB;

	//UDetectionComponent* _detectionComp = player->GetDetectionComponent();
	//const bool _hasDetectedActor = _detectionComp ? _detectionComp->GetHasDetectedActor() : false;

	const bool _isUsingHandcar = player->GetIsUsingNitroHandcar();

	bool _isRunning = false;

	AOffRoadRollerdrivesActor* _roller = Cast<AOffRoadRollerdrivesActor>(player->GetUsingThigh());

	if (_roller)
		_isRunning = _roller->GetIsRunning();

	return !_HUBStatus && !player->GetInInventory() && !player->GetInPause() /*&& !_hasDetectedActor*/
		&& !player->GetIsDown() && !player->GetIsAiming() && !_isUsingHandcar && !player->GetInCombat()
		&& !player->GetIsDashing() && !_isRunning && !player->GetCharaAnim()->GetIsBumped() && !player->GetDisableInput();
}
