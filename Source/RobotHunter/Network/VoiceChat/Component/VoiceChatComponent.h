// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//test
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../CCC/Character/MainCharacter.h"
#include <Net/VoiceConfig.h>
#include "../../../Game/CustomGameState.h"
#include "../UI/VoiceChatParameter.h"
#include "VoiceChatComponent.generated.h"

#define VOICE_CHAT_COMPONENT_USE_LOG false


UCLASS()
class ROBOTHUNTER_API UVoiceChatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Parameter/Property
	UPROPERTY(EditAnywhere, Category = "VoiceChat/Settings")
	TObjectPtr<USoundClass> VoiceSoundClass = nullptr;
	UPROPERTY(EditAnywhere,Category= "VoiceChat/Settings", meta = (UMin = 1, ClampMin = 1))
	int maxDistance = 2000;
	UPROPERTY(EditAnywhere, Category = "VoiceChat/Settings", meta = (UMin = 1, ClampMin = 1))
	int maxSoundNearDistance = 100;

	UPROPERTY(EditAnywhere,Category= "VoiceChat/Settings")
	float micThreshold = 0.5f;
	UPROPERTY(EditAnywhere,Category= "VoiceChat/Settings")
	bool isMuted = false;
	UPROPERTY(EditAnywhere,Category= "VoiceChat/Settings")
	float maxVoiceVolume = 1.0f;

	UPROPERTY(EditAnywhere,Category= "VoiceChat/Debug")
	bool isDebug = false;
#pragma endregion

#pragma region Property
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;
	UPROPERTY()
	TObjectPtr<UVOIPTalker> voip = nullptr;
	UPROPERTY()
	ACustomGameState* gameState = nullptr;
	
	bool isVoipInit = false;
	bool isPushToTalk = false;
	FTimerHandle TimerHandle;
#pragma endregion

#pragma region Getter/setter
public :
	FORCEINLINE float GetVoiceLevel() const { return voip->GetVoiceLevel(); }
	FORCEINLINE bool GetIsVoipInit() const { return isVoipInit; }
	FORCEINLINE float GetMaxVoiceVolume() const { return maxVoiceVolume; }
	FORCEINLINE float GetMicThreshold() const { return micThreshold; }
	FORCEINLINE int GetMaxDistance() const { return maxDistance; }
	FORCEINLINE int GetMaxSoundNearDistance() const { return maxSoundNearDistance; }
	FORCEINLINE bool GetIsMuted() const { return isMuted; }
	FORCEINLINE bool GetIsPushToTalk() const { return isPushToTalk; }
	FORCEINLINE void SetMaxVoiceVolume(float _maxVoiceVolume) { maxVoiceVolume = _maxVoiceVolume; }
	FORCEINLINE void SetMaxDistance(float _maxDistance) { maxDistance = _maxDistance; }
	FORCEINLINE void SetSoundNearDistance(float _maxSoundNearDistance) { maxSoundNearDistance = _maxSoundNearDistance; }
#pragma endregion

public:
	// Sets default values for this component's properties
	UVoiceChatComponent();

#pragma region Methods
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/// <summary>
	/// Initializes the voice chat system. It links the voice chat to the main character's player state
	/// and sets up necessary components such as VOIP (Voice Over IP).
	/// </summary>
	void InitVoiceChat();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/** Draws debug information, such as the voice chat radius, if debugging is enabled. */
	void Debug();
	/** Updates the voice volume based on the distance between players. */
	void VolumeDistance();
	/// <summary>
	/// Destroys the voice chat (VOIP) component and cleans up related resources.
	/// Stops any current talking sessions.
	/// </summary>
	void DestroyVoip();
	/// <summary>
	/// Mutes a specific player by their unique player ID.
	/// </summary>
	/// <param name="PlayerId"> The unique ID of the player to mute. </param>
	void MuteOtherPlayer(const FUniqueNetIdRepl& PlayerId);
	/// <summary>
	/// Mutes or unmutes the local player.
	/// </summary>
	/// <param name="_isMute">Set to true to mute, false to unmute.</param>
	void Mute(bool _isMute);
	/// <summary>
	/// Enables or disables Push-To-Talk functionality for the local player.
	/// </summary>
	/// <param name="_isPushToTalk">Set to true to enable Push-To-Talk, false to disable it.</param>
	void PushToTalk(bool _isPushToTalk);
	/// <summary>
	/// Configures Push-To-Talk mode at initialization. When enabled, the player
	/// can only talk while holding the push-to-talk button.
	/// </summary>
	/// <param name="_pushToTalk">Set to true to enable Push-To-Talk, false to disable it.</param>
	void InitPushToTalk(bool _pushToTalk);
	/// <summary>
	/// Configures the microphone sensitivity threshold.
	/// Only audio input exceeding this threshold will activate the microphone.
	/// </summary>
	/// <param name="_value">The new threshold value.</param>
	void SetMicThreshold(float _value);
	/// <summary>
	/// Refreshes the VOIP system by destroying and reinitializing it.
	/// Used to apply updated settings or recover from errors.
	/// </summary>
	void RefreshVoip();
	/*void InitSoundVoice();
	void ClearTimer();*/
#pragma endregion

		
};
