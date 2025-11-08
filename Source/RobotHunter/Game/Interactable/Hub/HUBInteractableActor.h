// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MoveInteractable.h"
#include <RobotHunter/CCC/Camera/Components/CustomCameraComponent.h>
#include "HUBInteractableActor.generated.h"

#define HUB_INTERACTABLE_USE_LOG false


UCLASS()
class ROBOTHUNTER_API AHUBInteractableActor : public AMoveInteractable
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlendEnd);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCustomCameraComponent> camera = nullptr;

	UPROPERTY(EditAnywhere)
	FString name = "";

	UPROPERTY()
	TObjectPtr<AMainCharacter> mainCharacter;

	bool linkedToFSM;
	FOnBlendEnd onBlendEnd;

public:
	FORCEINLINE void SetLinkedToFSM(const bool _linked) { linkedToFSM = _linked; }

	FORCEINLINE bool GetLinkedToFSM() const { return linkedToFSM; }
 	FORCEINLINE FOnBlendEnd& GetOnBlendEnd() { return onBlendEnd; }
	FORCEINLINE UCustomCameraComponent* GetCameraComponent() const { return camera; }

public:
	AHUBInteractableActor();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	//virtual void Interacted(AMainCharacter* _character) override;

	virtual void Interacted_Internal() override;
	void CallOnEndBlend();

protected:
	virtual void ShowMenu() PURE_VIRTUAL(AHUBInteractableActor::ShowMenu, );


	UFUNCTION() virtual void OnQuitMenu();
};
