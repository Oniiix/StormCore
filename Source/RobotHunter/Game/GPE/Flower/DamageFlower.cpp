#include "DamageFlower.h"
#include "../../../CCC/Character/MainCharacter.h"
#include "../../../Utils/DebugUtils.h"
#include "Net/UnrealNetwork.h"

#define DEBUG_DAMAGE_FLOWER 0
void ADamageFlower::ZoneEffect(TArray<AActor*> _actorWithEffect)
{
	Super::ZoneEffect(_actorWithEffect);
	AppliedDamage(_actorWithEffect);
}
void ADamageFlower::AppliedDamage(TArray<AActor*> _actorWithEffect)
{
	for (size_t i = 0; i < _actorWithEffect.Num(); i++)
	{
		AMainCharacter* _character = Cast<AMainCharacter>(_actorWithEffect[i]);
		if (!_character || !_character->GetLifeComponent())
		{
			if (!_character)
			{
				LOG_ERROR(DEBUG_DAMAGE_FLOWER, "ADamageFlower:: no character");
				return;
			}
			if (!_character->GetLifeComponent())
				LOG_ERROR(DEBUG_DAMAGE_FLOWER, "ADamageFlower::no life component");
			return;
		}
		LOG_WARNING(DEBUG_DAMAGE_FLOWER, "ADamageFlower::AppliedDamage");
		_character->GetLifeComponent()->Damage(damage, this);

	}
}

void ADamageFlower::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADamageFlower, damage);
}