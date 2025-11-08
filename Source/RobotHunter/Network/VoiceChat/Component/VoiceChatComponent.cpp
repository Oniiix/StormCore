// Fill out your copyright notice in the Description page of Project Settings.
#include "VoiceChatComponent.h"
#include <string>
#include "../../../Utils/DebugUtils.h"
#include "../../../Game/CustomGameState.h"
#include "Sound/SoundClass.h"

// Sets default values for this component's properties
UVoiceChatComponent::UVoiceChatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
//test

// Called when the game starts
void UVoiceChatComponent::BeginPlay()
{
	Super::BeginPlay();
	//gameState = Cast<ACustomGameStateBase>(GetWorld()->GetGameState());
	//mainCharacter = Cast<AMainCharacter>(gameState->GetPlayerOwner());
	if (VoiceSoundClass)
		LOG_COLOR(VOICE_CHAT_COMPONENT_USE_LOG, "[VoiceChat] VoiceSoundClass is not null", GREEN);
}

#if WITH_EDITOR
void UVoiceChatComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName _propertyName = PropertyChangedEvent.GetPropertyName();
	if (_propertyName == "maxDistance" || _propertyName == "maxSoundNearDistance")
		maxSoundNearDistance = maxSoundNearDistance > maxDistance ? maxDistance : maxSoundNearDistance;
}
#endif

void UVoiceChatComponent::InitVoiceChat()
{
	LOG_COLOR(true, "InitVoiceChat", YELLOW);

	gameState = Cast<ACustomGameState>(GetWorld()->GetGameState());
	mainCharacter = Cast<AMainCharacter>(GetOwner());
	CHECK_NULL(mainCharacter, "MainCharacter is null!!");
	CHECK_NULL(mainCharacter->GetPlayerState(), "PlayerState is null!!");

	LOG(VOICE_CHAT_COMPONENT_USE_LOG, "[VoiceChat] Voip INIT !!!");
	LOG(VOICE_CHAT_COMPONENT_USE_LOG, "[VoiceChat] mainCharacter = " + mainCharacter->GetName());

	voip = UVOIPTalker::CreateTalkerForPlayer(mainCharacter->GetPlayerState());
	voip->RegisterWithPlayerState(mainCharacter->GetPlayerState());
	voip->Settings.ComponentToAttachTo = mainCharacter->GetCameraComponent()->GetCamera();
	SetMicThreshold(micThreshold);
	Mute(isMuted);
	isVoipInit = true;
}

// Called every frame
void UVoiceChatComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//VolumeDistance();
	//Debug();
}

void UVoiceChatComponent::Debug()
{
	if (!isDebug)
		return;
	DRAW_SPHERE(GetOwner()->GetActorLocation(), maxDistance, FColor::Blue);
}

void UVoiceChatComponent::VolumeDistance()
{
	 if (!gameState || !isVoipInit || gameState->GetPlayers().Num() < 2)
	 	return;

	const FVector _characterLocation = gameState->GetPlayers()[0]->GetActorLocation(); //TODO VoiceChatComponent : optimiser la façon dont on calcul la distance entre les deux joueurs si possible
	const FVector _characterLocation2 = gameState->GetPlayers()[1]->GetActorLocation();
	const float _distance = FVector::Distance(_characterLocation, _characterLocation2);

	//const float _volumeMultiplier = FMath::Clamp((maxVoiceVolume - (_distance < maxSoundNearDistance ? 0 : _distance / (maxDistance / maxVoiceVolume ))), 0.0f, maxVoiceVolume);  // Decreases volume with distance
	const float _value = maxDistance / maxSoundNearDistance;
	const float _facteurAttenuation = FMath::Loge((_distance + 1) / maxSoundNearDistance) / FMath::Loge(_value);
	const float _volumeMultiplier = FMath::Clamp(maxVoiceVolume * (1.0f - _facteurAttenuation), 0.0f, maxVoiceVolume);

	if (VoiceSoundClass)
	{
		VoiceSoundClass->Properties.Volume = _volumeMultiplier;  // Adjust the volume
		LOG_COLOR(VOICE_CHAT_COMPONENT_USE_LOG, "[VoiceChat] Volume = " + FString::SanitizeFloat(VoiceSoundClass->Properties.Volume) + " Character => " + mainCharacter->GetName(), BLUE);
	}
}

void UVoiceChatComponent::DestroyVoip()
{
	if (!voip)
		return;
	GetWorld()->GetFirstPlayerController()->StopTalking();
	//voip->UnregisterComponent();
	//voip->DestroyComponent();
	//voip->
	isVoipInit = false;
}


void UVoiceChatComponent::MuteOtherPlayer(const FUniqueNetIdRepl& PlayerId)
{
	GetWorld()->GetFirstPlayerController()->ClientMutePlayer(PlayerId);
}

void UVoiceChatComponent::Mute(const bool _isMute)
{
	isMuted = _isMute;
	if (_isMute || isPushToTalk)
	{
		GetWorld()->GetFirstPlayerController()->StopTalking();
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->StartTalking();
	}
}

void UVoiceChatComponent::PushToTalk(const bool _isPushToTalk)
{
	if (isMuted)
		return;
	GetWorld()->GetFirstPlayerController()->ToggleSpeaking(_isPushToTalk);
	
}

void UVoiceChatComponent::InitPushToTalk(const bool _pushToTalk)
{
	isPushToTalk = _pushToTalk;
	if (_pushToTalk)
		GetWorld()->GetFirstPlayerController()->StopTalking();
	else
	{
		GetWorld()->GetFirstPlayerController()->StartTalking();
	}

}

void UVoiceChatComponent::SetMicThreshold(const float _value)
{
	UVOIPStatics::SetMicThreshold(_value);
	micThreshold = _value;
	
}

void UVoiceChatComponent::RefreshVoip()
{
	DestroyVoip();
	InitVoiceChat();
}

//void UVoiceChatComponent::InitSoundVoice()
//{
//	if (VoiceSoundClass.IsNull())
//	{
//		LOG_COLOR(VOICE_CHAT_COMPONENT_USE_LOG, "VoiceSoundClass is null", RED);
//		return;
//	}
//	maxVoiceVolume = VoiceSoundClass->Properties.Volume;
//	LOG_COLOR(VOICE_CHAT_COMPONENT_USE_LOG, "maxVoiceVolume =" + FString::SanitizeFloat(maxVoiceVolume), BLUE);
//}


