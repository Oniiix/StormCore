// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "../../CCC/Character/MainCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"

AInteractableActor::AInteractableActor()
{
	interactionFeedback = CreateDefaultSubobject<UTextRenderComponent>("InteractionFeedback");
	interactionFeedback->SetupAttachment(RootComponent);

	interactionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("interactableWidgetComponent");
	interactionWidgetComponent->SetupAttachment(RootComponent);
	interactionWidgetComponent->SetCollisionProfileName("IgnoreAll", false);
	interactionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	interactionWidgetComponent->SetDrawSize({400, 400});

	canBeDetected = true;
	isDetected = false;
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	interactionFeedback->SetVisibility(false);
	if (interactionWidgetComponent)
	{
		if (UFloatingUI* _ui = Cast<UFloatingUI>(interactionWidgetComponent->GetUserWidgetObject()))
		{
			_ui->Setup();

			if (widgetUI = Cast<UWorldInteractUI>(_ui))
			{
				widgetUI->SetKey(interactionKey);
				widgetUI->SetInteractOwner(this);
				widgetUI->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	network = GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>();
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRuntime)
	{
		TickUpdateInteractionFeedback();
	}
}

void AInteractableActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	/*AMainCharacter* _character = Cast<AMainCharacter>(OtherActor);

	if (_character)
	{
		UInteractionComponent* _interactionComponent = _character->GetInteractionComponent();

		if (_interactionComponent)
		{
			_interactionComponent->AddActor(GetName(), this);
			interactionFeedback->SetVisibility(true);
			isDetected = true;
		}
	}*/
}

void AInteractableActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	/*AMainCharacter* _character = Cast<AMainCharacter>(OtherActor);

	if (_character)
	{
		UInteractionComponent* _interactionComponent = _character->GetInteractionComponent();

		if (_interactionComponent)
		{
			_interactionComponent->RemoveActor(GetName());
			interactionFeedback->SetVisibility(false);
			isDetected = false;
		}
	}*/
}

void AInteractableActor::TickUpdateInteractionFeedback()
{
	if (!isDetected)
		return;

	UpdateInteractionFeedback(interactionFeedback);
}

void AInteractableActor::UpdateInteractionFeedback(UTextRenderComponent* _feedback)
{
	FVector _cameraLoc = FVector();
	FVector _cameraDir = FVector();
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	UGameplayStatics::DeprojectScreenToWorld(_controller, FVector2D(0.5f, 0.5f), _cameraLoc, _cameraDir);
	FRotator _newRot = UKismetMathLibrary::FindLookAtRotation(_cameraLoc, GetActorLocation());
	//_feedback->SetRelativeRotation(_newRot);
	//interactionWidgetComponent->SetRelativeRotation(FRotator(-_newRot.Pitch, _newRot.Yaw, _newRot.Roll));
}

void AInteractableActor::Interacted(AMainCharacter* _character)
{
	LOG_WARNING(INTERACTABLE_ACTOR_USE_LOG, GetName());
	onInteract.Broadcast(_character);
	if (HasAuthority())
		network->ClientRpc_CallOnInteractEvent(this, _character);
	else
		network->ServerRpc_CallOnInteractEvent(this, _character);
	LaunchAnimation(_character);
}

void AInteractableActor::Drop(AMainCharacter* _character)
{
	if (!canDrop)return;
	//_character->ReactivateMembers({_character->GetUsingCalf(), _character->GetUsingThigh()});
	SetPhysic(true);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	canDrop = false;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	_character->GetInteractionComponent()->AddActor(GetName(), this);
	_character->GetCharaAnim()->SetAnimWeight(-1);
	canBeDetected = true;
}

void AInteractableActor::Carry(AMainCharacter* _character)
{
	if (canDrop)return;
	canDrop = true;
	SetPhysic(false);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorLocation(_character->GetMesh()->GetSocketLocation("R_Hand_Socket"));
	AttachToComponent(_character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "R_Hand_Socket");
	_character->GetInteractionComponent()->RemoveActor(GetName());
	canBeDetected = false;
}

void AInteractableActor::SetInSight(AMainCharacter* _character)
{
	SetIsDetected(true);
	if (widgetUI)
	{
		widgetUI->SetVisibility(ESlateVisibility::Visible);
		
		widgetUI->SetRenderOpacity(1.0f);
		widgetUI->Setup();

		widgetUI->ShowInRange();
	}
}

void AInteractableActor::SetNotInSight()
{
	SetIsDetected(false);

	FVector2D ScreenLocation;
	bool bIsOnScreen = GETFPC->ProjectWorldLocationToScreen(
		GetActorLocation(), 
		ScreenLocation, 
		true
	);
    
	if (!bIsOnScreen)
	{
		widgetUI->SetRenderOpacity(0.0f);
		return;
	}

	if (widgetUI)
		widgetUI->HideInRange();
}

void AInteractableActor::SetClosest(const bool _state)
{
	if (!widgetUI) return;
	
	if (_state)
		widgetUI->ShowInteract();
	else
		widgetUI->ShowInRange();
}

void AInteractableActor::LaunchAnimation(AMainCharacter* _character)
{
	_character->GetCharaAnim()->ClearAllAnimWeight(false);
	_character->GetCharaAnim()->SetHasInteract(true);
	_character->GetInteractionComponent()->SetHasInteracted(true);
}

bool AInteractableActor::CanInteract(AMainCharacter* _character)
{
	return !_character->GetCustomMovementComponent()->IsFalling() && !_character->GetInteractionComponent()->
		GetHasInteracted();
}
