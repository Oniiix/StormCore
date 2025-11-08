#include "TransmitterLock.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/Interactable/Loot/Key/KeyLootActor.h"


ATransmitterLock::ATransmitterLock()
{
	canBeDetected = true;
}

void ATransmitterLock::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
	LOG_COLOR(true, "Interacted with Lock", YELLOW);
	if (!_character || !HasKey(_character))
		return;
	Activate();
}

bool ATransmitterLock::HasKey(AMainCharacter* _character)
{
	if (keyToOpen)
	{
		AKeyLootActor* _key = Cast<AKeyLootActor>(_character->GetInteractionComponent()->GetCarryableActor());
		if(_key && _key == keyToOpen)
			return true;
	}
	return false;
}
