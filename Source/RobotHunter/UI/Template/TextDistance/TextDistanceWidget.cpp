#include "TextDistanceWidget.h"

void UTextDistanceWidget::SetDistance(const int& _distance) const
{
	SET_TEXT_STR(textDistance->GetText(), FORMAT(textDistancePlaceholder, _distance));
}
