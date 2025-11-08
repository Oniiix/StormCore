// 


#include "EndDay.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/SaveSystem/GameplaySave.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/HUD/Settings/FocusSystem.h"
#include "RobotHunter/UI/Menu/Pause/Game/GameSectionSettings.h"


void UEndDay::InitWithDelay()
{
	Super::InitWithDelay();

	Character = Cast<AMainCharacter>(GETFPC->GetPawn());
	CHECK_NULL(Character, "Character is null");

	currentDay = Character->GetDayManager()->GetCurrentDay();
}

void UEndDay::LoadMap()
{
	UCustomGameInstance* GameInstance = GetGameInstance<UCustomGameInstance>();
	GameInstance->OnLevelLoad().RemoveDynamic(this, &UEndDay::LoadMap);
	const FString levelName = settings->GetLevelToHub();
	if (UOnlineManagerSubsystem* _manager = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
	{
		//on le fait dans le Init du Hub Gamemode mtn 
		//_manager->StopServer();
		_manager->LoadMap(levelName + "?listen");
	}
}

void UEndDay::BackToHub() const
{
	UCustomGameInstance* GameInstance = GetGameInstance<UCustomGameInstance>();
	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UEndDay::LoadMap);

	GameInstance->CallbackOnLevelChange(ETypeChange::GameToHub);

	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	CHECK_NULL(_saveManager, "Save manager is null");
	AMainCharacter* _character = Cast<AMainCharacter>(GETFPC->GetPawn());
	CHECK_NULL(_character, "Character is null");
	UGameplaySave* _save = _saveManager->GetCurrentSave();
	CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
	_save->SaveBonus(_character);
	_character->GetDayManager()->UpdateDay();
	_save->SaveGameplay(_character);
	_saveManager->SaveCurrent(false);
}

void UEndDay::DisableInputCharacter()
{
	CALLBACK_LAMBDA_AFTER_TIME(0.2f,
	                           if (AMainCharacter* _character = Cast<AMainCharacter>(GETFPC->GetCharacter()))
	                           {
	                           //LOG_DEBUG("Disable input");
	                           _character->DisablePlayerContext(_character->GetCurrentPlayerContext());

	                           hud->GetFocusSystem()->SetFocusTo(EFocusMode::UI,_character, false, false);
	                           });
}

bool UEndDay::IsLastDay() const
{
	return currentDay == 3;
}

void UEndDay::BackToMain()
{
	UCustomGameInstance* GameInstance = GetGameInstance<UCustomGameInstance>();
	GameInstance->OnLevelLoad().RemoveDynamic(this, &UEndDay::BackToMain);

	//if (UOnlineManagerSubsystem* _onlineManager = GetWorld()->GetGameInstance()->GetSubsystem<
	//	UOnlineManagerSubsystem>())
	//{
	//	_onlineManager->QuitServer();
	//}
	
	UGameplayStatics::OpenLevel(GetWorld(), FName(GameInstance->GetMainMenuLevel().GetAssetName()));
}

void UEndDay::DeleteSave() const
{
	USaveManagerSubsystem* _save = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (_save->DeleteSave(_save->GetIndexOfSave()))
	{
		UCustomGameInstance* GameInstance = GetGameInstance<UCustomGameInstance>();
		GameInstance->OnLevelLoad().AddUniqueDynamic(this, &UEndDay::BackToMain);

		GameInstance->CallbackOnLevelChange(ETypeChange::GameToMain);
	}
}

void UEndDay::RegisterEvents()
{
	Super::RegisterEvents();
	onDeathPlayer.AddUniqueDynamic(this, &UEndDay::DisableInputCharacter);
	onDayNext.AddUniqueDynamic(this, &UEndDay::DisableInputCharacter);
	onDayEnd.AddUniqueDynamic(this, &UEndDay::DisableInputCharacter);
	onDeathByKillZone.AddUniqueDynamic(this, &UEndDay::DisableInputCharacter);
}
