#include "BaseMenu.h"

#include "RobotHunter/InputSystem/InputSubsystem.h"

void UBaseMenu::RegisterEvents()
{
	Super::RegisterEvents();
	//BIND_ACTION(inputSubsystem, OnInputGamepadReceive, UBaseMenu, OnInputReceiveEvent);
	//BIND_ACTION(inputSubsystem, OnInputControllerChange, UBaseMenu, OnInputChangeEvent);

	//LOG_DEBUG(INTSTR(inputSubsystem->OnInputGamepadReceive().GetAllObjects().Num()));
}

/*
void UBaseMenu::Init()
{
	Super::Init();
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Initialize");
}

void UBaseMenu::InitWithDelay()
{
	Super::InitWithDelay();
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Initialize with delay");
}

void UBaseMenu::RegisterEvents()
{
	Super::RegisterEvents();
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Register events");
}*/
void UBaseMenu::ExitUI()
{
	Super::ExitUI();
	isOpened = false;
}

void UBaseMenu::Refresh()
{
	Super::Refresh();
	isOpened = true;
}/*

void UBaseMenu::OnInputChangeEvent(bool bIsGamepad)
{
	if (!isOpened) return;
	if (inputSubsystem->IsGamepad())
	{
		if (currentButtonSelected)
		currentButtonSelected->SetFocus();
		else if (UpDownButtonOrder.Num() > 0)
			UpDownButtonOrder[0]->SetFocus();
	}
	else	
		if (currentButtonSelected)
			currentButtonSelected->Callback_OnUnHover();
	
	GETFPC->SetShowMouseCursor(!bIsGamepad);
}

void UBaseMenu::OnInputReceiveEvent(EInputDirection _direction)
{
	LOG_DEBUG("allo ?")
	if (!isOpened) return;

	LOG_DEBUG(INTSTR(static_cast<int>(_direction)))
	switch (_direction) {
	case EInputDirection::Up:
		HandleVerticalNavigation(-1);
		break;
        
	case EInputDirection::Down:
		HandleVerticalNavigation(1);
		break;
        
	case EInputDirection::Left:
		HandleHorizontalNavigation(-1);
		break;
        
	case EInputDirection::Right:
		HandleHorizontalNavigation(1);
		break;
        
	default: 
		break;
	}
}

void UBaseMenu::HandleVerticalNavigation(int direction)
{
	if (UpDownButtonOrder.Num() == 0) return;
    
	currentVerticalIndexSelection += direction;
	currentVerticalIndexSelection = ((currentVerticalIndexSelection % UpDownButtonOrder.Num()) + UpDownButtonOrder.Num()) % UpDownButtonOrder.Num();
    
	UpDownButtonOrder[currentVerticalIndexSelection]->SetFocus();
}

void UBaseMenu::HandleHorizontalNavigation(int direction)
{
	if (LeftRightButtonOrder.Num() == 0) return;
    
	currentHorizontalIndexSelection += direction;
	currentHorizontalIndexSelection = ((currentHorizontalIndexSelection % LeftRightButtonOrder.Num()) + LeftRightButtonOrder.Num()) % LeftRightButtonOrder.Num();
    
	LeftRightButtonOrder[currentHorizontalIndexSelection]->SetFocus();
}
*/