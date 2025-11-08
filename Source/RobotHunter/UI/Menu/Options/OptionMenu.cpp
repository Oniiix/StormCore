#include "OptionMenu.h"

#include "OptionFrame.h"


void UOptionMenu::RegisterEvents()
{
	Super::RegisterEvents();
}

void UOptionMenu::Refresh()
{
	Super::Refresh();
	OptionFrame->Refresh();
}
