// Fill out your copyright notice in the Description page of Project Settings.
#include "HUBInteractableActor.h"
#include "../../../CCC/Character/MainCharacter.h"



AHUBInteractableActor::AHUBInteractableActor()
{
	camera = CreateDefaultSubobject<UCustomCameraComponent>("Camera");
	camera->SetupAttachment(RootComponent);
	AddOwnedComponent(camera);

	mainCharacter = nullptr;
	linkedToFSM = false;
}

void AHUBInteractableActor::BeginPlay()
{
	interactionFeedback->SetText(FText::FromString(name));
	Super::BeginPlay();
}

void AHUBInteractableActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

//void AHUBInteractableActor::Interacted(AMainCharacter* _character)
//{
//	Super::Interacted(_character);
//	FTimerHandle blendTimer;
//	LOG_COLOR_TIME(HUB_INTERACTABLE_USE_LOG, name + " !", GREEN, 5);
//	mainCharacter = _character;
//
//	if (mainCharacter)
//	{
//		mainCharacter->SetInMenu(true);
//
//		if (camera)
//		{
//			camera->UpdateCurrentSettings(camera->GetCurrentSettingsKey());
//
//			UCustomCameraComponent* _cameraMC = mainCharacter->GetCameraComponent();
//
//			if (_cameraMC)
//				camera->SetFOV(_cameraMC->GetCameraSettings().fieldOfView);
//
//			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, camera->GetBlendSpeed());
//			GetWorldTimerManager().SetTimer(blendTimer, this, &AHUBInteractableActor::CallOnEndBlend, camera->GetBlendSpeed(), false);
//		}
//	}
//}
void AHUBInteractableActor::Interacted_Internal()
{

	FTimerHandle blendTimer;
	LOG_COLOR_TIME(HUB_INTERACTABLE_USE_LOG, name + " !", GREEN, 5);
	mainCharacter = player;

	if (mainCharacter)
	{
		mainCharacter->SetInMenu(true);
		mainCharacter->GetInteractionComponent()->SetIsInInteraction(true);
		mainCharacter->OnQuitMenu().AddUniqueDynamic(this, &AHUBInteractableActor::OnQuitMenu);

		if (camera)
		{
			camera->UpdateCurrentSettings(camera->GetCurrentSettingsKey());

			UCustomCameraComponent* _cameraMC = mainCharacter->GetCameraComponent();

			if (_cameraMC)
				camera->SetFOV(_cameraMC->GetCameraSettings().fieldOfView);

			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, camera->GetBlendDuration());
			GetWorldTimerManager().SetTimer(blendTimer, this, &AHUBInteractableActor::CallOnEndBlend, camera->GetBlendDuration(), false);
		}
	}

	Super::Interacted_Internal();
}

void AHUBInteractableActor::CallOnEndBlend()
{
	onBlendEnd.Broadcast();	
}

void AHUBInteractableActor::OnQuitMenu()
{
	mainCharacter->OnQuitMenu().RemoveDynamic(this, &AHUBInteractableActor::OnQuitMenu);
	mainCharacter->GetInteractionComponent()->SetIsInInteraction(false);
}
