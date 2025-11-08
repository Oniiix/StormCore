#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/MoveInteractable.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "RobotHunter/Game/GPE/Handcar/HandcarDataAsset.h"
#include "Components/WidgetComponent.h"
#include "RobotHunter/AI/Component/Widget/LifebarWidgetComponent.h"
#include "LifeStockHandcarActor.generated.h"

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API ALifeStockHandcarActor : public AMoveInteractable
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransferLife, bool, isTransferring);

	UPROPERTY(EditAnywhere, Category = "CustomProperty", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float lifeExchangeValue;

	UPROPERTY(EditAnywhere, Category = "CustomProperty", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float lifeExchangeRate;

	UPROPERTY(EditAnywhere, Category = "CustomProperty")
	bool isAtExterior;

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULifebarWidgetComponent> lifeStockWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULifeComponent> lifeComponent;


	UPROPERTY()
	TObjectPtr<AMainCharacter> lifePlayer;


	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnTransferLife onTransferLife;

	FTimerHandle lifeExchangeTimer;

public:
	FORCEINLINE void SetLifeComponent(ULifeComponent* _lifeComp) 
	{ 
		lifeComponent = _lifeComp;

		if (lifeComponent)
		{
			lifeStockWidget->InitLifebar(lifeComponent);
			lifeComponent->InitLife();
		}
	}

	FORCEINLINE bool GetIsAtExterior() const { return isAtExterior; }
	FORCEINLINE FOnTransferLife& GetOnTransferLife() { return onTransferLife; }

public:
	ALifeStockHandcarActor();

protected:
	virtual void BeginPlay() override;

private:
	void TransferLife();

public:
	virtual void Interacted(AMainCharacter* _character) override;
	virtual void LaunchAnimation(AMainCharacter* _character) override;
	virtual bool CanInteract(AMainCharacter* _character) override;

	virtual void Interacted_Internal() override;


	void AddLifeInStock(const bool _lifeAdded);
	void UpdateValuesFromDataAsset(UHandcarDataAsset* _da);
	UFUNCTION() void StopInteraction(const bool _atStop = false, const bool _inAutoMode = false);
};
