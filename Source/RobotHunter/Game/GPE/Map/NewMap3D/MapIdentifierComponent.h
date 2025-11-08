

#pragma once

#include "CoreMinimal.h"
#include "Map3D_Manager.h"
#include "Components/ActorComponent.h"
#include "MapIdentifierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UMapIdentifierComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map Manager")
	UMap3D_Manager* mapManager;
	UPROPERTY()
	TObjectPtr<AMainCharacter> mainCharacter;

public:	
	// Sets default values for this component's properties
	UMapIdentifierComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegisterWithManager();


		
};
