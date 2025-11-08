// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppParameterMayBeConstPtrOrRef
// ReSharper disable CppParameterMayBeConst
#include "VoiceChatParameter.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/VoiceChat/Component/VoiceChatComponent.h"
#include "RobotHunter/CCC/CustomPlayerController.h"

void UVoiceChatParameter::NativeConstruct()
{
	Super::NativeConstruct();
	BIND_ACTION_PARAM(VolumeSlider, OnValueChanged, UVoiceChatParameter, ValueVolumeChanged);
	BIND_ACTION_PARAM(micThreshold, OnValueChanged, UVoiceChatParameter, MicThresholdChanged);
	BIND_ACTION_PARAM(muteCheckBox, OnCheckStateChanged, UVoiceChatParameter, MuteCheckBoxChanged);
	BIND_ACTION_PARAM(pushToTalkCheckBox, OnCheckStateChanged, UVoiceChatParameter, PushToTalkCheckBoxChanged);
	BIND_ACTION_PARAM(closeButton, OnClicked, UVoiceChatParameter, CloseButtonClicked);
	Init();
}

void UVoiceChatParameter::Init()
{
	gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());
	player = gameState->GetPlayerOwner();
	if (!player)
		return;
	//============Important=====
	//VolumeSlider->SetValue(player->GetVoiceChatComponent()->GetMaxVoiceVolume());
	//micThreshold->SetValue(player->GetVoiceChatComponent()->GetMicThreshold());
	//muteCheckBox->SetIsChecked(player->GetVoiceChatComponent()->GetIsMuted());
	//pushToTalkCheckBox->SetIsChecked(player->GetVoiceChatComponent()->GetIsPushToTalk());
}

void UVoiceChatParameter::ValueVolumeChanged(float _value)
{
	if (!player)
		return;
	if (gameState->GetPlayers().Num() < 2) //TODO A Opti avec le postLogin
		return;
	if (player->HasAuthority())
	{
		player = gameState->GetPlayers()[1];
	}
	else
	{
		player = gameState->GetPlayers()[0];
	}
	//============Important=====
	//player->GetVoiceChatComponent()->SetMaxVoiceVolume(_value);
	Debug(player, _value);
}

void UVoiceChatParameter::Debug(AMainCharacter* _player, float _value) const
{
	LOG_COLOR(VOICECHAT_PARAMETER_USE_LOG, "[VoiceChatParameter] Player Name = " + _player->GetName(), BLUE);
	//LOG_COLOR(VOICECHAT_PARAMETER_USE_LOG,
	//          "[VoiceChatParameter] Player Name Voice Chat = " + _player->GetVoiceChatComponent()->GetOwner()->GetName(
	  //        ), GREEN);
	LOG_COLOR(VOICECHAT_PARAMETER_USE_LOG, "[VoiceChatParameter] Value = " + FString::SanitizeFloat(_value), BLUE);
	//LOG_COLOR(VOICECHAT_PARAMETER_USE_LOG,
	  //        "[VoiceChatParameter] Volume = " + FString::SanitizeFloat(_player->GetVoiceChatComponent()->
	//	          GetMaxVoiceVolume()), BLUE);
}

void UVoiceChatParameter::MicThresholdChanged(float _value)
{
	if (!player)
		return;
	//player->GetVoiceChatComponent()->SetMicThreshold(_value);
}

void UVoiceChatParameter::MuteCheckBoxChanged(bool _bool)
{
	if (!player)
		return;
	//player->GetVoiceChatComponent()->Mute(_bool);
}

void UVoiceChatParameter::PushToTalkCheckBoxChanged(bool _bool)
{
	//player->GetVoiceChatComponent()->InitPushToTalk(_bool);
}

void UVoiceChatParameter::CloseButtonClicked()
{
	GetOwningPlayer()->bShowMouseCursor = false;
	SetFocus();
	this->RemoveFromParent();
}

//if (gameState->GetPlayers().Num() < 2) //TODO A Opti avec le postLogin
//	return;
//if (player->HasAuthority())
//{
//	player = gameState->GetPlayers()[1];
//}
//else
//player = gameState->GetPlayers()[0];
