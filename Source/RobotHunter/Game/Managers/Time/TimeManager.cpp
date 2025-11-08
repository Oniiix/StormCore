// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"
#include <RobotHunter/Utils/DebugUtils.h>
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include <Net/UnrealNetwork.h>
#include <RobotHunter/SaveSystem/GameplaySave.h>
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/ATH/EndDay/EndDay.h"

ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
	hasAuthority = HasAuthority();
	net = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>();
	if (!net)
	{
		LOG_ERROR(true, "NETNULL");
		return;
	}
	if(!hasAuthority)
		net->ServerRPC_CurrentTime(this);

	GetWorld()->GetTimerManager().SetTimer(dayTimer, this, &ATimeManager::UpdateMinute, (inGameDayDuration/dayTimeValue));
	
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LOG_COLOR_TICK(TIME_MANAGER_USE_LOG, "Il est actuellement : " + FString::FromInt(startHour + inGameHour) + "h et " + FString::FromInt(inGameMinutes) + "min", GREEN);
}

void ATimeManager::UpdateHour()
{
	inGameHour++;
	onNewHourStart.Broadcast(startHour + inGameHour);
	inGameMinutes = 0;
	if (inGameHour == dayTimeValue && hasAuthority)
	{
		onEndDay.Broadcast();
		GetWorldTimerManager().ClearTimer(dayTimer);

		if (AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter()))
		{
			_chara->DisablePlayerContext(_chara->GetCurrentPlayerContext());
		}


		if (AGameHUD* _hud = Cast<AGameHUD>(GETFPC->GetHUD()))
		{
			_hud->SetState(EStateUI::DayEnd);
			_hud->GetCurrentUI<UEndDay>()->Callback_OnDayEnd();
		}
		else
		{
			UCustomGameInstance* _gmInstance = GetWorld()->GetGameInstance<UCustomGameInstance>();
			UOnlineManagerSubsystem* _subSystem = _gmInstance->GetSubsystem< UOnlineManagerSubsystem>();

			SetupDay();
			//_subSystem->StopServer();
			_subSystem->LoadMap(_gmInstance->GetHubLevel().GetAssetName() + "?listen");
			//UGameplayStatics::OpenLevel(GetWorld(), FName(_gmInstance->GetHubLevel().GetAssetName()), false);
		}
	}
}

void ATimeManager::SetupDay()
{
	AMainCharacter* _chara = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	_chara->GetDayManager()->UpdateDay();
	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	CHECK_NULL(_saveManager, "Save manager is null");
	UGameplaySave* _save = _saveManager->GetCurrentSave();
	CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
	_save->SaveGameplay(_chara);
	_save->SaveBonus(_chara);
}

void ATimeManager::UpdateMinute()
{
	inGameMinutes++;
	if (inGameMinutes == 60)
		UpdateHour();
	onUpdateTime.Broadcast(startHour + inGameHour, inGameMinutes);
	GetWorld()->GetTimerManager().SetTimer(dayTimer, this, &ATimeManager::UpdateMinute, (inGameDayDuration / dayTimeValue));
}

