#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/DamageZoneFlower/DamageZoneFlowerV2.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/DamageZoneFlower/StormFlower/PowerLine/PowerLineV2.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "StormFlowerV2.generated.h"

UCLASS()
class ROBOTHUNTER_API AStormFlowerV2 : public ADamageZoneFlowerV2
{
	GENERATED_BODY()

#pragma region PowerLines
	UPROPERTY(EditAnywhere, Category = "Custom Property|Power Line")
	TSubclassOf<APowerLineV2> powerLineType;

	UPROPERTY(VisibleAnywhere, Category = "Custom Property|Power Line")
	TArray<APowerLineV2*> powerLines;
#pragma endregion
	
#pragma region NearbyFlowers
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nearby Flowers")
	TEnumAsByte<EObjectTypeQuery> flowerLayer;

	UPROPERTY(VisibleAnywhere, Category = "Custom Property|Nearby Flowers")
	TArray<AStormFlowerV2*> nearbyFlowers;
#pragma endregion

#pragma region Modifiers
	UPROPERTY(EditAnywhere, Category = "Custom Property|Modifiers")
	float zoneRadiusModifier;

	float initialZoneRadius;
	float zoneRadius;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Custom Property|Status")
	FStatusParams deactivatedStatus;

public:
	FORCEINLINE bool GetIsActive() const { return isActive; }

public:
	AStormFlowerV2();

private:
	void FindNearbyFlowers();
	void ActivateNearbyFlowers();

	void SpawnPowerLine(AStormFlowerV2* _flowerToLink);
	void ActivatePowerLines();

	void LinkFlower(AStormFlowerV2* _flowerToLink);
	void UnlinkFlower(AStormFlowerV2* _flowerToUnlink);
	int IsFlowerLinked(AStormFlowerV2* _flower);

	UFUNCTION(Server, Unreliable) void ServerRPC_SpawnPowerLine(AStormFlowerV2* _flowerToLink);

protected:
	virtual void OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void Init() override;

	virtual void StartActivePhase() override;
	virtual void StartCooldownPhase() override;

	virtual void ApplyEffectToActorsInArea() override;

	virtual void EnableModifiers() override;
	virtual void DisableModifiers() override;

public:
	UFUNCTION(CallInEditor) void LightningActivation();
	void RemovePowerLine(APowerLineV2* _powerLine);
};
