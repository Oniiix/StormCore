#include "LootActor.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../CCC/Character/Animations/CharaAnim.h"

ALootActor::ALootActor()
{
}

void ALootActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
	LOG_COLOR(LOOT_USE_LOG, dropName, RED);
}

void ALootActor::LaunchAnimation(AMainCharacter* _character)
{
	_character->GetCharaAnim()->SetAnimWeight(1, false);
	_character->GetCharaAnim()->SetHasPickUp(true);
	//_character->GetInteractionComponent()->SetHasInteracted(true);
}
