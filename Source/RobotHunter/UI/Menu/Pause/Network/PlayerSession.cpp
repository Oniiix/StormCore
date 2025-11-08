#include "PlayerSession.h"

#include "GameFramework/PlayerState.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"


void UPlayerSession::RegisterEvents()
{
	Super::RegisterEvents();

	ACustomGameState* _gs = GetWorld()->GetGameState<ACustomGameState>();
	BIND_ACTION(_gs, OnPlayerJoin, UPlayerSession, AddPlayerToList);
}

void UPlayerSession::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	currentTime += InDeltaTime;
	if (currentTime >= settings->GetTimeRefresh())
	{
		ACustomGameState* _gs = GetWorld()->GetGameState<ACustomGameState>();
		TArray<TObjectPtr<APlayerState>> _ps = _gs->PlayerArray;

		RefreshPings(_ps[0], FConnection, FPing);
		if (_ps.Num() == 2)
		{
			RefreshPings(_ps[1], FConnection, SPing);
		}

		currentTime = 0.f;
	}
}

void UPlayerSession::Refresh()
{
	Super::Refresh();

	FTimerHandle _fTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_fTimerHandle, this, &UPlayerSession::RefreshDelay, .1f, false, 0.1f);
}

void UPlayerSession::LoadInformationPlayer(const TObjectPtr<APlayerState>& _player,
                                           const TObjectPtr<UTextPrefab>& _text, const TObjectPtr<UImage>& _pdp) const
{
	SET_TEXT_STR(_text->GetText(), _player->GetPlayerName());
}

void UPlayerSession::RefreshPings(const APlayerState* _player, UImage* _pingImage, UTextPrefab* _pingText)
{
	int _ping = _player->ExactPing;

	SET_TEXT_STR(_pingText->GetText(), "" + INTSTR(_ping) + " ms");


	EConnectionStatePlayer current = _ping <= StateConnection[Full].pingMax
		                                 ? Full
		                                 : _ping <= StateConnection[Three_Bar].pingMax
		                                 ? Three_Bar
		                                 : _ping <= StateConnection[Two_Bar].pingMax
		                                 ? Two_Bar
		                                 : _ping <= StateConnection[One_Bar].pingMax
		                                 ? One_Bar
		                                 : current = No_Connection;

	_pingImage->SetBrushFromTexture(StateConnection[current].texture);
}

void UPlayerSession::EnableLayerPlayer(UVerticalBox* _layer, bool _visibility) const
{
	_layer->SetVisibility(_visibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_layer->SetIsEnabled(_visibility);
}

void UPlayerSession::AddPlayerToList(AMainCharacter* _player)
{
	Refresh();
}

void UPlayerSession::RefreshDelay()
{
	EnableLayerPlayer(FirstPlayer, false);
	EnableLayerPlayer(SecondPlayer, false);

	ACustomGameState* _gs = GetWorld()->GetGameState<ACustomGameState>();
	TArray<TObjectPtr<APlayerState>> _ps = _gs->PlayerArray;

	//EnableLayerPlayer(FirstPlayer, true);
	//LoadInformationPlayer(_ps[0], textNameFirstPlayer, FPdp);
	//RefreshPings(_ps[0], FConnection, FPing);
	//if (_ps.Num() == 2)
	//{
	//	EnableLayerPlayer(SecondPlayer, true);
	//	LoadInformationPlayer(_ps[1], textNameSecondPlayer, SPdp);
	//	RefreshPings(_ps[1], SConnection, SPing);
	//}
}
