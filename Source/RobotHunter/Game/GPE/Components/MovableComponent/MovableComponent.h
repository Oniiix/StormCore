// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Game/GPE/Members/CalfActors/WorldChampionsKick/LaunchSettings.h"
#include "MovableComponent.generated.h"

UENUM()
enum class EMovementSource
{
	WCK UMETA(DisplayName = "World Champions Kick"),
	Phazer UMETA(DisplayName = "Phazer Beacon"),
	Seismic UMETA(DisplayName = "Seismic Kick"),
	Cableur UMETA(DisplayName = "Cableur")
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UMovableComponent : public UActorComponent
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunch);
	FOnLaunch onLaunch;


	UPROPERTY(EditAnywhere, Category="Custom|Property")
	TArray<EMovementSource> movementSources;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Launch")
	float weight = 30;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Launch")
	TSubclassOf<ULaunchSettings> launchSettings = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Custom|Property|Launch")
	TObjectPtr<ULaunchSettings> currentLaunchSettings = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float snapOffset = 0;

public:
	FORCEINLINE FOnLaunch& OnLaunch() {	return onLaunch; }

	FORCEINLINE float GetSnapOffset() const { return snapOffset; }
	FORCEINLINE bool IsAffectBy(const EMovementSource& _source) { return movementSources.Contains(_source); }

	UMovableComponent();

	void Launch(const FVector& _direction);

protected:
	virtual void BeginPlay() override;
};
