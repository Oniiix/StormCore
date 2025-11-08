#include "CameraInteractableActor.h"
#include "../../CCC/Character/MainCharacter.h"

ACameraInteractableActor::ACameraInteractableActor()
{
	camera = CreateDefaultSubobject<UCustomCameraComponent>("Camera");
	camera->SetupAttachment(RootComponent);
	AddOwnedComponent(camera);

	firstInteraction = true;
	linkedToFSM = false;
}

void ACameraInteractableActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);

	if (firstInteraction)
	{
		if (camera)
		{
			camera->UpdateCurrentSettings(camera->GetCurrentSettingsKey());

			UCustomCameraComponent* _cameraMC = _character->GetCameraComponent();

			if (_cameraMC)
				camera->SetFOV(_cameraMC->GetCameraSettings().fieldOfView);

			FTimerHandle _blendTimer;
			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, camera->GetBlendDuration());
			GetWorldTimerManager().SetTimer(_blendTimer, this, &ACameraInteractableActor::CallOnEndBlend, camera->GetBlendDuration(), false);
		}
	}
}

void ACameraInteractableActor::CallOnEndBlend()
{
	onBlendEnd.Broadcast();
}
