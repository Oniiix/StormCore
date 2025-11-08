// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"
#include "BeaconActor.generated.h"

USTRUCT()
struct FTeleportationData
{
	GENERATED_USTRUCT_BODY()

	TObjectPtr<AActor> target = nullptr;
	TObjectPtr<UMovableComponent> comp = nullptr;

	FVector direction = FVector::ZeroVector;

	FTeleportationData(const TObjectPtr<AActor>& target, const FVector& direction, const TObjectPtr<UMovableComponent>& _comp)
		: target(target), comp(_comp), direction(direction)
	{
	}
	FTeleportationData() = default;
};

#define BEACON_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API ABeaconActor : public ACustomActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<TEnumAsByte<EObjectTypeQuery>> detectionLayers;
	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<TEnumAsByte<EObjectTypeQuery>> snapLayers;

public:	
	ABeaconActor();
	virtual void BeginPlay() override;
	TArray<FTeleportationData> GetActorAround(const float _radius);
	void TeleportAllActors(const TArray<FTeleportationData>& _teleportationData);

	void TeleportActor(FVector& _newLoc, const FTeleportationData& _teleportationData);

protected:
	virtual void Tick(float _deltatime) override;

private:
	TObjectPtr<UMeshComponent> GetActorMesh(AActor* _actor);
};
