

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"
#include "StoneActor.generated.h"

USTRUCT()
struct FSeismicActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TObjectPtr<AActor> owner = nullptr;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> collisionComponent = nullptr;

	bool simulatePhysics = false;

	UPROPERTY()
	TObjectPtr<UMovableComponent> moveComp = nullptr;

	FSeismicActorData() = default;

	FSeismicActorData(const TObjectPtr<AActor>& owner, const TObjectPtr<UPrimitiveComponent>& collisionComponent, bool simulatePhysics, const TObjectPtr<UMovableComponent>& moveComp)
		: owner(owner), collisionComponent(collisionComponent), simulatePhysics(simulatePhysics), moveComp(moveComp)
	{
	}
};

UCLASS()
class ROBOTHUNTER_API AStoneActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> mesh = nullptr;

	float initSpeed = 1000;
	float ejectionForce = 30;
	float ejectionAngle = 80;

	UPROPERTY(ReplicatedUsing=OnRep_UpdateInit)
	FVector finalLocation;
	bool isInit = false;

	TArray<FSeismicActorData> touchActor;

public:
	AStoneActor();
	void InitStone(const FVector& _initLocation, class USeismicKickDataAsset* _settings);

protected:
	virtual void Tick(float _deltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
private:
	UFUNCTION() void StoneHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void InitFinish();
	UFUNCTION() void OnRep_UpdateInit();
};
