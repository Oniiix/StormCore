#pragma once

#include "CoreMinimal.h"
#include "../../../../CustomActorComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "DetectionComponent.generated.h"

UCLASS()
class ROBOTHUNTER_API UDetectionComponent : public UCustomActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Detection", meta = (UIMin = 10.0f, ClampMin = 10.0f))
	float detectionRadius;

	UPROPERTY(EditAnywhere, Category = "Detection", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float detectionRate;

	UPROPERTY(EditAnywhere, Category = "Detection|Exit", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float exitTime;

	UPROPERTY(EditAnywhere, Category = "Detection|Visibility")
	TArray<TEnumAsByte<EObjectTypeQuery>> layers;

	UPROPERTY(VisibleAnywhere, Category = "Detection")
	TObjectPtr<USphereComponent> detectionSphere;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> inRangeActors;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> inFrustumActors;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> detectedActors;

	UPROPERTY()
	TObjectPtr<UCameraComponent> cameraComponent;

	/*UPROPERTY()
	TObjectPtr<ULocalPlayer> localPlayer;

	UPROPERTY()
	TObjectPtr<UGameViewportClient> viewportClient;

	FViewport* viewport;
	FSceneView* sceneView;*/

	bool hasDetectedActor;
	FTimerHandle stealthExitTimer;
	FTimerHandle combatExitTimer;

public:
	FORCEINLINE bool GetHasDetectedActor() const { return hasDetectedActor; }
	
public:
	UDetectionComponent();

private:
	void InitComponent();
	//void UpdateSceneView();
	void UpdateDetectionSphere();

	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckActorsInFrustum();
	void CheckActorsVisibility();

	void ExitStealth();
	void ExitCombat();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	virtual void SetupAttachment(USceneComponent* _root) override;
};
