#include "AIDebugInfoWidget.h"
#include "RobotHunter/Utils/DebugUtils.h"

#define STATE_INFO_FIELD_NAME "Text_StateInfo"
#define TARGET_INFO_FIELD_NAME "Text_TargetInfo"
#define THREAT_INFO_FIELD_NAME "Text_ThreatInfo"

void UAIDebugInfoWidget::InitWidget()
{
	stateInfo->SetColorAndOpacity(FSlateColor(BLACK));
	targetInfo->SetColorAndOpacity(FSlateColor(CYAN));
	threatInfo->SetColorAndOpacity(FSlateColor(RED));
}

void UAIDebugInfoWidget::SetStateVisibility(const bool _value)
{
	ESlateVisibility _visibility = _value ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	stateInfo->SetVisibility(_visibility);
}

void UAIDebugInfoWidget::SetTargetVisibility(const bool _value)
{
	ESlateVisibility _visibility = _value ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	targetInfo->SetVisibility(_visibility);
}

void UAIDebugInfoWidget::SetThreatVisibility(const bool _value)
{
	ESlateVisibility _visibility = _value ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	threatInfo->SetVisibility(_visibility);
}

void UAIDebugInfoWidget::SetStateInfo(const FText& _info, const FColor& _color) const
{
	stateInfo->SetText(_info);
	stateInfo->SetColorAndOpacity(FSlateColor(_color));
}

void UAIDebugInfoWidget::SetTargetInfo(const FString& _info) const
{
	targetInfo->SetText(FText::FromString(_info));
}

void UAIDebugInfoWidget::SetThreatInfo(const FString& _info) const
{
	threatInfo->SetText(FText::FromString(_info));
}
