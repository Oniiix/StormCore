// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>
#include "Interfaces/OnlinePresenceInterface.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "OnlineManagerSubsystem.generated.h"



UENUM()
enum EOnlineAccessibilityState
{
	Public = 0,
	OnlyFriends,
	Private
};

#pragma region Struct

USTRUCT()
struct FServerResult
{
	GENERATED_BODY()

	FString sessionName;
	FString serverName;
	FString name;
	int pingMs = 0;

	FServerResult()
	{
		name = "";
		pingMs = 0;
	}

	FServerResult(const FString& sessionName, const FString& _serverName, const FString& name, int pingMs)
		: sessionName(sessionName), serverName(_serverName), name(name), pingMs(pingMs)
	{
	}
};

USTRUCT()
struct FFriendsResult
{
	GENERATED_BODY()

	FString name;
	FOnlineUserPresence presence;
	int index;

	FFriendsResult()
	{
		name = "";
		presence = FOnlineUserPresence();
		index = -1;
	}

	FFriendsResult(const FString& _name, FOnlineUserPresence _presence, int _index)
		: name(_name), presence(_presence), index(_index)
	{
	}
	FFriendsResult(const FString& _name, int _index)
		: name(_name), index(_index)
	{
	}
};

USTRUCT()
struct FOnlineAccessibilitySettings
{
	GENERATED_BODY()


	bool usesPresence = true;
	bool allowJoinViaPresence = true;
	bool allowJoinViaPresenceFriendsOnly = true;
	bool shouldAdvertise = true;

	FOnlineAccessibilitySettings()
	{
	}

	FOnlineAccessibilitySettings(bool usesPresence, bool allowJoinViaPresence, bool allowJoinViaPresenceFriendsOnly,
	                             bool shouldAdvertise)
		: usesPresence(usesPresence), allowJoinViaPresence(allowJoinViaPresence),
		  allowJoinViaPresenceFriendsOnly(allowJoinViaPresenceFriendsOnly), shouldAdvertise(shouldAdvertise)
	{
	}
};

#pragma endregion

#define IS_LAN() IOnlineSubsystem::Get()->GetSubsystemName() == "NULL"
#define ONLINE_MANAGER_USE_LOG false
#define BAN_LIST FName("BAN_LIST")



UCLASS()
class ROBOTHUNTER_API UOnlineManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerFound, const TArray<FServerResult>&, _serverList);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendListUpdate, const TArray<FFriendsResult>&, _friendList);

	UPROPERTY(Blueprintcallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnServerFound onServerFound;

	UPROPERTY(Blueprintcallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnFriendListUpdate onFriendListUpdate;

	FOnReadFriendsListComplete onReadFriendListComplete;
	FOnInviteReceivedDelegate onInviteReceivedDelegate;

	UPROPERTY()
	TMap<TEnumAsByte<EOnlineAccessibilityState>, FOnlineAccessibilitySettings> accessibilitySettings;

	EOnlineAccessibilityState currentAccessibiltyState = EOnlineAccessibilityState::Public;

	TMap<FString, FOnlineSessionSearchResult> searchResult;

	TArray<TSharedRef<FOnlineFriend>> resultFriends;

	TArray<FFriendsResult> friendsList;

	FUniqueNetIdRepl steamID;



	TObjectPtr<UCustomGameInstance> gameInstance = nullptr;

	FTimerHandle checkSessionsTimer;

	short int findFriendIndex = 0;

	static short int const playerMaxCount = 2;


	FString travelPath;

public:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FName sessionName = "RobotHunter_Session";
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FName rhKey = "75ZAKLF527EYH88WDHDR";
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FString banList;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FName serverName = "RobotHunter_Server";
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	FString ipAddress = "0.0.0.0";


#pragma region net ptr
	IOnlineSubsystem* online = nullptr;
	IOnlineSessionPtr session = nullptr;
	IOnlineFriendsPtr friends = nullptr;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
#pragma endregion net ptr

#pragma region Setter/Getter
public:
	FORCEINLINE FOnServerFound& OnServerFound() { return onServerFound; }
	FORCEINLINE FOnFriendListUpdate& OnFriendListUpdate() { return onFriendListUpdate; }
	FORCEINLINE FUniqueNetIdRepl GetSteamID() const { return steamID; }
	FORCEINLINE static short int GetPlayerMaxCount() { return playerMaxCount; }
	FORCEINLINE FString GetBanList() const { return banList; }
	FORCEINLINE void SetBanList(FString _banList) { banList = _banList; }
	FORCEINLINE void AddBanList(FString _banList) { banList += _banList; }

	UFUNCTION(BlueprintCallable) bool IsServer(AActor* _actor) const { return _actor->HasAuthority(); }

#pragma endregion


	UOnlineManagerSubsystem();
	void InitOnline();
	void Initialize(FSubsystemCollectionBase& _collection) override;
	void Deinitialize() override;
public:

	UFUNCTION(BlueprintCallable) void CreateServer(const FString& _serverName, const EOnlineAccessibilityState _accessibility, const FString& _travelPath = "");
	UFUNCTION(BlueprintCallable) void FindServer();
	UFUNCTION(BlueprintCallable) void FindFriendServer();
	UFUNCTION(BlueprintCallable) void CancelFindServer();
	 void JoinServer(const FString& _sessionName, const FOnlineSessionSearchResult& _searchResult);
	UFUNCTION(BlueprintCallable) void QuitServer();
	UFUNCTION(BlueprintCallable) void QuitServerCrash();

	UFUNCTION(BlueprintCallable) void KickPlayer(APlayerController* _playerController, const FText& _kickReason);
	UFUNCTION(BlueprintCallable) void BanPlayer(APlayerController* _playerController, const FText& _banreason);

	void StartServer();
	void UpdateServer(const EOnlineAccessibilityState& _newAccebilityState);
	void UpdateServer(const EOnlineAccessibilityState& _newAccebilityState, const FName& _newSessionName);
	void UpdateServer(FName _key, const FString& _value);
	void StopServer();

	void LoadMap(const FString& _path);
	bool IsInSession() const;
#pragma region Social
	void InitFriendList();
	bool SendInviteFriends(int _friendsIndex);
	FORCEINLINE FOnlineSessionSearchResult GetSessionSearchResult(const FString& _sessionName) const { return searchResult[_sessionName]; }
#pragma endregion

	UFUNCTION() void OnReadFriendListComplete(int32 _userNum, bool _bWasSuccessfull, const FString& _listName, const FString& _errorStr);
private:
	//EVENTS
	/// <summary>
	/// Delegate fired when a session create request has completed
	/// </summary>
	void OnCreateSessionComplete(FName _sessionName, bool _success);

	/// <summary>
	/// Delegate fired when the search for an online session has completed
	/// </summary>
	void OnFindSessionComplete(bool _success);

	void OnFindFriendSessionComplete(int32 _localID, bool _success, const TArray<FOnlineSessionSearchResult>& _results);

	void OnCancelFindSessionComplete(bool _success);

	void OnSessionInviteReceived(const FUniqueNetId& _userId, const FUniqueNetId& _fromId, const FString& _appId, const FOnlineSessionSearchResult& _inviteResult);
	void OnInviteReceived(const FUniqueNetId& _userId, const FUniqueNetId& _friendID);

	void OnSessionUserInviteAccepted(const bool _bWasSuccessful, const int32 _controllerId, FUniqueNetIdPtr _userId, const FOnlineSessionSearchResult& _inviteResult);

	void OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result);

	void OnQuitSessionComplete(FName _sessionName, bool _success);

	/// <summary>
	/// Event triggered after a network failure of any kind has occurred
	/// </summary>
	void OnNetworkFailure(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,const FString& ErrorString);

	void OnSessionFailure(const FUniqueNetId& _userId, ESessionFailure::Type _reason);
	bool VerifBanned(FString _banList);
};
