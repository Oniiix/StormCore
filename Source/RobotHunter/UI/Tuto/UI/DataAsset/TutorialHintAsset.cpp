// 


#include "TutorialHintAsset.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/ATH/LobbyATH.h"
#include "RobotHunter/UI/ATH/TutorialATH.h"
#include "RobotHunter/UI/ATH/Draisine/DraisineATH.h"
#include "RobotHunter/UI/ATH/Game/NewGameATH.h"
#include "RobotHunter/UI/ATH/NewDraisine/NewDraisineATH.h"
#include "RobotHunter/UI/HUD/GameHUD.h"
#include "RobotHunter/UI/Tuto/Condition/ConditionDataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/FileLogger.h"

void UTutorialHintAsset::ValidationProgress(float _value)
{
	onValidateProgress.Broadcast(_value);
}

void UTutorialHintAsset::LaunchTutorial(AMainCharacter* _chara)
{
	AMainHUD* _hud = Cast<AMainHUD>(_chara->GetWorld()->GetFirstPlayerController()->GetHUD());
	CHECK_NULL(_hud, "HUD Is null");

	auto HandleTutorial = [&](auto* Ath)
	{
		if (!isBigPopup)
		{
			CHECK_NULL(InputCondition, "New Input condition is null");

			InputCondition->InitCondition(_chara);
			BIND_ACTION(InputCondition, OnValidateCondition, UTutorialHintAsset, ValidationProgress);

			Ath->GetTutorialATH()->AddTutorial(this, InputCondition);
		}
		else
		{
			Ath->GetTutorialATH()->ShowBigTutorial(bigTutorial);
		}
	};

	if (UNewGameAth* Ath = _hud->GetCurrentUI<UNewGameAth>())
	{
		HandleTutorial(Ath);
	}
	else if (UNewDraisineATH* DraisineAth = _hud->GetCurrentUI<UNewDraisineATH>())
	{
		HandleTutorial(DraisineAth);
	}
	else if (ULobbyATH* LobbyAth = _hud->GetCurrentUI<ULobbyATH>())
	{
		HandleTutorial(LobbyAth);
	}
}
