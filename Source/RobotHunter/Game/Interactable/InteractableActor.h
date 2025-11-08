// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../CustomActor.h"
#include <Components/TextRenderComponent.h>
#include <Components/WidgetComponent.h>
#include "RobotHunter/UI/FloatingUI/WorldInteraction/WorldInteractUI.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "InteractableActor.generated.h"


#define INTERACTABLE_ACTOR_USE_LOG false


class AMainCharacter;
class UNetworkGPEComponent;

UCLASS()
class ROBOTHUNTER_API AInteractableActor : public ACustomActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AMainCharacter*, _character);

protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnInteract onInteract;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Interaction")
	TObjectPtr<UTextRenderComponent> interactionFeedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Interaction")
	TObjectPtr<UWidgetComponent> interactionWidgetComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom|Interaction", meta = (AllowPrivateAccess))
	bool canBeDetected;
	UPROPERTY(EditAnywhere, Category = "Custom|Interaction")
	bool canBeCarry = false;
	bool isDetected;
	bool canDrop = false;

	/** Name of key in inputContext */
	UPROPERTY(EditAnywhere, Category = "Custom|Interaction")
	FText interactionKey = TEXTSTR("Keyboard_Interact");


	UPROPERTY()
	TObjectPtr<UNetworkGPEComponent> network = nullptr;

	UPROPERTY()
	TObjectPtr<UWorldInteractUI> widgetUI = nullptr;

public:
	AInteractableActor();

public:
	FORCEINLINE FOnInteract& OnInteract() { return onInteract; }
	FORCEINLINE void SetIsDetected(const bool _isDetected) { isDetected = _isDetected; }
	FORCEINLINE void SetCanBeDetected(const bool _canBeDetected) { canBeDetected = _canBeDetected; }

	FORCEINLINE bool GetCanBeDetected() const { return canBeDetected; }
	FORCEINLINE bool GetCanBeCarry() const { return canBeCarry; }
	FORCEINLINE UTextRenderComponent* GetInteractionFeedback() const { return interactionFeedback; }
	FORCEINLINE void SetPhysic(const bool& _active) { mesh->SetSimulatePhysics(_active); }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void TickUpdateInteractionFeedback();

	void UpdateInteractionFeedback(UTextRenderComponent* _feedback);

public:
	virtual void ServerRPC_Interacted()
	{
	};
	virtual void Interacted(AMainCharacter* _character);
	virtual void Drop(AMainCharacter* _character);
	virtual void Carry(AMainCharacter* _character);

	virtual void SetInSight(AMainCharacter* _character);
	virtual void SetNotInSight();
	virtual void SetClosest(const bool _state);

	virtual void LaunchAnimation(AMainCharacter* _character);
	virtual bool CanInteract(AMainCharacter* _character);
};
