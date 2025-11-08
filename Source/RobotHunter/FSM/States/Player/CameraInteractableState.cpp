#include "CameraInteractableState.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../Game/Interactable/CameraInteractableActor.h"

void UCameraInteractableState::LinkCameraInteractableToFSM()
{
	if (player)
	{
		UInteractionComponent* _interactionComp = player->GetInteractionComponent();

		if (_interactionComp)
		{
			ACameraInteractableActor* _hubActor = Cast<ACameraInteractableActor>(_interactionComp->GetClosestActor());

			if (_hubActor && !_hubActor->GetLinkedToFSM())
			{
				_hubActor->GetOnBlendEnd().AddDynamic(this, &UCameraInteractableState::EnablePlayerContext);
				_hubActor->SetLinkedToFSM(true);
			}
		}
	}
}

void UCameraInteractableState::BlendEnded()
{
	if (player)
		player->SetInBlend(false);
}

void UCameraInteractableState::EnablePlayerContext()
{
	Super::EnablePlayerContext();
}

void UCameraInteractableState::Enter()
{
	RetrievePlayer();
	DisablePlayerContext();
	LinkCameraInteractableToFSM();

	UCustomState::Enter();
}

void UCameraInteractableState::Exit()
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
			_world->GetTimerManager().SetTimer(_blendTimer, this, &UCameraInteractableState::BlendEnded, _blendSpeed, false);
		}
	}

	UCustomState::Exit();
}
