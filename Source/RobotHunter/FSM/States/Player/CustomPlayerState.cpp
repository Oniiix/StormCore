#include "CustomPlayerState.h"
#include "../../Components/PlayerFSMComponent.h"
#include "../../../CCC/Character/MainCharacter.h"

UCustomPlayerState::UCustomPlayerState()
{
	cameraSettingsKey = FString();
	stateInputs = nullptr;
	player = nullptr;
}

void UCustomPlayerState::RetrievePlayer()
{
	if (player)
		return;

	if (fsm)
	{
		UPlayerFSMComponent* _playerFSMComp = dynamic_cast<UPlayerFSMComponent*>(fsm->GetOwner());

		if (_playerFSMComp)
			player = _playerFSMComp->GetPlayer();
	}
}

void UCustomPlayerState::DisablePlayerContext()
{
	if (player)
	{
		TSoftObjectPtr<UInputMappingContext> _currentContext = player->GetCurrentPlayerContext();
		player->DisablePlayerContext(_currentContext);
	}
}

void UCustomPlayerState::EnablePlayerContext()
{
	if (player && stateInputs)
		player->EnablePlayerContext(stateInputs);
}

void UCustomPlayerState::UpdateCameraSettings()
{
	if (player)
	{
		UCustomCameraComponent* _cameraComp = player->GetCameraComponent();

		if (_cameraComp)
		{
			const FString _currentSettings = _cameraComp->GetCurrentSettingsKey();
			FString _newSettings = cameraSettingsKey;

			if (specificCameraSettingsKeys.Contains(_currentSettings))
				_newSettings = specificCameraSettingsKeys[_currentSettings];

			_cameraComp->UpdateCurrentSettings(_newSettings);
		}
	}
}

void UCustomPlayerState::UpdateInputMappingContext()	
{
	DisablePlayerContext();
	EnablePlayerContext();
}

void UCustomPlayerState::Enter()
{
	LOG_COLOR_TIME(false, GetName(), FColor::Red, 2);
	RetrievePlayer();

	if (player)
	{
		//[CustomPLayerState] : si l'aim casse c'est ici
		//player->SetIsAiming(false);
		//player->OnScope().Broadcast(false);
		UpdateStateSpeed();
	}

	UpdateCameraSettings();
	UpdateInputMappingContext();
	Super::Enter();
}

void UCustomPlayerState::UpdateStateSpeed()
{
	if (stateSpeed == ESpeedMode::NOSPEED)return;
	if (player->HasAuthority())
		player->SetSpeedMode(stateSpeed);
	else
		player->ServerRpc_SetClientSpeedMode(stateSpeed);
}
