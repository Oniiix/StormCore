

#pragma once

#include "CoreMinimal.h"
#include "Map3D.h"
#include "MapPawn.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/UI/Menu/Pause/PauseMenu.h"
#include "RobotHunter/UI/ATH/Game/Map3D/WDG_MiniMap.h"
#include "Map3D_Manager.generated.h"

#define DEBUG_MAP3D_MANAGER true

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UMap3D_Manager : public UActorComponent
{
	GENERATED_BODY()
	

	UPROPERTY(EditAnywhere, Category = "Properties|MiniMap")
	TArray<TSubclassOf<AMap3D>> map3DActor;
	UPROPERTY(EditAnywhere, Category = "Properties|MiniMap")
	TSubclassOf<AMapPawn> map3DPawn;
	UPROPERTY(VisibleAnywhere, Category = "Properties|MiniMap")
	AMapPawn* map3DPawnInstance;
	UPROPERTY(VisibleAnywhere, Category = "Properties|MiniMap")
	TObjectPtr<AMap3D> mapActorInstance;
	UPROPERTY(EditAnywhere,Category = "Properties|MiniMap")
	bool bStartInMainCharacter = false;
	UPROPERTY(EditAnywhere, Category = "Properties|MiniMap", meta = (EditCondition = "!bStartInMainCharacter"))
	FName startSocket;
	UPROPERTY(EditAnywhere, Category = "Properties|MiniMap")
	TArray<TSoftObjectPtr<UWorld>> levelToMapIndex;
	bool mapSpawn = false;
	UPROPERTY()
	TObjectPtr<AMainCharacter> mainCharacter;
	UPROPERTY()
	int currentIndexMap = 0;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> registeredActors;
	FTimerHandle HandleTimer;
	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPauseMenu> pauseMenu;


public:	
	// Sets default values for this component's properties
	UMap3D_Manager();
	FORCEINLINE UPauseMenu* GetPauseMenu() const { return pauseMenu; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitMapVariable();
	UFUNCTION()
	void InitiatePauseMenu();
	// Show the map in render
	UFUNCTION()
	void OnShowMap();
	// Hide the map mesh in render
	UFUNCTION()
	void OnHideMap();
	void SpawnMiniMap();
	void SetMapView();
	UFUNCTION()
	void RegisterActor(AActor* _actorToRegister);
	UFUNCTION()
	void UnregisterActor(AActor* _actorToRemove);

		
};
