#pragma once

#include "CoreMinimal.h"
#include "../ProjectileActor.h"
#include "../../../../Utils/ENUM_AI.h"
#include "NetProjectileActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ANetProjectileActor : public AProjectileActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Stun")
	TMap<TEnumAsByte<EAIType>, float> stunDuration;

public:
	ANetProjectileActor();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
