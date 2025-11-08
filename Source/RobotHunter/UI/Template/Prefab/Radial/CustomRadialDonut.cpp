// Objectif 3D Copyright


#include "CustomRadialDonut.h"

#include "RobotHunter/Utils/DebugUtils.h"


void UCustomRadialDonut::SetPercentMin(const float _percentMin)
{
	CLAMP(_percentMin, 0.f, 1.f);
	imageRoundBar->GetDynamicMaterial()->SetScalarParameterValue("PercentMin", _percentMin);
}

void UCustomRadialDonut::SetPercentMax(const float _percentMax)
{
	CLAMP(_percentMax, 0.f, 1.f);
	imageRoundBar->GetDynamicMaterial()->SetScalarParameterValue("PercentMax", _percentMax);
}

void UCustomRadialDonut::SetColor(const FLinearColor& _color)
{
	imageRoundBar->GetDynamicMaterial()->SetVectorParameterValue("Color", _color);
}
