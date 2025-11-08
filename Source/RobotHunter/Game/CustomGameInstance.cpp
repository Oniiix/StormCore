// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"

#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "RobotHunter/QuestSystem/QuestSubsystem.h"

void UCustomGameInstance::Init()
{
	Super::Init();
	GetSubsystem<UInputSubsystem>()->SetInputImage(keyImageDataAsset);
}

void UCustomGameInstance::CallbackOnLevelChange(const ETypeChange _type) const
{
	UQuestSubsystem* _subsystem = GetSubsystem<UQuestSubsystem>();
	_subsystem->ClearAllQuests();

	switch (_type)
	{
	case ETypeChange::MainToHub:
		//LOG_DEBUG("Main To Hub");
		onLevelChangeMainToHub.Broadcast();
		break;
	case ETypeChange::HubToGame:
		//LOG_DEBUG("Hub to game");
		onLevelChangeHubToGame.Broadcast();
		break;
	case ETypeChange::HubToMain:
		//LOG_DEBUG("Hub to main");
		onLevelChangeHubToMain.Broadcast();
		break;

	case ETypeChange::GameToHub:
		//LOG_DEBUG("Game To Hub");
		onLevelChangeGameToHub.Broadcast();
		break;
	case ETypeChange::GameToMain:
		//LOG_DEBUG("Game To Main");
		onLevelChangeGameToMain.Broadcast();
		break;
	case ETypeChange::Quit:
		//LOG_DEBUG("Quit Game");
		onLevelChangeMainToQuit.Broadcast();
		break;
	case ETypeChange::DeathToHub:
		//LOG_DEBUG("Quit Game");
		onDeathToHub.Broadcast();
		break;
	case ETypeChange::IntroToHub:
		onLevelChangeIntroToHub.Broadcast();
		break;
	case ETypeChange::MainToIntro:
		onLevelChangeMainToIntro.Broadcast();
		break;
	case ETypeChange::IntroToMain:
		onLevelChangeIntroToMain.Broadcast();
		break;
	}
}

void UCustomGameInstance::CallbackOnLanguageChange(const FString& _shortcut) const
{
	onLanguageChange.Broadcast(_shortcut);
}
