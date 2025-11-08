#include "BonusInteractable.h"

void ABonusInteractable::Interacted(AMainCharacter* _character)
{
	if (_character)
	{
		AInteractableActor::Interacted(_character);

		UBonusComponent* _bonusComp = _character->GetBonusComponent();

		if (_bonusComp)
			_bonusComp->AddBonus(bonusType);
	}
}
