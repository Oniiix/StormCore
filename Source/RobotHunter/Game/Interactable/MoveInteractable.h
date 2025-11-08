#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Components/SphereComponent.h"
#include "MoveInteractable.generated.h"

#define MOVE_INTERACTABLE_USE_LOG true


UCLASS()
class ROBOTHUNTER_API AMoveInteractable : public AInteractableActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Custom|Move")
		TObjectPtr<USceneComponent> pivotPoint = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Move")
		TObjectPtr<USceneComponent> targetPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Move", meta = (UMin = 1.0f, ClampMin = 1.0f, UMax = 360.0f, ClampMax = 360.0f))
		float interactionAngle = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Move", meta = (UMin = 0.01f, ClampMin = 0.01f))
		float tolerance = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Custom|Move")
		TObjectPtr<USphereComponent> interactionSphere = nullptr;

	bool movePlayer;
	bool rotatePlayer;

	FRotator targetRotation;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Custom|Move")
		TObjectPtr<AMainCharacter> player;

public:
	AMoveInteractable();


	virtual void Interacted(class AMainCharacter* _character) override;
	virtual void Interacted_Internal();

	virtual bool CanInteract(AMainCharacter* _character) override;
protected:
	virtual void Tick(float _deltaTime) override;

	virtual void DrawDebug() override;
private:
	void MovePlayerToPosition();
	void RotatePlayerTowardsPosition();
};
