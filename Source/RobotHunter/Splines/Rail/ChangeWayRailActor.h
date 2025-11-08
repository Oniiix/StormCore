#pragma once

#include "CoreMinimal.h"
#include "RailActor.h"
#include "Components/BoxComponent.h"
#include "ChangeWayRailActor.generated.h"

UCLASS()
class ROBOTHUNTER_API AChangeWayRailActor : public ARailActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TObjectPtr<UBoxComponent> trigger;

public:
	AChangeWayRailActor();

private:
	UFUNCTION() void OnEnterTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, 
		bool _bFromSweep, const FHitResult& _SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void DrawDebug() override;
};
