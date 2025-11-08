#include "HitDamageWidget.h"

void UHitDamageWidget::InitWidget(const int _damage)
{
	damageValue->SetColorAndOpacity(FSlateColor(FColor::Red));
	const FString _value = FString::FromInt(_damage);
	damageValue->SetText(FText::FromString(_value));
}
