// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <RobotHunter/CustomActor.h>
#include <Components/BoxComponent.h>
#include "PowerLine.generated.h"




class AStormFlower;

UCLASS()
class ROBOTHUNTER_API APowerLine : public ACustomActor
{
	GENERATED_BODY()
#pragma region Property
private:
	UPROPERTY(EditAnywhere)TObjectPtr<UBoxComponent> areaEffect = nullptr;
	UPROPERTY(VisibleAnywhere ,Category = "PowerLine") TArray<AStormFlower*> flowerConnect = TArray<AStormFlower*>();
	UPROPERTY(EditAnywhere, Category = "PowerLine") float LaunchForce = 1000;
	UPROPERTY(EditAnywhere, Category = "PowerLine") float LaunchUpForce = 1000;	
	UPROPERTY(ReplicatedUsing = OnRep_DistanceTwoFlower) float distanceTwoFlower = 0.0f;
#pragma endregion

	
public:	
	// Sets default values for this actor's properties
	APowerLine();

#pragma region Methods
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void DrawDebug() override ;
private:
	void Init();
	UFUNCTION() void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	/// <summary>
	/// Set the size of the Trigger Box so that it hits both points.
	/// </summary>
	/// <param name="_from">Coordinates of the first point</param>
	/// <param name="_to">Coordinates of the second point</param>
	void InitLonger(FVector _from, FVector _to);
	/// <summary>
	/// adds in the list the flowers to which the actor is linked
	/// </summary>
	/// <param name="_a">First flower</param>
	/// <param name="_b">Second flower</param>
	void ConnectFlower(AStormFlower* _a, AStormFlower* _b);
private:
	/// <summary>
	/// Destroyed The actor if one of the two flowers in the list is not activated
	/// </summary>
	void DestroyActor();
	void Debug();
	UFUNCTION() void OnRep_DistanceTwoFlower();
#pragma endregion

};
