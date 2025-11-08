#include "SlotServerTemplate.h"
#include "Kismet/KismetStringLibrary.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/Menu/MainMenu/ListServerMenu/ListServerMenu.h"
#include "RobotHunter/Utils/DebugUtils.h"

void USlotServerTemplate::LoadTextWidgetWithClamp(FString _nameServer, UTextPrefab* _widget,
                                                  const FString& _format) const
{
	FString _textClamp = "";

	_textClamp = _nameServer.Len() > lengthOfMaxSizeText
		             ? UKismetStringLibrary::GetSubstring(_nameServer, 0, lengthOfMaxSizeText) + "..."
		             : _nameServer;

	SET_TEXT_STR(_widget->GetText(), FORMAT(_format, _textClamp));
}

void USlotServerTemplate::InitServer(const UListServerMenu* _owner, FServerResult _server)
{
	sessionName = _server.sessionName;
	ownerClass = _owner;

	//SET_TEXT_STR(serverName->GetText(), "test");

	LoadTextWidgetWithClamp(_server.serverName, serverName, ownerNameFormat);
	LoadTextWidgetWithClamp(_server.name, owner, ownerNameFormat);

	BIND_ACTION(serverButton, OnClick, USlotServerTemplate, Connect);
}

void USlotServerTemplate::LaunchMap()
{
	//ownerClass->GetHud()->SetState(EStateUI::Loading);
	Cast<UCustomGameInstance>(GetWorld()->GetGameInstance())->OnLevelLoad().RemoveDynamic(
		this, &USlotServerTemplate::LaunchMap);

	UOnlineManagerSubsystem* oms = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>();
	oms->JoinServer(sessionName, oms->GetSessionSearchResult(sessionName));

	//Cast<UCustomGameInstance>(GetWorld()->GetGameInstance())->SetGameStatus(HUB);
}

void USlotServerTemplate::Connect()
{
	Cast<UCustomGameInstance>(GetWorld()->GetGameInstance())->CallbackOnLevelChange(ETypeChange::MainToHub);
	LaunchMap();
}
