


#include "EndZoneActor.h"

#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "Misc/PackageName.h"


AEndZoneActor::AEndZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	RootComponent = triggerBox;
}

void AEndZoneActor::NotifyActorBeginOverlap(AActor* _otherActor)
{
	Super::NotifyActorBeginOverlap(_otherActor);
	if (FPackageName::GetShortName(GetWorld()->GetOutermost()->GetName()) == "P_Intro")
	{
		UnlockAchievement("Knowledgeable");
	}
	gi = GetGameInstance<UCustomGameInstance>();
	if (gi)
	{
		AMainCharacter* _chara = Cast<AMainCharacter>(_otherActor);

		_chara->ResetChara();
		_chara->DisablePlayerContext(_chara->GetCurrentPlayerContext());

		gi->CallbackOnLevelChange(travelChange);
		gi->OnLevelLoad().AddUniqueDynamic(this, &AEndZoneActor::Travel);
	}
}

void AEndZoneActor::Travel()
{
	gi->OnLevelLoad().RemoveDynamic(this, &AEndZoneActor::Travel);

	//UOnlineManagerSubsystem* _online = gi->GetSubsystem<UOnlineManagerSubsystem>();
	//if (_online)
	//	createServer ? _online->CreateServer("Party", EOnlineAccessibilityState::Private, travelMap.GetAssetName() + "?listen") : _online->LoadMap(travelMap.GetAssetName() + "?listen");
	UGameplayStatics::OpenLevel(GetWorld(), FName(travelMap.GetAssetName()));
}


void AEndZoneActor::UnlockAchievement(const FName& _achievementName)
{
	ACustomPlayerController* _pc = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	if (_pc)
		_pc->UpdateLeaderboard(_achievementName,1);
}
