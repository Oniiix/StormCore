#include "GameplayMenu.h"

#include "RobotHunter/Utils/DebugUtils.h"


void UGameplayMenu::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(btnInventory, OnClick, UGameplayMenu, ShowInventory);
	BIND_ACTION(btnMap, OnClick, UGameplayMenu, ShowMap);
	BIND_ACTION(btnCodex, OnClick, UGameplayMenu, ShowCodex);
}

void UGameplayMenu::Refresh()
{
	Super::Refresh();
	RefreshUI(menuSwitcher->GetActiveWidgetIndex());
}

void UGameplayMenu::ShowInventory()
{
	SetIndex(0);
}

void UGameplayMenu::ShowMap()
{
	SetIndex(1);
}

void UGameplayMenu::ShowCodex()
{
	SetIndex(2);
}

void UGameplayMenu::SetIndex(const int index) const
{
	if (menuSwitcher->GetChildrenCount() <= index)
	{
		return;
	}
	menuSwitcher->SetActiveWidgetIndex(index);
	RefreshUI(index);
}

void UGameplayMenu::RefreshUI(const int index) const
{
	Cast<UBaseMenu>(menuSwitcher->GetWidgetAtIndex(index))->Refresh();
}
