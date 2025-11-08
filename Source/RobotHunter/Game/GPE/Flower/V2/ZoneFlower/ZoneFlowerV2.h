#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Flower/V2/FlowerV2.h"
#include "Components/ShapeComponent.h"
#include "ZoneFlowerV2.generated.h"

UCLASS(Abstract)
class ROBOTHUNTER_API AZoneFlowerV2 : public AFlowerV2
{
	GENERATED_BODY()
	
protected:
#pragma region Effect

#pragma region Duration
	UPROPERTY(EditAnywhere, Category = "Custom Property|Effect|Duration")
	bool useRandomEffectDuration; 
	
	UPROPERTY(EditAnywhere, Category = "Custom Property|Effect|Duration", meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "!useRandomEffectDuration", EditConditionHides))
	float effectDuration;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Effect|Duration", meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "useRandomEffectDuration", EditConditionHides))
	float minEffectDuration;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Effect|Duration", meta = (UIMin = 0.1f, ClampMin = 0.1f, EditCondition = "useRandomEffectDuration", EditConditionHides))
	float maxEffectDuration;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Custom Property|Effect|Area")
	TObjectPtr<UShapeComponent> effectArea;

	UPROPERTY(VisibleAnywhere, Category = "Custom Property|Effect|Area")
	TArray<AActor*> actorsInArea;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Effect", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float effectRate;

	FTimerHandle effectTimer;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Custom Property|Modifiers")
	bool disableModifiers;

public:
	AZoneFlowerV2();

private:
	void InitEffectTimer();
	void ClearEffectTimer();

protected:
	UFUNCTION() virtual void OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Init() override;
	virtual void StartActivePhase() override;
	virtual void StartCooldownPhase() override;

	virtual void ApplyEffectToActorsInArea() {}
};


