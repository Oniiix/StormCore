#include "DamageZoneFlowerV2.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"

ADamageZoneFlowerV2::ADamageZoneFlowerV2()
{
	damage = 10.0f;
	damageModifier = 2.0f;
	initialDamage = 0.0f;
}

void ADamageZoneFlowerV2::ApplyDamageToActorsInArea()
{
	if (actorsInArea.IsEmpty())
		return;

	const int _num = actorsInArea.Num();
	ULifeComponent* _lifeComp = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_lifeComp = actorsInArea[i]->GetComponentByClass<ULifeComponent>();

		if (_lifeComp)
			_lifeComp->Damage(damage, this);
	}
}

void ADamageZoneFlowerV2::Init()
{
	initialDamage = damage;

	Super::Init();
}

void ADamageZoneFlowerV2::ApplyEffectToActorsInArea()
{
	ApplyDamageToActorsInArea();
}

void ADamageZoneFlowerV2::EnableModifiers()
{
	damage *= damageModifier;
}

void ADamageZoneFlowerV2::DisableModifiers()
{
	damage = initialDamage;
}
