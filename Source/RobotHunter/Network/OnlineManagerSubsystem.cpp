// Fill out your copyright notice in the Description page of Project Settings.
#include "OnlineManagerSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Kismet/GameplayStatics.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include <Interfaces/OnlineIdentityInterface.h>
#include "RobotHunter/Game/CustomGameMode.h"
#include <Online/OnlineSessionNames.h>
#include "VoiceChat/Component/VoiceChatComponent.h"
// pas utilisé pour l'instant :
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/Utils/CustomBlueprintLibrary.h"


#define SESSION_NAME FName("SESSION_NAME")
#define SERVER_NAME FName("SERVER_NAME")
#define OAS FName("OAS")
#define RH_KEY FName("RH_KEY")


UOnlineManagerSubsystem::UOnlineManagerSubsystem()
{
	serverName = "RobotHunter_Server";
	sessionName = "RobotHunter_Session";
	resultFriends = TArray<TSharedRef<FOnlineFriend>>();
}

//=========== OVERRIDE METHOD ==============
void UOnlineManagerSubsystem::Initialize(FSubsystemCollectionBase& _collection)
{
	Super::Initialize(_collection);

	if (!(gameInstance = GetWorld()->GetGameInstance<UCustomGameInstance>()))
		LOG_ERROR(ONLINE_MANAGER_USE_LOG, " gameInstance is null!!");

	accessibilitySettings.Add(EOnlineAccessibilityState::Public, FOnlineAccessibilitySettings(true, true, false, true));
	accessibilitySettings.Add(EOnlineAccessibilityState::OnlyFriends,
	                          FOnlineAccessibilitySettings(true, true, false, true));
	accessibilitySettings.Add(EOnlineAccessibilityState::Private,
	                          FOnlineAccessibilitySettings(false, false, false, false));
	InitOnline();
}

void UOnlineManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Exit Online", RED);
}
//=========== OVERRIDE METHOD ==============



//=========== MAIN ONLINE METHOD ==============  
#pragma region MainOnlineMethod
void UOnlineManagerSubsystem::InitOnline()
{
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Enter Online", GREEN);

	//UNcomment pour le Online

	/*online = IOnlineSubsystem::Get();
	if (online)
	{
		if (!(IS_LAN()))
		{
			friends = online->GetFriendsInterface();
			InitFriendList();
		}

		LOG(ONLINE_MANAGER_USE_LOG, "Online : " + online->GetSubsystemName().ToString());
		session = online->GetSessionInterface();
		if (session.IsValid())
		{
			session->OnCreateSessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnCreateSessionComplete);
			session->OnFindSessionsCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnFindSessionComplete);
			session->OnFindFriendSessionCompleteDelegates->AddUObject(this, &UOnlineManagerSubsystem::OnFindFriendSessionComplete);
			session->OnCancelFindSessionsCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnCancelFindSessionComplete);
			session->OnJoinSessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnJoinSessionComplete);
			session->OnDestroySessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnQuitSessionComplete);
			session->OnSessionInviteReceivedDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnSessionInviteReceived);
			session->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnSessionUserInviteAccepted);
			session->OnSessionFailureDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnSessionFailure);
			GEngine->OnNetworkFailure().AddUObject(this, &UOnlineManagerSubsystem::OnNetworkFailure);
		}

		steamID = online->GetIdentityInterface()->GetUniquePlayerId(0);
	}*/
}

void UOnlineManagerSubsystem::CreateServer(const FString& _serverName, const EOnlineAccessibilityState _accessibility, const FString& _travelPath)
{
	const FString _steamID = steamID->ToString();
	travelPath = _travelPath;
	serverName = FName(_serverName);
	sessionName = FName("RobotHunter_" + _steamID + "_Session");

	currentAccessibiltyState = _accessibility;
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, " Create Session with " + FString::FromInt(currentAccessibiltyState), ORANGE);

	if (session->GetNamedSession(sessionName))
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Server DestroySession : " + sessionName.ToString(), GREEN);
		session->DestroySession(sessionName);
		return;
	}

	FOnlineSessionSettings _sessionSettings;
	_sessionSettings.bAllowJoinInProgress = true;
	_sessionSettings.bIsDedicated = false;
	_sessionSettings.bShouldAdvertise = accessibilitySettings[EOnlineAccessibilityState::Private].shouldAdvertise;
	_sessionSettings.NumPublicConnections = playerMaxCount;
	_sessionSettings.bUsesPresence = accessibilitySettings[EOnlineAccessibilityState::Private].usesPresence;
	_sessionSettings.bAllowJoinViaPresence = accessibilitySettings[EOnlineAccessibilityState::Private].allowJoinViaPresence;
	_sessionSettings.bAllowJoinViaPresenceFriendsOnly = accessibilitySettings[EOnlineAccessibilityState::Private].allowJoinViaPresenceFriendsOnly;
	_sessionSettings.bUseLobbiesIfAvailable = true;
	_sessionSettings.bIsLANMatch = IS_LAN();

	_sessionSettings.Set(SERVER_NAME, _serverName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	_sessionSettings.Set(SESSION_NAME, sessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	_sessionSettings.Set(RH_KEY, rhKey.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	_sessionSettings.Set(OAS, FString::FromInt(currentAccessibiltyState), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	_sessionSettings.Set(BAN_LIST,banList, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Enter Online", GREEN);
	session->CreateSession(0, sessionName, _sessionSettings);
}

void UOnlineManagerSubsystem::FindServer()
{
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Find server", BLUE);
	if (serverName == "")
	{
		onServerFound.Broadcast({});
		return;
	}
	//TODO OnlineManager : Check if this "if" is useful (I don't think so)


	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	sessionSearch->bIsLanQuery = IS_LAN();
	sessionSearch->MaxSearchResults = 100;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	sessionSearch->QuerySettings.Set(SEARCH_NONEMPTY_SERVERS_ONLY, true, EOnlineComparisonOp::Equals);
	sessionSearch->TimeoutInSeconds = 5;
	const bool _result = session->FindSessions(0, sessionSearch.ToSharedRef());
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, _result ? "Find Server => TRUE" : "Find Server => FALSE", _result ? GREEN : RED);
}

void UOnlineManagerSubsystem::FindFriendServer()
{
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Find friends server", BLUE);
	if (resultFriends.Num() == 0 || IS_LAN())
	{
		onServerFound.Broadcast({});
		return;
	}
	if (findFriendIndex == 0)
		searchResult.Empty();

	session->FindFriendSession(0, resultFriends[findFriendIndex]->GetUserId().Get());
}

void UOnlineManagerSubsystem::CancelFindServer()
{
	session->CancelFindSessions();
}

void UOnlineManagerSubsystem::JoinServer(const FString& _sessionName, const FOnlineSessionSearchResult& _searchResult)
{
	if (_sessionName == "" || !_searchResult.IsValid() || !_searchResult.IsSessionInfoValid() || _searchResult.Session.SessionSettings.Settings.IsEmpty())
	{
		LOG_WARNING(ONLINE_MANAGER_USE_LOG, "[JOIN] => Session name is null !");
		return;
	}

	sessionName = FName(_sessionName);
	session->JoinSession(0, sessionName, _searchResult);
}

void UOnlineManagerSubsystem::QuitServer()
{
	if (online->IsServer())
	{
		StopServer();
		if (session->GetNamedSession(sessionName))
		{
			LOG_COLOR(true, "Server DestroySession : " + sessionName.ToString(), GREEN);
			session->DestroySession(sessionName);
		}
	}
	else
		session->DestroySession(sessionName);


	//AMainCharacter* main_character = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//if (main_character)
	//{
	//	main_character->PlayerDestroyVoiceChat();
	//}
	//else
	//{
	//}
}

void UOnlineManagerSubsystem::QuitServerCrash()
{
	//if (online->IsServer())
	//	StopServer();
	//session->DestroySession(sessionName);
}

void UOnlineManagerSubsystem::KickPlayer(APlayerController* _playerController, const FText& _kickReason)
{
	if (ACustomGameMode* _gm = GetWorld()->GetAuthGameMode<ACustomGameMode>())
		_gm->GetGameSession()->KickPlayer(_playerController, _kickReason);
}

void UOnlineManagerSubsystem::BanPlayer(APlayerController* _playerController, const FText& _banReason)
{
	if (ACustomGameMode* _gm = GetWorld()->GetAuthGameMode<ACustomGameMode>())
		_gm->GetGameSession()->BanPlayer(_playerController, _banReason);
}
#pragma endregion
//=========== MAIN ONLINE METHOD ==============


//=========== MAIN ONLINE COMPLETE ==============
#pragma region MainOnlineComplete
void UOnlineManagerSubsystem::OnCreateSessionComplete(FName _sessionName, bool _success)
{
	if (_success)
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Session created " + _sessionName.ToString(), GREEN);

		if (!gameInstance && !(gameInstance = GetWorld()->GetGameInstance<UCustomGameInstance>()))
		{
			LOG_ERROR(ONLINE_MANAGER_USE_LOG, " (OnCreateSessionComplete) : gameInstance is Null !!");
			return;
		}
		FString _path;
		if (GetWorld()->IsPlayInEditor())
			_path = gameInstance->GetHubLevel().GetAssetName() + "?listen";
		else
			_path = gameInstance->GetHubLevel().LoadSynchronous()->GetName() + "?listen";
		session->GetResolvedConnectString(sessionName, ipAddress);
		GetWorld()->ServerTravel(travelPath == "" ? _path : travelPath, true);
		session->RegisterPlayer(sessionName, *steamID, false);
	}
	else LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Session failed " + _sessionName.ToString(), RED);
}

void UOnlineManagerSubsystem::OnFindSessionComplete(bool _success)
{
	searchResult.Empty();
	if (_success)
	{
		TArray<FServerResult> _result;
		const TArray<FOnlineSessionSearchResult> _results = sessionSearch->SearchResults;
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "SESSION FOUND -> " + FString::FromInt(sessionSearch->SearchResults.Num()), sessionSearch->SearchResults.Num() >= 1 ? GREEN : RED);

		for (FOnlineSessionSearchResult _r : _results)
		{
			const FUniqueNetIdPtr& _steamID = _r.Session.OwningUserId;
			FString _oas, _rh_key, _serverName, _sessionName, _banList;
			_r.Session.SessionSettings.Get(OAS, _oas);
			_r.Session.SessionSettings.Get(RH_KEY, _rh_key);
			_r.Session.SessionSettings.Get(BAN_LIST,_banList);

			const bool _isFriend = IS_LAN() ? true : friends->IsFriend(0, *_steamID, EFriendsLists::ToString(EFriendsLists::InGamePlayers));
			if (_rh_key != rhKey)
				continue;
			if (_oas == FString::FromInt(EOnlineAccessibilityState::OnlyFriends) && !_isFriend)
				continue;
			if (VerifBanned(_banList))
				continue;
			_r.Session.SessionSettings.Get(SESSION_NAME, _sessionName);
			_r.Session.SessionSettings.Get(SERVER_NAME, _serverName);
			searchResult.Add(_sessionName, _r);
			_result.Add(FServerResult(_sessionName, _serverName, _r.Session.OwningUserName, _r.PingInMs));
		}
		onServerFound.Broadcast(_result);
	}
}

void UOnlineManagerSubsystem::OnFindFriendSessionComplete(int32 _localID, bool _success, const TArray<FOnlineSessionSearchResult>& _results)
{
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Find Friend Session Complete", BLUE);
	findFriendIndex++;
	for (FOnlineSessionSearchResult _r : _results)
	{
		const FUniqueNetIdPtr _steamID = _r.Session.OwningUserId;

		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "[" + _r.Session.OwningUserName + "] => Find session", YELLOW);

		FString _oas, _rh_key;
		_r.Session.SessionSettings.Get(RH_KEY, _rh_key);
		_r.Session.SessionSettings.Get(OAS, _oas);

		if (_rh_key != rhKey)
			continue;

		FString _sessionName;
		_r.Session.SessionSettings.Get(SESSION_NAME, _sessionName);
		searchResult.Add(_sessionName, _r);
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "[" + _r.Session.OwningUserName + "] => Success", GREEN);
	}

	if (findFriendIndex == friendsList.Num())
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Finish Check friend", GREEN);
		TArray<FServerResult> _result;
		for (TPair<FString, FOnlineSessionSearchResult> _search : searchResult)
		{
			FString _sessionName, _serverName;
			_search.Value.Session.SessionSettings.Get(SESSION_NAME, _sessionName);
			_search.Value.Session.SessionSettings.Get(SERVER_NAME, _serverName);
			_result.Add(FServerResult(_sessionName, _serverName, _search.Value.Session.OwningUserName, _search.Value.PingInMs));
		}
		LOG(ONLINE_MANAGER_USE_LOG, FString::FromInt(_result.Num()));
		onServerFound.Broadcast(_result);
		findFriendIndex = 0;
	}
	else
		FindFriendServer();
}

void UOnlineManagerSubsystem::OnCancelFindSessionComplete(bool _success)
{
	if (_success)
		onServerFound.Broadcast({});
}

void UOnlineManagerSubsystem::OnSessionInviteReceived(const FUniqueNetId& _userId, const FUniqueNetId& _fromId, const FString& _appId, const FOnlineSessionSearchResult& _inviteResult)
{
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "SessionInvite Received Session", GREEN);
	// MARCHE PAS
}

void UOnlineManagerSubsystem::OnInviteReceived(const FUniqueNetId& _userId, const FUniqueNetId& _friendID)
{
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "SessionInvite Received Friends friendID => "+ _friendID.ToString(), GREEN);
	// MARCHE PAS 
}

void UOnlineManagerSubsystem::OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result)
{
	if (_result == EOnJoinSessionCompleteResult::Success)
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "JOIN -> " + _sessionName.ToString(), YELLOW);
		session->GetResolvedConnectString(_sessionName, ipAddress);
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "IP : " + ipAddress, YELLOW);

		if (APlayerController* _pc = GetGameInstance()->GetFirstLocalPlayerController())
		{
			_pc->ClientTravel(ipAddress, ETravelType::TRAVEL_Absolute);
			session->RegisterPlayer(sessionName, *steamID, false);
		}
		else
			LOG(ONLINE_MANAGER_USE_LOG, "Player controller is null");
	}
	else if (_result == EOnJoinSessionCompleteResult::AlreadyInSession)
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "You already are in session {" + _sessionName.ToString() + "}", ORANGE);
	}
	else if (_result == EOnJoinSessionCompleteResult::SessionIsFull)
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Session {" + _sessionName.ToString() + "} is full" , RED);
	}
	else
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Can't join session {" + _sessionName.ToString() + "} because of \"" + LexToString(_result) + "\" error!!", RED);

	//TODO OnlineManager : Remove "else if" ans "else" after debug properly the joinning/quiting session mechanic
}

void UOnlineManagerSubsystem::OnQuitSessionComplete(FName _sessionName, bool _success)
{
	if (_success)
	{
		//CHECK_NULL(gameInstance, "Game Instance is null !!");
		//gameInstance->SetGameStatus(EGameStatus::MAINMENU);
		//if (GetWorld()->IsPlayInEditor())
		//	UGameplayStatics::OpenLevel(GetWorld(), FName(gameInstance->GetMainMenuLevel().GetAssetName()));
		//else
		//	UGameplayStatics::OpenLevel(GetWorld(), FName(gameInstance->GetMainMenuLevel().GetAssetName()));
	}
}
#pragma endregion
//=========== MAIN ONLINE COMPLETE ==============  


//=========== START/STOP GAME ==============
#pragma region Start/Stop Game
void UOnlineManagerSubsystem::StartServer()
{
	if (!session->GetNamedSession(sessionName))
		return;
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Start Session", PINK);
	if (session->StartSession(sessionName))
	{
		FOnlineSessionSettings* _settings = session->GetSessionSettings(sessionName);
		_settings->bShouldAdvertise = accessibilitySettings[EOnlineAccessibilityState::Private].shouldAdvertise;
		_settings->bUsesPresence = accessibilitySettings[EOnlineAccessibilityState::Private].usesPresence;
		_settings->bAllowJoinViaPresence = accessibilitySettings[EOnlineAccessibilityState::Private].allowJoinViaPresence;
		_settings->bAllowJoinViaPresenceFriendsOnly = accessibilitySettings[EOnlineAccessibilityState::Private].allowJoinViaPresenceFriendsOnly;
		session->UpdateSession(sessionName, *_settings);
	}
}

void UOnlineManagerSubsystem::UpdateServer(const EOnlineAccessibilityState& _newAccebilityState)
{
	UpdateServer(_newAccebilityState, sessionName);
}

void UOnlineManagerSubsystem::UpdateServer(const EOnlineAccessibilityState& _newAccebilityState, const FName& _newSessionName)
{
	FOnlineSessionSettings* _settings = session->GetSessionSettings(sessionName);
	_settings->bShouldAdvertise = accessibilitySettings[_newAccebilityState].shouldAdvertise;
	_settings->bUsesPresence = accessibilitySettings[_newAccebilityState].usesPresence;
	_settings->bAllowJoinViaPresence = accessibilitySettings[_newAccebilityState].allowJoinViaPresence;
	_settings->bAllowJoinViaPresenceFriendsOnly = accessibilitySettings[_newAccebilityState].allowJoinViaPresenceFriendsOnly;
	currentAccessibiltyState = _newAccebilityState;
	sessionName = _newSessionName;

	LOG_COLOR(ONLINE_MANAGER_USE_LOG, FString::FromInt(currentAccessibiltyState), ORANGE);
	LOG_COLOR(ONLINE_MANAGER_USE_LOG, FString::FromInt(session->UpdateSession(sessionName, *_settings)), ORANGE);
}
void UOnlineManagerSubsystem::UpdateServer(const FName _key, const FString& _value)
{
	FOnlineSessionSettings* _settings = session->GetSessionSettings(sessionName);
	_settings->Set(_key, _value,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	session->UpdateSession(sessionName, *_settings);
}

void UOnlineManagerSubsystem::StopServer()
{
	if (session->EndSession(sessionName))
	{
		LOG_COLOR(ONLINE_MANAGER_USE_LOG, "Stop Session", BLUE);
		FOnlineSessionSettings* _settings = session->GetSessionSettings(sessionName);
		_settings->bShouldAdvertise = accessibilitySettings[currentAccessibiltyState].shouldAdvertise;
		_settings->bUsesPresence = accessibilitySettings[currentAccessibiltyState].usesPresence;
		_settings->bAllowJoinViaPresence = accessibilitySettings[currentAccessibiltyState].allowJoinViaPresence;
		_settings->bAllowJoinViaPresenceFriendsOnly = accessibilitySettings[currentAccessibiltyState].allowJoinViaPresenceFriendsOnly;
		session->UpdateSession(sessionName, *_settings);
	}
}
#pragma endregion
//=========== START/STOP GAME ==============


//=========== FRIENDS ==============  
#pragma region Friends
void UOnlineManagerSubsystem::InitFriendList()
{
	friendsList.Empty();
	resultFriends.Empty();

	if (!friends)
	{
		LOG_ERROR(ONLINE_MANAGER_USE_LOG,  "Friends is null !!!");
		return;
	}
	friends->ReadFriendsList(0, "MyFriendsList", FOnReadFriendsListComplete::CreateUObject(this, &UOnlineManagerSubsystem::OnReadFriendListComplete));
	friends->OnInviteReceivedDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnInviteReceived);

}

void UOnlineManagerSubsystem::OnReadFriendListComplete(int32 _userNum, bool _bWasSuccessfull, const FString& _listName, const FString& _errorStr)
{
	if (_bWasSuccessfull)
	{
		friends->GetFriendsList(_userNum, _listName, resultFriends);
		IOnlinePresencePtr _prensenceInterface = online->GetPresenceInterface();

		const int _size = resultFriends.Num();
		for (int _index = 0; _index < _size; _index++)
		{
			//const FOnlineFriend& _friend = *(resultFriends[_index].ToSharedPtr());

			TSharedPtr<FOnlineUserPresence> _presencePtr;
			_prensenceInterface->GetCachedPresence(resultFriends[_index]->GetUserId().Get(), _presencePtr);

			friendsList.Add(FFriendsResult(resultFriends[_index]->GetDisplayName(), *_presencePtr.Get(), _index));
		}
		onFriendListUpdate.Broadcast(friendsList);
	}
}

bool UOnlineManagerSubsystem::SendInviteFriends(int _friendsIndex)
{
	LOG_COLOR(true, "OnlineManager : Send Invite to friend", CYAN);
	return session->SendSessionInviteToFriend(0, sessionName, resultFriends[_friendsIndex]->GetUserId().Get());
	//return friends->SendInvite(0,resultFriends[_friendsIndex]->GetUserId().Get(),"MyFriendsList" );
}


void UOnlineManagerSubsystem::OnSessionUserInviteAccepted(const bool _bWasSuccessful, const int32 _controllerId, FUniqueNetIdPtr _userId, const FOnlineSessionSearchResult& _inviteResult)
{
	LOG_COLOR_TIME(true, "(OnSessionUserInviteAccepted) bWasSuccessful =  " + _bWasSuccessful ? "True" : "False", BLUE, 10);
	if (_bWasSuccessful)
	{
		//FPlatformProcess::CreateProc(TEXT("C:/Program Files/Google/Chrome/Application/chrome.exe"), nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
		session->DestroySession(sessionName);

		if (_inviteResult.IsValid())
		{
			session = IOnlineSubsystem::Get()->GetSessionInterface();
			JoinServer("RobotHunter_" + _userId->ToString() + "_Session", _inviteResult);
		}
		else
		{
			LOG_COLOR_TIME(true, "Invite accept returned no search result", RED, 20);
		}
	}
}
#pragma endregion
//=========== FRIENDS ==============  


//=========== OTHER ==============  
#pragma region Other
void UOnlineManagerSubsystem::LoadMap(const FString& _path)
{
	//session->GetResolvedConnectString(sessionName, ipAddress);
	GetWorld()->ServerTravel(_path);
}

bool UOnlineManagerSubsystem::IsInSession() const
{
	if (session->GetNamedSession(sessionName))
		return true;
	return false;
}

void UOnlineManagerSubsystem::OnNetworkFailure(UWorld* InWorld, UNetDriver* NetDriver,  ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LOG_COLOR_TIME(ONLINE_MANAGER_USE_LOG, "Crash " + FString(ENetworkFailure::ToString(FailureType)), RED, 5);
	QuitServerCrash();
}

void UOnlineManagerSubsystem::OnSessionFailure(const FUniqueNetId& _userId, ESessionFailure::Type _reason)
{
	LOG_ERROR(ONLINE_MANAGER_USE_LOG, "Session Failure");
	QuitServerCrash();
}

bool UOnlineManagerSubsystem::VerifBanned(FString _banList)
{
	FString _wordVerif;
	FString separt = "/";
	int _indexSecur = 0;
	FString _myID = GetWorld()->GetFirstPlayerController()->PlayerState->GetUniqueId()->ToString() + "/";
	LOG_COLOR_TIME(ONLINE_MANAGER_USE_LOG, "BanList => " + _banList, FColor::Orange, 30);
	int _length = _banList.Len();
	for (int _index = 0; _index < _length; _index++)
	{
		if (_myID[_index - _indexSecur] == _banList[_index] && _banList[_index] != separt[0])
		{
			_wordVerif += _banList[_index];
		}
		else if (_banList[_index] == separt[0])
		{
			_wordVerif += "/";
			LOG_COLOR_TIME(ONLINE_MANAGER_USE_LOG, "WordVerif => " + _wordVerif + " == _myID => " + _myID,  FColor::Orange, 30);
			if (_myID == _wordVerif)
				return true;
			else
			{
				_indexSecur = _index + 1;
				_wordVerif = "";
				continue;
			}
		}
	}
	
	return false;
}
#pragma endregion
//=========== OTHER ==============   
