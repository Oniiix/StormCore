#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/InteractableActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "BonusInteractable.generated.h"

UCLASS()
class ROBOTHUNTER_API ABonusInteractable : public AInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TEnumAsByte<EBonusType> bonusType = EBonusType::DefaultBonus;
	
public:
	virtual void Interacted(AMainCharacter* _character);
};
