#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/Game/Interactable/InteractableActor.h"
#include "InteractionComponent.generated.h"

#define INTERACTION_COMPONENT_USE_LOG false


class AMainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<USphereComponent> interactionSphere;
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<USphereComponent> detectionSphere;

	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (UIMin = 10.0f, ClampMin = 10.0f))
	float interactionRadius = 250.0f;
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (UIMin = 10.0f, ClampMin = 10.0f))
	float detectionRadius = 250.0f;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TMap<FString, AInteractableActor*> actorsInRange;
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TMap<FString, AInteractableActor*> actorsInSight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr< AInteractableActor> closestActor = nullptr;


	bool isInInteraction = false;

	//When we are doing an interaction like the animation of the pick-up or having the control of handcar
	bool hasInteracted = false;

	bool canInteract = true;


	UPROPERTY()
	TObjectPtr<AMainCharacter> chara = nullptr;


	UPROPERTY()
	TObjectPtr< AInteractableActor> carryableActor = nullptr;

	FTimerHandle carryableLifeSpanTimer;

	FTimerHandle checkTimer;

public:
	UFUNCTION(BlueprintCallable) void SetIsInInteraction(const bool _isInInteraction);
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetCanInteract(const bool _canInteract) { canInteract = _canInteract; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetHasInteracted(const bool _hasInteracted) { hasInteracted = _hasInteracted; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetHasInteracted() const { return hasInteracted; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetCanInteract() const { return canInteract; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetInteractionRadius() const { return interactionRadius; }
	FORCEINLINE void SetCarryableActor(AInteractableActor* _interactable) { carryableActor = _interactable; }

	FORCEINLINE AInteractableActor* GetClosestActor() const { return closestActor; }
	FORCEINLINE AInteractableActor* GetCarryableActor() const { return carryableActor; }

public:	
	UInteractionComponent();

private:
	void UpdateInteractionSphere();
	UFUNCTION() void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void HideAll();
	void ShowAll();

	UFUNCTION() void CheckActorsInRange();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	void InitInteraction(AMainCharacter* _character);
	UFUNCTION() void SetupSphere();
	void SetupAttachment(USceneComponent* _root);
	void AddActor(const FString& _key, AInteractableActor* _actor);
	void RemoveActor(const FString& _key);
	void Interact();
	void DropInteractable();
	void DestroyInteractable(const float _lifeSpan);
	UFUNCTION(BlueprintCallable) void CarryInteractable();
};
