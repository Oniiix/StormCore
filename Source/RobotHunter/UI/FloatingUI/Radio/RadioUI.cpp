#include "RadioUI.h"


void URadioUI::CallBackOnShow(const TSubclassOf<AWeather>& Weather, const int Day,
                              const TObjectPtr<ARadioActor>& _actor)
{
	SetVisibility(ESlateVisibility::Visible);
	isInitialized = true;
	weather = Weather.GetDefaultObject();
	currentDay = Day;
	RadioGPE = _actor;
	onShow.Broadcast(weather, currentDay);
}

void URadioUI::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}
