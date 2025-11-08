// 


#include "HitValue.h"


void UHitValue::NativeConstruct()
{
	Super::NativeConstruct();
	FString _msg = FORMAT(formatText.ToString(), INTSTR(damage));
	SET_TEXT_STR(text->GetText(), _msg);
	text->SetColorAndOpacity(GetColorFromDamage());
}

void UHitValue::Spawn(const float _lifeTime) const
{
	onSpawn.Broadcast(1.f / _lifeTime);
}

void UHitValue::SetTextDamage(const int _damage, const bool _critical)
{
	damage = _damage;
	critical = _critical;
}

FLinearColor UHitValue::GetColorFromDamage() const
{
	if (critical)
		return colorCritical;

	for (const TTuple<int, FLinearColor>& ColorByDamage : colorByDamage)
	{
		if (damage <= ColorByDamage.Key)
			return ColorByDamage.Value;
	}
	return FLinearColor::White;
}
