#include "BaseFrame.h"
#include <RobotHunter/Utils/FileLogger.h>

void UBaseFrame::Show()
{
	if (!IsInViewport() && !HasParent())
	{
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Add to viewport");
		AddToViewport();
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UBaseFrame::Hide()
{
	if (!IsInViewport())
	{
		return;
	}
	SetVisibility(ESlateVisibility::Hidden);
}

TObjectPtr<UWidgetSwitcher> UBaseFrame::GetSwitcher() const
{
	return switcher;
}
