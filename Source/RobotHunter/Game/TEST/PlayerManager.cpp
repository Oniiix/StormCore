


#include "PlayerManager.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/CustomPlayerController.h"

void UPlayerManager::Init()
{
	controllerCount = GetWorld()->GetNumPlayerControllers();
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(Iterator->Get());
		if (PlayerController)
		{
			if (PlayerController->GetPawn())
			{
				players.Add(Cast<AMainCharacter>(PlayerController->GetPawn()));
				LOG_COLOR(PLAYER_MANAGER_USE_LOG, PlayerController->GetPawn()->GetName(), GREEN);
			}
			else
				PlayerController->OnPawnRegistered().AddUniqueDynamic(this, &UPlayerManager::PlayerJoin);
		}
	}
	if (controllerCount == players.Num())
	{
		onAllPlayerJoin.Broadcast();
		LOG_COLOR(PLAYER_MANAGER_USE_LOG, "On a tout chef", FColor::Emerald);
	}
}

void UPlayerManager::PlayerJoin(AMainCharacter* _chara)
{
	if (_chara)
	{
		players.Add(_chara);
		LOG_COLOR(PLAYER_MANAGER_USE_LOG, _chara->GetName(), GREEN);

		if (controllerCount == players.Num())
		{
			LOG_COLOR(PLAYER_MANAGER_USE_LOG, "On a tout chef", FColor::Emerald);
			onAllPlayerJoin.Broadcast();
		}
	}
}
