#include "WDG_MiniMap.h"

#include "RobotHunter/UI/HUD/GameHUD.h"
#include "RobotHunter/UI/Menu/Pause/PauseMenu.h"
#include "RobotHunter/Utils/DebugUtils.h"


void UWDG_MiniMap::ShowGamePause()
{
	if (AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD()))
	{
		_hud->SetState(EStateUI::MainPause);
	}
}

void UWDG_MiniMap::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(Map3D_GameBtn, OnClick, UWDG_MiniMap, ShowGamePause);
}

void UWDG_MiniMap::Refresh()
{
	Super::Refresh();
	//gameButton->Callback_OnUnHover();

	Map3D_map3DBtn->Callback_OnSelected();
	
	UGameplayStatics::SetViewportMouseCaptureMode(hud->GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
}

void UWDG_MiniMap::ExitUI()
{
	Super::ExitUI();
	if (AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD()))
	{
		UPauseMenu* _pause = _hud->GetUI<UPauseMenu>(EStateUI::MainPause);
		_pause->GetOnHideMap().Broadcast();
	}
}
