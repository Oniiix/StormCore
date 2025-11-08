#include "LobbyATH.h"

#include "Reticle/BaseReticle/BaseReticle.h"
#include "RobotHunter/Game/CustomGameInstance.h"

void ULobbyATH::Init()
{
	Super::Init();

	UCustomGameInstance* _instance = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	CHECK_NULL(_instance, "Instance is null");

	/*if (_instance->GetGameStatus() == HUB)
	{
		for (const TTuple<FString, UCooldownPrefab*>& ProgressBar : baseReticle->getProgressBars())
		{
			UCooldownPrefab* _cooldown = ProgressBar.Value;
			_cooldown->SetVisibility(ESlateVisibility::Hidden);
			_cooldown->SetIsEnabled(false);
		}
	}*/
}

void ULobbyATH::ShowSubtitle()
{
	Subtitle->SetRenderOpacity(1);
}

void ULobbyATH::HideSubtitle()
{Subtitle->SetRenderOpacity(0);
}
