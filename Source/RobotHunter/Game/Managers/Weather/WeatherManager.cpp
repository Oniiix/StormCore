// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Net/UnrealNetwork.h>
#include <RobotHunter/Game/CustomGameInstance.h>
#include "RobotHunter/Utils/DebugUtils.h"
#include <RobotHunter/Game/CustomGameState.h>
#include <RobotHunter/CCC/Character/MainCharacter.h>

AWeatherManager::AWeatherManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AWeatherManager::BeginPlay()
{
	Super::BeginPlay();
	InitAllDayWeathers();
	//SelectWeather();


}

void AWeatherManager::InitAllDayWeathers()
{
	allDayWeathers.Add(firstDayWeathers);
	allDayWeathers.Add(secondDayWeathers);
	allDayWeathers.Add(thirdDayWeathers);
}


// Called every frame
void AWeatherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeatherManager::SelectWeather()
{
	if (allWeathers.Num() < 1) return;
	LOG_COLOR_TIME(WEATHER_MANAGER_USE_LOG, "START WEATHER MANAGER", BLUE, 10);
	SetOwner(GetWorld()->GetFirstPlayerController());
	int _length = allDayWeathers.Num();
	LOG_COLOR_TIME(WEATHER_MANAGER_USE_LOG, FString::FromInt(dayManager->GetCurrentDay()), BLUE, 10);
	for (size_t i = 0; i < _length; i++)
	{
		if (dayManager->GetCurrentDay() == i+1)
		{
			weatherIndex = FMath::RandRange(0, allDayWeathers[i].dayWeathers.Num() - 1);
			chosenWeather = allDayWeathers[i].dayWeathers[weatherIndex];
			break;
		}
	}
	//LOG(WEATHER_MANAGER_USE_LOG, "Meteo " + chosenWeather.GetDefaultObject()->GetWeatherName() +" Select");
	SetWeather();
}

void AWeatherManager::ServerRpc_SetClientWeather_Implementation(TSubclassOf < AWeather> _weather)
{
	LOG_COLOR_TIME(WEATHER_MANAGER_USE_LOG, "START WEATHER CLIENT RPC", BLUE, 10);
	if(radio)radio->SetCurrentWeather(_weather);
}


void AWeatherManager::SetWeather()
{
	GetWorld()->GetGameInstance<UCustomGameInstance>()->SetCurrentWeather(chosenWeather);
	if (GetWorld()->GetGameInstance<UCustomGameInstance>()->GetCurrentWeather())
		LOG_COLOR_TIME(WEATHER_MANAGER_USE_LOG, "WEATHER DANS LE GAME INSTANCE", BLUE, 10);
}

void AWeatherManager::ServerRpc_SetWeather_Implementation(float _index)
{
	chosenWeather = allWeathers[_index];
}

void AWeatherManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeatherManager, chosenWeather);
}

void AWeatherManager::InitCharacter(AMainCharacter* _chara)
{
	chara = _chara;
	LOG_COLOR(WEATHER_MANAGER_USE_LOG, chara->GetName(),RED);
	/*chara->OnBeginPlayer().AddDynamic(this, &AWeatherManager::SelectWeather);*/
}
