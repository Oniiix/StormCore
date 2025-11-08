#pragma once
/*
* Enum pour le HUD 
*/
UENUM(Blueprintable)
enum class EStateUI : uint8
{
	None UMETA(DisplayName = "None"),
	Loading UMETA(DisplayName = "Loading"),

	Global UMETA(DisplayName = "=====[GLOBAL]====="),
	MainMain UMETA(DisplayName = "Main Menu"),
	MainHost UMETA(DisplayName = "Host Menu"),
	MainServer UMETA(DisplayName = "Server List Menu"),
	MainOptions UMETA(DisplayName = "Options Menu"),
	MainPause UMETA(DisplayName = "Pause Menu"),
	MainCredit UMETA(DisplayName = "Credits"),

	UI UMETA(DisplayName = "=====[ATH]====="),
	PlayerLobbyUI UMETA(DisplayName = "Player Lobby ATH"),
	PlayerGameUI UMETA(DisplayName = "Player Game ATH"),
	DraisineUI UMETA(DisplayName = "DraisineATH"),
	Map3D UMETA(DisplayName = "Map3D Fullscreen"),
	BonusUI UMETA(DisplayName = "Bonus"),
	DayEnd UMETA(DisplayName = "End Day/Death Player/Time End"),

	Hub UMETA(DisplayName = "=====[HUB]====="),
	HubStart UMETA(DisplayName= "Start Menu"),
	HubWeather UMETA(DisplayName= "Meteo Menu"),
	HubStock UMETA(DisplayName= "Stock Menu"),

	EndGame UMETA(DisplayName="End Game")
};
