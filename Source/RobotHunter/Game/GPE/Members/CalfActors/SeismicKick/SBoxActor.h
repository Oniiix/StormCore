

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "StoneActor.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "SBoxActor.generated.h"

class ASeismicKickActor;
class USeismicKickDataAsset;

UCLASS()
class ROBOTHUNTER_API ASBoxActor : public AActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSnapToGround, FHitResult, _hitResult);


	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnSnapToGround onSnapToGround;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> box = nullptr;

	TSubclassOf<AStoneActor> stoneRef = nullptr;
	TArray<TEnumAsByte<EObjectTypeQuery>> boxSnapLayer;

	float timeBetweenGap = 0.5f;

	float currentBoxSpeed = 0.0f;

	float ejectionAngle = 45;
	float ejectionForce = 1;

	int damage = 30;

	float bonusDamage;
	float bleedRate;
	FStatusParams bleedBonusParams;

	UPROPERTY(ReplicatedUsing= OnRep_Init)
	TObjectPtr<USeismicKickDataAsset> settings = nullptr;

	UPROPERTY()
	TObjectPtr<AStoneActor> currentStone = nullptr;

	FVector2D finalLocation;

	bool isInit = false;
	FTimerHandle snapTimer;

	TObjectPtr<ASeismicKickActor> weaponOwner = nullptr;

public:
	FORCEINLINE FOnSnapToGround& OnSnapToGround() { return onSnapToGround; }
	FORCEINLINE void SetBonusDamage(const float _bonus) { bonusDamage = _bonus; }

	FORCEINLINE void SetBleedBonusParams(const float _value, const float _duration, const float _bleedRate)
	{
		bleedBonusParams.value = _value;
		bleedBonusParams.duration = _duration;
		bleedRate = _bleedRate;
	}

	ASBoxActor();
	void InitBox(const FVector2D& _finalLocation, const float _chargeRatio, USeismicKickDataAsset* _settings, ASeismicKickActor* _owner);

protected:
	virtual void Tick(float _deltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
private:
	void SnapBox();

	UFUNCTION() void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnRep_Init();

	void AddBleed(AActor* _target);
};
