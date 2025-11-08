

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/Hub/HUBInteractableActor.h"
#include "ChooseWeatherInteractable.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AChooseWeatherInteractable : public AHUBInteractableActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Interacted(AMainCharacter* _character) override;


private:
	UFUNCTION() virtual void ShowMenu() override;

};
