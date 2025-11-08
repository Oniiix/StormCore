#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/MoveInteractable.h"
#include "EggStockHandcarActor.generated.h"

class ACustomHandcarActor;

UCLASS()
class ROBOTHUNTER_API AEggStockHandcarActor : public AMoveInteractable
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDropEgg);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProperty", meta = (UIMin = 0.1f, ClampMin = 0.1f, AllowPrivateAccess))
	float lifeStockRestoration = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProperty", meta = (UIMin = 0.1f, ClampMin = 0.1f, AllowPrivateAccess))
	float dropEggTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProperty", meta = (UIMin = 0.1f, ClampMin = 0.1f, AllowPrivateAccess))
	float animTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "CustomProperty")
	bool isAtExterior = false;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnDropEgg onDropEgg;

	UPROPERTY()
	TObjectPtr<AMainCharacter> eggPlayer = nullptr;

	UPROPERTY(EditAnywhere)
	FVector offsetTP;	
	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;

	FTimerHandle stopInteractionTimer = FTimerHandle();

public:
	FORCEINLINE void SetHandcar(ACustomHandcarActor* _handcar) { handcar = _handcar; }

	FORCEINLINE bool GetIsAtExterior() const { return isAtExterior; }
	FORCEINLINE FOnDropEgg& GetOnDropEgg() { return onDropEgg; }

public:
	AEggStockHandcarActor();
	
	virtual void Interacted(AMainCharacter* _character) override;

	virtual void Interacted_Internal() override;
	virtual void LaunchAnimation(AMainCharacter* _character) override;
	virtual bool CanInteract(AMainCharacter* _character) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	UFUNCTION() void StopInteraction(const bool _atStop = false, const bool _inAutoMode = false);

};

