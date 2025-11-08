// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <Net/UnrealNetwork.h>

#include "RobotHunter/UI/HUD/GameHUD.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "RobotHunter/UI/FloatingUI/Radio/RadioUI.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/ATH/LobbyATH.h"

ARadioActor::ARadioActor()
{
	PrimaryActorTick.bCanEverTick = true;
	//widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	//AddOwnedComponent(widget);
}

void ARadioActor::ServerRpc_SetClientWeather_Implementation(TSubclassOf<AWeather> _weather)
{
	LOG_COLOR_TIME(RADIO_USE_LOG, "MULTI WEATHER ", BLUE, 10);
	currentWeather = _weather;
}

void ARadioActor::SetCurrentWeather(TSubclassOf<AWeather> _weather)
{
	if (HasAuthority())
	{
		currentWeather = _weather;
		//TODO RadioActor : set la popup avec la nouvelle meteo si elle �taot afficher
	}
	else
		ServerRpc_SetClientWeather(_weather);
}

void ARadioActor::Interacted(AMainCharacter* _character)
{
	if (!isOn)
		Super::Interacted(_character);
}

void ARadioActor::Interacted_Internal()
{
	if (!radioWidgetRef) return;

	interactionWidgetComponent->SetWidgetClass(radioWidgetRef);
	isOn = true;
	onLightOnRadio.Broadcast(currentWeather, player->GetDayManager()->GetCurrentDay());

	URadioUI* Radio = GetOrInitRadioWidget();
	CHECK_NULL(Radio, "Radio is null");
	if (Radio)
	{
		if (!Radio->IsInitialized())
			Radio->CallBackOnShow(currentWeather, player->GetDayManager()->GetCurrentDay(), this);
	}
	Super::Interacted_Internal();
}

void ARadioActor::LaunchAnimation(AMainCharacter* _character)
{
}

void ARadioActor::SetClosest(const bool _state)
{
	Super::SetClosest(_state);
	if (isOn)
		interactionWidgetComponent->SetVisibility(_state);
}

void ARadioActor::OnRep_UpdateCurrentWeather()
{
	//TODO RadioActor : set la popup avec la nouvelle meteo si elle �taot afficher
}

URadioUI* ARadioActor::GetOrInitRadioWidget()
{
	if (RadioWidget) return RadioWidget;
	if (UWidgetComponent* _component = GetComponentByClass<UWidgetComponent>())
	{
		if (!_component->GetWidget())
			_component->SetWidgetClass(radioWidgetRef);
		RadioWidget = Cast<URadioUI>(_component->GetWidget());
		CHECK_NULL_WITH_RETURN(RadioWidget, "Component widget has error to cast in radioWidget", return nullptr);
	}
	return RadioWidget;
}

void ARadioActor::PlaySubtitles(UDataTable* _data)
{
	if (AMainHUD* _hud = Cast<AMainHUD>(GETFPC->GetHUD()))
		if (ULobbyATH* _ath = _hud->GetCurrentUI<ULobbyATH>())
			_ath->GetSubtitles()->OnPlaySubtitle().Broadcast(_data);
}

void ARadioActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARadioActor, currentWeather);
}

void ARadioActor::BeginPlay()
{
	Super::BeginPlay();
	if (UCustomGameInstance* _gi = GetWorld()->GetGameInstance<UCustomGameInstance>())
	{
		_gi->OnWeatherChange().AddUniqueDynamic(this, &ARadioActor::SetCurrentWeather);
		if (_gi->GetCurrentWeather())
			SetCurrentWeather(_gi->GetCurrentWeather());
	}

	if (GetOrInitRadioWidget())
		GetOrInitRadioWidget()->Hide();

	//onBlendEnd.AddDynamic(this, &ARadioActor::ShowMenu);
}

//TODO edit Romain
/*
void ARadioActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
	if (!currentWeather)
	{
		LOG_COLOR_TIME(RADIO_USE_LOG, "PAS DE WEATHER DANS LA RADIO \n ", BLUE, 10);
		return;
	}
	LOG_COLOR_TIME(RADIO_USE_LOG, "RADIO LOIC !!!!! \n  Today we have a " + currentWeather->GetDefaultObject<AWeather>()->GetWeatherName() + ".", BLUE, 10);

}

void ARadioActor::ShowMenu()
{
	/AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	_hud->SetState(EStateUI::HUB_Meteo);

	/*UMeteoUI* _ui = Cast<UMeteoUI>(_hud->GetCurrentUI());
	if (!_ui) {
		LOG(RADIO_USE_LOG, "Not meteo ui");
		return;
	}
	_ui->UpdateWeather(currentWeather);
}
*/
