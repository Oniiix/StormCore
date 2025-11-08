// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RobotHunter/Network/Component/NetworkAIComponent.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "RobotHunter/Interface/LoadingInterface/LoadingInterface.h"
#include "RobotHunter/Utils/ENUM_UI.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "CustomPlayerController.generated.h"

#define CUSTOM_PLAYER_CONTROLLER_USE_LOG false

class AAI_Base;
class AArena;
class ULifeComponent;
class UPerceptionSystemComponent;
class USightSystemComponent;
class UListenSystemComponent;
class ASphereSound;
class AHUD;

UCLASS()
class ROBOTHUNTER_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnRegistered, AMainCharacter*, _chara);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadFinish, EStateUI, _state);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnLoadFinish onLoadFinish;


	FOnPawnRegistered onPawnRegistered;

	UPROPERTY(VisibleAnywhere, Category = "Custom|Network")
	TObjectPtr<UNetworkAIComponent> networkAI = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Custom|Network")
	TObjectPtr<UNetworkGPEComponent> networkGPE = nullptr;


	UPROPERTY(VisibleAnywhere, Category = "Custom|HUD")
	TObjectPtr<AMainHUD> hud = nullptr;

	//TEST LOADING

	int loadingActorCount = 0;

	int currentActorLoad = 0;

	UPROPERTY(EditAnywhere, Category = "Custom|Members") TArray <TSubclassOf<AMembersActor>> allMembers;

	UPROPERTY(EditAnywhere, Category = "Custom|Members") TSubclassOf<ARightArmActor> rightArm;
	UPROPERTY(EditAnywhere, Category = "Custom|Members") TSubclassOf<ALeftArmActor> leftArm;
	UPROPERTY(EditAnywhere, Category = "Custom|Members") TSubclassOf<ACalfActor> calf;
	UPROPERTY(EditAnywhere, Category = "Custom|Members") TSubclassOf<AThighActor> thigh;

	TArray<TSubclassOf<AMembersActor>> currentMembers;

	UPROPERTY() 
	AMainCharacter* mainCharacter = nullptr;


public:
	FORCEINLINE FOnPawnRegistered& OnPawnRegistered() { return onPawnRegistered; }
	FORCEINLINE FOnLoadFinish& OnLoadFinish() { return onLoadFinish; }

	ACustomPlayerController();

#pragma region Members

public:
	FORCEINLINE TArray<TSubclassOf<AMembersActor>> GetAllMembers() const { return allMembers; }
	FORCEINLINE TSubclassOf<ARightArmActor> GetRightArm() const { return rightArm; }
	FORCEINLINE TSubclassOf<ALeftArmActor> GetLeftArm() const { return leftArm; }
	FORCEINLINE TSubclassOf<ACalfActor> GetCalf() const { return calf; }
	FORCEINLINE TSubclassOf<AThighActor> GetThigh() const { return thigh; }
	FORCEINLINE TArray<TSubclassOf<AMembersActor>> GetCurrentMembers() const { return currentMembers; }


	void SetRightArm(TSubclassOf<AMembersActor> _rightArm);
	UFUNCTION(Server, Reliable) void ServerRpc_SetRightArm(TSubclassOf<AMembersActor> _rightArm);
	void SetLeftArm(TSubclassOf<AMembersActor> _leftArm);
	UFUNCTION(Server, Reliable) void ServerRpc_SetLeftArm(TSubclassOf<AMembersActor> _leftArm);
	void SetCalf(TSubclassOf<AMembersActor> _calf);
	UFUNCTION(Server, Reliable) void ServerRpc_SetCalf(TSubclassOf<AMembersActor> _calf);
	void SetThigh(TSubclassOf<AMembersActor> _thigh);
	UFUNCTION(Server, Reliable) void ServerRpc_SetThigh(TSubclassOf<AMembersActor> _thigh);
#pragma endregion


#pragma region Network
	FORCEINLINE TObjectPtr<UNetworkAIComponent> NetworkAI() { return networkAI; }
	FORCEINLINE TObjectPtr<UNetworkGPEComponent> NetworkGPE() { return networkGPE; }

	UFUNCTION(Client, Reliable) void KickCustom();
#pragma endregion

#pragma region SeamlessTravel
	virtual void BeginPlayingState() override;

	virtual void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

	UFUNCTION(BlueprintCallable, Category = "HUD", Reliable, Client)
	void CustomClientSetHUD(TSubclassOf<AHUD> NewHUDClass);
#pragma endregion

	UFUNCTION(blueprintCallable)
	void UpdateLeaderboard(const FName& _statName, int32 _score);



protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION() void ActorFinishLoading();

};
