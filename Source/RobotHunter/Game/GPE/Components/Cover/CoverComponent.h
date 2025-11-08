

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/AI/Arena/Component/AIRangePosition/AIRangePositionComponent.h"
#include "CoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UCoverComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAIRangePositionComponent> rangeAI = nullptr;
	TObjectPtr<AActor> owner = nullptr;

	FVector actorLocation;
	TArray< FPointRangeArena> listPoint;

	TArray< FPointRangeArena> listFwdPoint;
	TArray< FPointRangeArena> listBackPoint;
	TArray< FPointRangeArena> listRightPoint;
	TArray< FPointRangeArena> listLeftPoint;

	UPROPERTY(EditAnywhere, Category = "Grid|Angle")
	float rightLeftAngle = 40; // Angle de largeur pour les côtés (par défaut 45°)

	UPROPERTY(EditAnywhere, Category = "Grid|Angle")
	float forwardBackwardAngle = 40; // Angle de largeur pour l'avant et l'arrière (par défaut 45°)
	UPROPERTY(EditAnywhere, Category = "Grid|Angle")
	float totalAngle = 90.0f; // Somme totale des deux angles (par défaut 90°)

	float rightLeftCosAngle = 0;
	float forwardBackwardCosAngle = 0;

	int numList = 0;

public:	
	UCoverComponent();

	FORCEINLINE FVector GetActorLocation()const
	{
		return actorLocation;
	}

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Init(UAIRangePositionComponent* _rangeAI);
	void InitDirectionPoint(const FVector& _neighborLocation, const FPointRangeArena& _point);
	FPointRangeArena GetPoint(const FVector& _playerLocation, const FVector& _aiLocation);
};
