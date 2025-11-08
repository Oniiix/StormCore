#include "StartMenuSettings.h"

FString UStartMenuSettings::GetLevelToTravel() const
{
	return levelToTravel.GetAssetName();
}

FString UStartMenuSettings::GetSector3Name() const
{
	return sector3.GetAssetName();
}
