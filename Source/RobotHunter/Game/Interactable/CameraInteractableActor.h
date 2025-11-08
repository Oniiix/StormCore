#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "../../CCC/Camera/Components/CustomCameraComponent.h"
#include "CameraInteractableActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ACameraInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlendEnd);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCustomCameraComponent> camera;

	bool firstInteraction;
	bool linkedToFSM;
	FOnBlendEnd onBlendEnd;

public:
	FORCEINLINE void SetLinkedToFSM(const bool _linked) { linkedToFSM = _linked; }

	FORCEINLINE bool GetLinkedToFSM() const { return linkedToFSM; }
	FORCEINLINE FOnBlendEnd& GetOnBlendEnd() { return onBlendEnd; }

public:
	ACameraInteractableActor();

public:
	virtual void Interacted(AMainCharacter* _character) override;
	void CallOnEndBlend();
};
