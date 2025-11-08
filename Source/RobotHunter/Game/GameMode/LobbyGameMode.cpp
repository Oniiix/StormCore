


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#if WITH_EDITORONLY_DATA
#include "CustomEditor/Public/CustomEditorSettings.h"
#endif

void ALobbyGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

    if (AActor* _sessionSetup = UGameplayStatics::GetActorOfClass(GetWorld(), ASessionSetup::StaticClass()))
    {
        LOG_COLOR(true, "Get seamles SessionInfo", YELLOW);
        sessionSetup = Cast<ASessionSetup>(_sessionSetup); // reestablish the linkage
    }
    else
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.Instigator = GetInstigator();
        SpawnInfo.ObjectFlags |= RF_Transient;  // We never want to save team setups into the map

        sessionSetup = GetWorld()->SpawnActor<ASessionSetup>(ASessionSetup::StaticClass(), SpawnInfo);
    }

    if (ACustomGameSession* _gs = Cast<ACustomGameSession>(GameSession))
        _gs->SetSessionSetup(sessionSetup); // cache it in GameState so it's easily accessible client-side
}

void ALobbyGameMode::StartToLeaveMap()
{
   /* Super::StartToLeaveMap();
    AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD());
    CHECK_NULL(_hud, "Hud is null");

    _hud->SetState(EStateUI::PlayerGameUI);*/
}

void ALobbyGameMode::Init()
{

#if WITH_EDITORONLY_DATA // Development
    bUseSeamlessTravel = GetMutableDefault<UCustomEditorSettings>()->GetSeamless();
#else // Ship
    bUseSeamlessTravel = true;
#endif

    Super::Init();
    //if (UOnlineManagerSubsystem* _gis = GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
    //    _gis->StopServer();
}
