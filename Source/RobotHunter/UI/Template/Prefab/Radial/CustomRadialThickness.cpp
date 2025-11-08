// Objectif 3D Copyright


#include "CustomRadialThickness.h"
#include "RobotHunter/Utils/DebugUtils.h"


void UCustomRadialThickness::SetThickness(const float _thickness)
{
	CLAMP(_thickness, 0.001f, 1.f);
	imageRoundBar->GetDynamicMaterial()->SetScalarParameterValue("Thickness", _thickness / 2);
}

void UCustomRadialThickness::SetColor(const FLinearColor& _color)
{
	imageRoundBar->GetDynamicMaterial()->SetVectorParameterValue("Color", _color);
}
