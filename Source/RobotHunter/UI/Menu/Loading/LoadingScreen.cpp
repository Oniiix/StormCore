#include "LoadingScreen.h"


void ULoadingScreen::SetLoadPercent(const float _percent) const
{
	loadingProgressBar->SetPercent(NORMALIZE(_percent, 0.f, 100.f));
}
