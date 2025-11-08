#include "MenuState.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../Game/Interactable/Hub/HUBInteractableActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "../../../UI/HUD/GameHUD.h"

void UMenuState::EnterMenuHUBState()
{
	if (player)
	{
		UInteractionComponent* _interactionComp = player->GetInteractionComponent();

		if (_interactionComp)
		{
			AHUBInteractableActor* _hubActor = Cast<AHUBInteractableActor>(_interactionComp->GetClosestActor());

			if (_hubActor && !_hubActor->GetLinkedToFSM())
			{
				_hubActor->GetOnBlendEnd().AddDynamic(this, &UMenuState::EnablePlayerContext);
				LOG_COLOR_TIME(MENU_STATE_USE_LOG,"EVENT ADD DYNAMIC", ORANGE, 10);
				_hubActor->SetLinkedToFSM(true);
			}
		}
	}
}

void UMenuState::BlendEnded()
{
	if (player)
		player->SetInBlend(false);
}

void UMenuState::EnablePlayerContext()
{
	LOG_COLOR_TIME(MENU_STATE_USE_LOG, "ENABLE CONTEXT", ORANGE, 10);
	Super::EnablePlayerContext();
}

void UMenuState::Enter()
{
	RetrievePlayer();
	DisablePlayerContext();
	EnterMenuHUBState();

	UCustomState::Enter();
}

void UMenuState::Exit()
{
	UWorld* _world = GetWorld();
	APlayerController* _controller = _world->GetFirstPlayerController();

	if (_controller)
	{
		if (player)
		{
			player->SetInBlend(true);
			float _blendSpeed = 1.0f;
			UCustomCameraComponent* _camera = player->GetCameraComponent();

			if (_camera)
			{
				_camera->UpdateCurrentSettings(_camera->GetCurrentSettingsKey());
				_blendSpeed = _camera->GetBlendDuration();
			}

			_controller->SetViewTargetWithBlend(player, _blendSpeed);
			FTimerHandle _blendTimer;
			_world->GetTimerManager().SetTimer(_blendTimer, this, &UMenuState::BlendEnded, _blendSpeed, false);
		}

		if (_controller->GetHUD())
			Cast<AMainHUD>(_controller->GetHUD())->SetState(EStateUI::PlayerLobbyUI);
	}

	UCustomState::Exit();
}
