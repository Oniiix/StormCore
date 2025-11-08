

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "Map3D.generated.h"

class UPauseMenu;

UCLASS()
class ROBOTHUNTER_API AMap3D : public AActor
{
	GENERATED_BODY()
	// Composant pour le mesh de l'acteur
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* mapMesh;
	// Liste des noms de sockets
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	TMap<FName,FVector> travelPoint;
	UPROPERTY(EditAnywhere, Category = "Properties")
	int cameraSpringArmLength = 100;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACustomHandcarActor> handcar = nullptr;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> actorMarker;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPauseMenu> pauseMenu;

	
	
public:
	FORCEINLINE UStaticMeshComponent* GetMapMesh() const { return mapMesh; }
	FORCEINLINE TMap<FName,FVector> GetTravelPoint() const { return travelPoint; }
	FORCEINLINE AMainCharacter* GetMainCharacter() const { return mainCharacter; }
	FORCEINLINE ACustomHandcarActor* GetHandcar() const { return handcar; }
	FORCEINLINE UPauseMenu* GetPauseMenu() const { return pauseMenu; }
	FORCEINLINE void SetMainCharacter(const TObjectPtr<AMainCharacter>& _mainCharacter) { mainCharacter = _mainCharacter;}
	FORCEINLINE void SetActorMarker (const TArray<TObjectPtr<AActor>>& _actor) { actorMarker = _actor; }
	FORCEINLINE void SetHandcar(const TObjectPtr<ACustomHandcarActor>& _handcar) { handcar = _handcar; }
	FORCEINLINE void AddTravelPoint(const FName _key, const FVector& _value) { travelPoint.Add(_key,_value); }
	FORCEINLINE void SetPauseMenu(const TObjectPtr<UPauseMenu>& _pauseMenu) { pauseMenu = _pauseMenu; }
	FORCEINLINE int GetSpringArmLength() const { return cameraSpringArmLength; }

	
	AMap3D();
	virtual void Tick(float DeltaTime) override;
	void InitMeshSockets();
	void InitActorMarkers();

protected:
	virtual void BeginPlay() override;

};
