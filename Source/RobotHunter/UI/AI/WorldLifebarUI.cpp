#include "WorldLifebarUI.h"


void UWorldLifebarUI::UpdateLifeBar(const float _currentLife, const float _maxLife) const
{
	lifebar->SetPercent(_currentLife / _maxLife);
	SET_TEXT_STR(textLife->GetText(), FORMAT(formatText.ToString(), INTSTR(_currentLife), INTSTR(_maxLife)));
}
