#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "RobotHunter/UI/AI/WorldLifebarUI.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "LifebarWidgetComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API ULifebarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<ULifeComponent> lifeComponent = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<UWorldLifebarUI> ui = nullptr;

	UPROPERTY(EditAnywhere)
	bool lookAtPlayer = true;

	UPROPERTY()
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;

public:
	ULifebarWidgetComponent();

	UFUNCTION(BlueprintCallable)
	virtual void InitLifebar(ULifeComponent* _lifeComponent);
	UFUNCTION()
	void RefreshUI(AActor* _owner, const int _currentLife, const int _change, AActor* _damageSource);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetWidgetVisibility(const bool _value);

	void CallbackOnHit();
};
