// 

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "HitValueActor.generated.h"

UCLASS()
class ROBOTHUNTER_API AHitValueActor final : public AActor
{
	GENERATED_BODY()

	/**
	 * Life of actor before destroy
	 */
	UPROPERTY(EditAnywhere, Category = "Custom", meta =(UIMin = 0, ClampMin = 0))
	float lifeSpan = 3.f;

	/**
	 * Time before start fade
	 */
	UPROPERTY(EditAnywhere, Category = "Custom", meta =(UIMin = 0, ClampMin = 0))
	float timeToStartFade = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	TObjectPtr<UWidgetComponent> widgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Custom")
	float ResultTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	float damage;

public:
	AHitValueActor();

	void SetHitDamage(int _damage, const bool _critical = false);

private:
	UFUNCTION()
	void StartFade();

	virtual void BeginPlay() override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif WITH_EDITOR
};