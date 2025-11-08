#include "KillerZone.h"

#include "Components/BoxComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/Menu/Pause/Game/GameFrame.h"
#include "RobotHunter/UI/Menu/Pause/Game/GameSectionSettings.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include <RobotHunter/SaveSystem/GameplaySave.h>
#include <RobotHunter/UI/HUD/GameHUD.h>
#include <RobotHunter/UI/ATH/EndDay/EndDay.h>

AKillerZone::AKillerZone()
{
	// Initialiser la TriggerBox
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
}

void AKillerZone::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = GetGameInstance<UCustomGameInstance>();
	TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AKillerZone::OnOverlapBegin);
}

void AKillerZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	/*const AMainCharacter* _mainCharacter = Cast<AMainCharacter>(OtherActor);
	if (!_mainCharacter)
		return;*/


	/*GameInstance->CallbackOnLevelChange(ETypeChange::GameToHub);

	GameInstance->OnLevelLoad().AddUniqueDynamic(this, &AKillerZone::LoadMap);*/

	AGameHUD* _hud = GETFPC->GetHUD<AGameHUD>();
	CHECK_NULL(_hud, "HUD is null");
	_hud->SetState(EStateUI::DayEnd);
	if (UEndDay* EndDayUI = _hud->GetCurrentUI<UEndDay>())
	{
		CHECK_NULL(EndDayUI, "End day HUD is null");
		EndDayUI->Callback_OnDeathByKillZone();
	}

	AMainCharacter* _chara = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	_chara->GetDayManager()->UpdateDay();
	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	CHECK_NULL(_saveManager, "Save manager is null");
	UGameplaySave* _save = _saveManager->GetCurrentSave();
	CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
	_save->SaveGameplay(_chara);
	_save->SaveBonus(_chara);
}

/*
void AKillerZone::LoadMap()
{
	GameInstance->OnLevelLoad().RemoveDynamic(this, &AKillerZone::LoadMap);
	const FString levelName = settings->GetLevelToHub();

	AMainCharacter* _chara = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	_chara->GetDayManager()->UpdateDay();
	USaveManagerSubsystem* _saveManager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	CHECK_NULL(_saveManager, "Save manager is null");
	UGameplaySave* _save = _saveManager->GetCurrentSave();
	CHECK_NULL(_save, "Pas de save ! Lance depuis le main menu fdp !");
	_save->SaveGameplay(_chara);
	_save->SaveBonus(_chara);

	if (UOnlineManagerSubsystem* _manager = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
	{
		_manager->StopServer();
		_manager->LoadMap(levelName + "?listen");
	}
}*/
