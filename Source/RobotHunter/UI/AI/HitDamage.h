#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include "HitDamage.generated.h"

class UHitDamageWidget;

UCLASS()
class ROBOTHUNTER_API AHitDamage final : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> widget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Property")
	float lifeSpan = 2;
	UPROPERTY(EditAnywhere, Category = "Property")
	float speed = 1;

public:
	AHitDamage();

	void InitHitDamage(const int _damage);

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
