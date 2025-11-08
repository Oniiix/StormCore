#include "EndDayActor.h"
#include <RobotHunter/Network/OnlineManagerSubsystem.h>
#include "../../CustomGameMode.h"
#include "../../../CCC/Character/MainCharacter.h"
//#include "../Wagon/RHWagonActor.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"

AEndDayActor::AEndDayActor()
{
	bReplicates = true;

	interactionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	interactionSphere->SetGenerateOverlapEvents(true);
	interactionSphere->SetupAttachment(RootComponent);
	AddOwnedComponent(inventory);

	interactionRadius = 250.0f;
	holdingTime = 2.0f;
	players = TArray<AMainCharacter*>();
	//wagon = nullptr;


	inputsSetup = false;
	holdTime = 0.0f;
	holdTimer = FTimerHandle();
}

void AEndDayActor::UpdateInteractionSphere()
{
	interactionSphere->SetSphereRadius(interactionRadius);
}

void AEndDayActor::UpdateHoldTime()
{
	holdTime += 1.0f;
	if (holdTime >= holdingTime)
		EndDay();
}

void AEndDayActor::EndDay()
{
	TArray<AMainCharacter*> _players = GetWorld()->GetGameState<ACustomGameState>()->GetPlayers();
	if (players.Num() < _players.Num())
	{
		LOG_COLOR(true, "GAMESTATE: " + FString::FromInt(_players.Num()), ORANGE);
		return;
	}

	//UCustomGameInstance* _gm = GetWorld()->GetGameInstance<UCustomGameInstance>(); 

	for (AMainCharacter* _player : players)
		UpdatePlayerMoney(_player);

	GetWorld()->GetTimerManager().ClearTimer(holdTimer);/*
	if (UOnlineManagerSubsystem* _manager = GetWorld()->GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>())
	{
		LOG_COLOR(true, "NICE", ORANGE);
		_manager->StopServer();
		_manager->LoadMap(hubLevel.GetAssetName() + "?listen");
	}*/
}

void AEndDayActor::UpdatePlayerMoney(AMainCharacter* _player)
{
	UInventoryComponent* _inventoryComp = _player->GetInventoryComponent();

	if (_inventoryComp)
		_inventoryComp->SetMoney(_inventoryComp->GetMoney() + inventory->GetTotalInventoryMoney());
}

void AEndDayActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AMainCharacter* _mc = Cast<AMainCharacter>(OtherActor);
	//ARHWagonActor* _wagon = Cast<ARHWagonActor>(OtherActor);

	if (_mc)
		players.Add(_mc);

	//if (_wagon)
	//	wagon = _wagon;
}

void AEndDayActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	AMainCharacter* _mc = Cast<AMainCharacter>(OtherActor);
	//ARHWagonActor* _wagon = Cast<ARHWagonActor>(OtherActor);

	if (_mc)
		players.Remove(_mc);

	//if (_wagon)
	//	wagon = nullptr;
}

#if WITH_EDITOR
void AEndDayActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool _canUpdateInteractionSphere = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AEndDayActor, interactionRadius);

	if (_canUpdateInteractionSphere)
		UpdateInteractionSphere();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AEndDayActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);

	//SetOwner(_character);
	//if (_character->HasAuthority())
		UpdateInventoryFromPlayer(_character);

	//R�cup�rer les loots du wagon

	FTimerManager& _timer = GetWorld()->GetTimerManager();

	if (_timer.IsTimerActive(holdTimer))
		return;

	_timer.SetTimer(holdTimer, this, &AEndDayActor::UpdateHoldTime, 1.0f, true);
}

void AEndDayActor::UpdateInventoryFromPlayer(AMainCharacter* _player)
{
	UInventoryComponent* _inventoryComp = _player->GetInventoryComponent();

	if (!_inventoryComp)
		return;

		FCustomMap _inventory = _inventoryComp->GetItemsQuantity();

	if (_inventory.IsEmpty())
	{
		UKismetSystemLibrary::PrintString(this, "INVENTAIRE VIDE", true, true, FColor::Magenta, 10);
		return;
	}
	for (FCustomKeyValuePair _items : _inventory.Get())
	{
		inventory->AddItem(_items.GetKey(), _items.GetValue());
	}
	if (_inventoryComp->GetHasEgg())
		inventory->AddItem("Egg", 1);
	_inventoryComp->ClearInventory();

}
