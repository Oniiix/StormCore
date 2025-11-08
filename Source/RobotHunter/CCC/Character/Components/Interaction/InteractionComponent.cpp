#include "InteractionComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"
#include "RobotHunter/Utils/DebugUtils.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	interactionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	interactionSphere->SetGenerateOverlapEvents(true);
	actorsInRange = TMap<FString, AInteractableActor*>();

	detectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	detectionSphere->SetGenerateOverlapEvents(true);
	actorsInSight = TMap<FString, AInteractableActor*>();

	carryableLifeSpanTimer = FTimerHandle();
}

#pragma region Editor
#if WITH_EDITOR
void UInteractionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool _canUpdateInteractionSphere = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UInteractionComponent, interactionRadius);

	if (_canUpdateInteractionSphere)
		UpdateInteractionSphere();

	const bool _canUpdateDetectionSphere = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UInteractionComponent, detectionRadius);
	if (_canUpdateDetectionSphere)
		detectionSphere->SetSphereRadius(detectionRadius);




	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
void UInteractionComponent::UpdateInteractionSphere()
{
	interactionSphere->SetSphereRadius(interactionRadius);
}
#pragma endregion


#pragma region Init
void UInteractionComponent::InitInteraction(AMainCharacter* _character)
{
	chara = _character;

	FTimerHandle _initTimer;
	chara->GetWorld()->GetTimerManager().SetTimer(_initTimer, this, &UInteractionComponent::SetupSphere, 0.5f);
}

void UInteractionComponent::SetupSphere()
{
	if (chara->GetIsOwner())
	{
		TArray<AActor*> _actors;

		detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnDetectionBeginOverlap);
		detectionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnDetectionEndOverlap);
		detectionSphere->GetOverlappingActors(_actors, AInteractableActor::StaticClass());

		for (AActor* _actor : _actors)
			OnDetectionBeginOverlap(nullptr, _actor, nullptr, 0, false, FHitResult());


		interactionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnInteractionBeginOverlap);
		interactionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnInteractionEndOverlap);
		interactionSphere->GetOverlappingActors(_actors, AInteractableActor::StaticClass());

		for (AActor* _actor : _actors)
			OnInteractionBeginOverlap(nullptr, _actor, nullptr, 0, false, FHitResult());
	}
}

void UInteractionComponent::SetupAttachment(USceneComponent* _root)
{
	interactionSphere->SetupAttachment(_root);

	detectionSphere->SetupAttachment(_root);
}
#pragma endregion


#pragma region IsInInteraction
void UInteractionComponent::SetIsInInteraction(const bool _isInInteraction)
{
	isInInteraction = _isInInteraction;
	if (isInInteraction)
		HideAll();
	else
		ShowAll();
}

void UInteractionComponent::HideAll()
{
	if (closestActor)
	{
		closestActor->SetClosest(false);
		GetWorld()->GetTimerManager().ClearTimer(checkTimer);
	}

	for (TPair<FString, AInteractableActor*> _interactable : actorsInSight)
	{
		if (_interactable.Value)
			_interactable.Value->SetNotInSight();
	}
}

void UInteractionComponent::ShowAll()
{
	for (TPair<FString, AInteractableActor*> _interactable : actorsInSight)
	{
		if (_interactable.Value)
 			_interactable.Value->SetInSight(Cast<AMainCharacter>(GetOwner()));
	}
	if (actorsInRange.Num() > 0)
	{
		closestActor = nullptr;
		CheckActorsInRange();
		GetWorld()->GetTimerManager().SetTimer(checkTimer, this, &UInteractionComponent::CheckActorsInRange, 0.1f, true);
	}
}
#pragma endregion

void UInteractionComponent::CheckActorsInRange()
{
	const FVector& _ownerLoc = GetOwner()->GetActorLocation();
	FVector _actorLoc = FVector();
	float _shortestDistance = float();
	int _index = 0;

	AInteractableActor* _oldClosestActor = closestActor;


	AInteractableActor* _interactable = nullptr;

	for (TPair<FString, AInteractableActor*> _pair : actorsInRange)
	{
		_interactable = _pair.Value;

		if (_interactable)
		{
			_actorLoc = _interactable->GetActorLocation();
			const float _distance = FVector::Distance(_ownerLoc, _actorLoc);

			if (_index == 0 || _shortestDistance > _distance)
			{
				_shortestDistance = _distance;
				closestActor = _interactable;
			}
		}
		_index++;
	}
	if (_oldClosestActor != closestActor)
	{
		if (_oldClosestActor)
			_oldClosestActor->SetClosest(false);
		closestActor->SetClosest(true);
	}
}

#pragma region Overlap
void UInteractionComponent::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<AInteractableActor>())
	{
		AInteractableActor* _interactable = Cast<AInteractableActor>(OtherActor);

		if (_interactable->GetCanBeDetected())
		{
			actorsInRange.Add(_interactable->GetName(), _interactable);


			if (actorsInRange.Num() == 1)
			{
				closestActor = _interactable;
				closestActor->SetClosest(true);
				GetWorld()->GetTimerManager().SetTimer(checkTimer, this, &UInteractionComponent::CheckActorsInRange, 0.1f, true);
			}
		}
	}
}

void UInteractionComponent::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<AInteractableActor>())
	{
		AInteractableActor* _interactable = Cast<AInteractableActor>(OtherActor);

		if (_interactable->GetCanBeDetected())
		{
			actorsInRange.Remove(_interactable->GetName());

			if (_interactable == closestActor)
			{
				closestActor = nullptr;
				_interactable->SetClosest(false);
			}

			if (actorsInRange.Num() == 0)
			{
				GetWorld()->GetTimerManager().ClearTimer(checkTimer);
				closestActor = nullptr;
			}
		}
	}
}


void UInteractionComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<AInteractableActor>())
	{
		AInteractableActor* _interactable = Cast<AInteractableActor>(OtherActor);

		if (_interactable->GetCanBeDetected())
		{
			if (!isInInteraction)
				_interactable->SetInSight(Cast<AMainCharacter>(GetOwner()));
			actorsInSight.Add(_interactable->GetName(), _interactable);
		}
	}
}
void UInteractionComponent::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<AInteractableActor>())
	{
		AInteractableActor* _interactable = Cast<AInteractableActor>(OtherActor);

		if (_interactable->GetCanBeDetected())
			_interactable->SetNotInSight();
		actorsInSight.Remove(_interactable->GetName());
	}
}
#pragma endregion


#pragma region Add/Remove
void UInteractionComponent::AddActor(const FString& _key, AInteractableActor* _actor)
{
	if (!actorsInRange.Contains(_key))
	{
		actorsInRange.Add(_key, _actor);
		if (actorsInRange.Num() == 1)
		{
			closestActor = actorsInRange[_key];
			closestActor->SetClosest(true);
			GetWorld()->GetTimerManager().SetTimer(checkTimer, this, &UInteractionComponent::CheckActorsInRange, 0.1f, true);
		}
	}
	if (!actorsInSight.Contains(_key))
		actorsInSight.Add(_key, _actor);
	
}

void UInteractionComponent::RemoveActor(const FString& _key)
{
	if (actorsInRange.Contains(_key))
	{
		closestActor = closestActor == actorsInRange[_key] ? nullptr : closestActor;
		actorsInRange.Remove(_key);
	}
	if (actorsInSight.Contains(_key))
		actorsInSight.Remove(_key);
}
#pragma endregion


void UInteractionComponent::Interact()
{
	if (!canInteract) return;


	if (!closestActor)
		return;

	AActor* _owner = GetOwner();

	if (!_owner)
	{
		LOG_ERROR(INTERACTION_COMPONENT_USE_LOG, "_owner is null !!");
		return;
	}

	if (!chara)
	{
		LOG_ERROR(INTERACTION_COMPONENT_USE_LOG, "_chara is null !!");
		return;
	}

	if (!closestActor->CanInteract(chara))
		return;


	if (closestActor->GetCanBeCarry())
	{
		if(!carryableActor)
		{
			closestActor->Interacted(chara);
			carryableActor = closestActor;
		}
	}
	else
		closestActor->Interacted(chara);
}


#pragma region Carry
void UInteractionComponent::DropInteractable()
{
	AActor* _owner = GetOwner();
	AMainCharacter* _chara = Cast<AMainCharacter>(_owner);
	if (!carryableActor)return;
	carryableActor->Drop(_chara);
	carryableActor = nullptr;
}

void UInteractionComponent::DestroyInteractable(const float _lifeSpan)
{
	if (carryableActor)
	{
		const FTimerDelegate& _delegate = FTimerDelegate::CreateLambda([&]() 
			{
				AMainCharacter* _chara = Cast<AMainCharacter>(GetOwner());

				if (_chara)
				{
					UInventoryComponent* _inventoryComp = _chara->GetComponentByClass<UInventoryComponent>();

					if (_inventoryComp)
						_inventoryComp->DropEgg();

					AMembersActor* _member = _chara->GetUsingRightArm();

					if (_member)
						_member->SetIsActivated(true);

					_member = _chara->GetUsingLeftArm();

					if (_member)
						_member->SetIsActivated(true);

					UCharaAnim* _anim = _chara->GetCharaAnim();

					if (_anim)
						_anim->SetHasCarry(false); 
				}
			});


		FTimerManager& _tm = GetWorld()->GetTimerManager();

		if (_lifeSpan > 0.0f)
		{
			_tm.SetTimer(carryableLifeSpanTimer, _delegate, _lifeSpan, FTimerManagerTimerParameters());
			carryableActor->SetLifeSpan(_lifeSpan);
		}
		else
		{
			if (_tm.IsTimerActive(carryableLifeSpanTimer))
				_tm.ClearTimer(carryableLifeSpanTimer);

			carryableActor->SetLifeSpan(0.1f);
			_delegate.ExecuteIfBound();
		}
	}
}

void UInteractionComponent::CarryInteractable()
{
	AActor* _owner = GetOwner();
	AMainCharacter* _chara = Cast<AMainCharacter>(_owner);
	carryableActor->Carry(_chara);
}
#pragma endregion

