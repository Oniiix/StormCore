// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/InteractableActor.h"
#include "RobotHunter/Game/GPE/Members/MembersActor.h"
#include "Components/WidgetComponent.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "Components/SphereComponent.h"
#include "MemberGiver.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AMemberGiver : public AInteractableActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Member", meta = (AllowPrivateAccess))
	TSubclassOf<AMembersActor> memberRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Member", meta = (AllowPrivateAccess))
	TObjectPtr<UWidgetComponent> widget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Member", meta = (AllowPrivateAccess))
	TObjectPtr<USphereComponent> sphereTrigger = nullptr;

	TObjectPtr<ACustomPlayerController> playerController = nullptr;




public:
	AMemberGiver();

	virtual void Interacted(AMainCharacter* _character) override;
	virtual void LaunchAnimation(AMainCharacter* _character) override;
protected:
	virtual void BeginPlay() override;

	virtual void SetInSight(AMainCharacter* _character);
	virtual void SetNotInSight();


private:
	UFUNCTION() void OnEnterMemberGiverZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnExitMemberGiverZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
