// Fill out your copyright notice in the Description page of Project Settings.


#include "StockInteractableActor.h"
#include <Kismet/KismetSystemLibrary.h>
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/UI/Menu/Hub/StockMenu/NewStock.h"
#include "RobotHunter/Utils/DebugUtils.h"


AStockInteractableActor::AStockInteractableActor()
{
	bReplicates = true;
}

void AStockInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	onBlendEnd.AddDynamic(this, &AStockInteractableActor::ShowMenu);
	AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	CHECK_NULL(_hud, "Hud is null");
	game = Cast<ACustomGameState>(GetWorld()->GetGameState());

	stockUI = _hud->GetUI<UNewStock>(EStateUI::HubStock);
	CHECK_NULL(stockUI, "Stock ui is null");
	stockUI->OnChangeMember().AddUniqueDynamic(this, &AStockInteractableActor::VisibleChangeMesh);
	stockUI->SetOwner(this);
}

void AStockInteractableActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AStockInteractableActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
	//characterPtr = _character;
	//game = Cast<ACustomGameState>(GetWorld()->GetGameState());
	//_character->OnQuitMenu().AddUniqueDynamic(this, &AStockInteractableActor::ExitMenu);
	//_character->SetCanMove(false);
	//_character->GetMovementComponent()->StopMovementImmediately();
	//if (currentCharacter == "") //TODO A Optimiser 
	//{
	//	if (HasAuthority())
	//	{
	//		currentCharacter = _character->GetPlayerState()->GetName();
	//	}
	//	else
	//	{
	//		ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(
	//			GetWorld()->GetFirstPlayerController());
	//		UNetworkGPEComponent* NetworkComponent = _customPlayerController->FindComponentByClass<
	//			UNetworkGPEComponent>();
	//		if (NetworkComponent)
	//		{
	//			NetworkComponent->ServerRPC_SetCurrentCharacter(this, _character->GetPlayerState()->GetName());
	//		}
	//		currentCharacter = _character->GetPlayerState()->GetName();
	//	}
	//}
	//else
	//{
	//	_character->SetActorLocation(GetActorLocation() + FVector(0, 0, 75));
	//	_character->SetMeshRotation(FRotator(_character->GetActorRotation().Pitch, GetActorRotation().Yaw + 180,
	//	                                     _character->GetActorRotation().Roll), true);
	//	//_character->SetActorRotation(FRotator(_character->GetActorRotation().Pitch, GetActorRotation().Yaw + 180,_character->GetActorRotation().Roll),ETeleportType::TeleportPhysics);
	//	if (game->GetPlayers().Num() < 2)
	//		return;
	//	if (HasAuthority()) //TODO A Mettre dans une variable
	//	{
	//		LOG_COLOR(true, " Set visibility false", FColor::Green);
	//		if (game->GetPlayers()[0] == _character)
	//			SetCharacterVisibility(game->GetPlayers()[1], false);
	//		else
	//			SetCharacterVisibility(game->GetPlayers()[0], false);
	//		if (game->GetPlayers()[0] == _character)
	//			NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], false);
	//		else
	//			NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], false);
	//	}
	//	else
	//	{
	//		SetOtherCharacterVisibility_Client(_character);
	//	}
	//}
	//if (HasAuthority())
	//{
	//	_character->SetActorLocation(GetActorLocation() + FVector(0, 0, 75), false, nullptr,
	//	                             ETeleportType::TeleportPhysics);
	//	_character->SetMeshRotation(FRotator(_character->GetActorRotation().Pitch, GetActorRotation().Yaw + 180,
	//	                                     _character->GetActorRotation().Roll), true);
	//}
	//else
	//{
	//	ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(
	//		GetWorld()->GetFirstPlayerController());
	//	UNetworkGPEComponent* NetworkComponent = _customPlayerController->FindComponentByClass<UNetworkGPEComponent>();
	//	if (NetworkComponent)
	//	{
	//		NetworkComponent->ServerRPC_SetPositionClient(_character, GetActorLocation(), GetActorRotation());
	//	}
	//}
}

void AStockInteractableActor::LaunchAnimation(AMainCharacter* _character)
{
}


void AStockInteractableActor::SetCharacterVisibility(const AMainCharacter* Character, bool bVisible)
{
	if (Character)
		if (USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>())
		{
			Mesh->SetOnlyOwnerSee(!bVisible);
			for (TObjectPtr<AMembersActor> _memberActor : Character->GetUsingMembers())
			{
				if (!_memberActor)
					continue;
				for (USkeletalMeshComponent* _memberMesh : _memberActor->GetMemberMeshs())
				{
					_memberMesh->SetOnlyOwnerSee(!bVisible);
				}
			}
		}
}

void AStockInteractableActor::NetMuticastRPC_SetCharacterVisibility_Implementation(
	AMainCharacter* Character, bool bVisible)
{
	if (HasAuthority())
		return;
	LOG_COLOR(false, " SetCharacterVisibility_Client_Implementation", FColor::Green);
	if (USkeletalMeshComponent* Mesh = Character->FindComponentByClass<USkeletalMeshComponent>())
	{
		Mesh->SetOnlyOwnerSee(!bVisible); // Visible uniquement pour le propriétaire (Client/Serveur spécifique).
		for (TObjectPtr<AMembersActor> _memberActor : Character->GetUsingMembers())
		{
			if (!_memberActor)
				continue;
			for (USkeletalMeshComponent* _memberMesh : _memberActor->GetMemberMeshs())
			{
				_memberMesh->SetOnlyOwnerSee(!bVisible);
			}
		}
	}
}

void AStockInteractableActor::VisibleChangeMesh()
{
	if (game->GetPlayers().Num() < 2)
		return;
	mainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (HasAuthority())
		{
			if (game->GetPlayers()[0] == mainCharacter)
				SetCharacterVisibility(game->GetPlayers()[1], false);
			else
				SetCharacterVisibility(game->GetPlayers()[0], false);
			if (game->GetPlayers()[0] == mainCharacter)
				NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], false);
			else
				NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], false);
		}
		else
		{
			SetOtherCharacterVisibility_Client(mainCharacter);
		}
	}, 0.5f, false);
	LOG_COLOR(false, " VisibleChangeMesh IN STOCK ", FColor::Green);
}


void AStockInteractableActor::SetOtherCharacterVisibility_Client(const AMainCharacter* _character)
{
	if (game->GetPlayers()[0] == _character)
		SetCharacterVisibility(game->GetPlayers()[1], false);
	else
		SetCharacterVisibility(game->GetPlayers()[0], false);
	ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(
		GetWorld()->GetFirstPlayerController());
	UNetworkGPEComponent* NetworkComponent = _customPlayerController->FindComponentByClass<UNetworkGPEComponent>();
	if (NetworkComponent)
	{
		if (game->GetPlayers()[0] == _character)
			NetworkComponent->ServerRPC_RequestSetInvisibleForTarget(game->GetPlayers()[0], true);
		else
			NetworkComponent->ServerRPC_RequestSetInvisibleForTarget(game->GetPlayers()[1], true);
	}
}

void AStockInteractableActor::Interacted_Internal()
{
	characterPtr = player;
	Super::Interacted_Internal();
	game = Cast<ACustomGameState>(GetWorld()->GetGameState());
	characterPtr->OnQuitMenu().AddUniqueDynamic(this, &AStockInteractableActor::ExitMenu);
	characterPtr->SetCanMove(false);
	characterPtr->GetMovementComponent()->StopMovementImmediately();
	if (currentCharacter == "") //TODO A Optimiser 
	{
		if (HasAuthority())
		{
			currentCharacter = characterPtr->GetPlayerState()->GetName();
		}
		else
		{
			ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(
				GetWorld()->GetFirstPlayerController());
			UNetworkGPEComponent* NetworkComponent = _customPlayerController->FindComponentByClass<
				UNetworkGPEComponent>();
			if (NetworkComponent)
			{
				NetworkComponent->ServerRPC_SetCurrentCharacter(this, characterPtr->GetPlayerState()->GetName());
			}
			currentCharacter = characterPtr->GetPlayerState()->GetName();
		}
	}
	else
	{
		characterPtr->SetActorLocation(GetActorLocation() + FVector(0, 0, 75));
		characterPtr->SetMeshRotation(FRotator(characterPtr->GetActorRotation().Pitch, GetActorRotation().Yaw + 180,
		                                       characterPtr->GetActorRotation().Roll), true);

		//_character->SetActorRotation(FRotator(_character->GetActorRotation().Pitch, GetActorRotation().Yaw + 180,_character->GetActorRotation().Roll),ETeleportType::TeleportPhysics);
		if (game->GetPlayers().Num() < 2)
			return;
		if (HasAuthority()) //TODO A Mettre dans une variable
		{
			LOG_COLOR(true, " Set visibility false", FColor::Green);
			if (game->GetPlayers()[0] == characterPtr)
				SetCharacterVisibility(game->GetPlayers()[1], false);
			else
				SetCharacterVisibility(game->GetPlayers()[0], false);
			if (game->GetPlayers()[0] == characterPtr)
				NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], false);
			else
				NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], false);
		}
		else
		{
			SetOtherCharacterVisibility_Client(characterPtr);
		}
	}
	if (HasAuthority())
	{
		characterPtr->SetActorLocation(GetActorLocation() + FVector(0, 0, 75), false, nullptr,
		                               ETeleportType::TeleportPhysics);
		characterPtr->SetMeshRotation(FRotator(characterPtr->GetActorRotation().Pitch, GetActorRotation().Yaw + 180,
		                                       characterPtr->GetActorRotation().Roll), true);
	}
	else
	{
		ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(
			GetWorld()->GetFirstPlayerController());
		UNetworkGPEComponent* NetworkComponent = _customPlayerController->FindComponentByClass<UNetworkGPEComponent>();
		if (NetworkComponent)
		{
			NetworkComponent->ServerRPC_SetPositionClient(characterPtr, GetActorLocation(), GetActorRotation());
		}
	}
}

void AStockInteractableActor::ShowMenu()
{
	AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	_hud->SetState(EStateUI::HubStock);
}

void AStockInteractableActor::ExitMenu()
{
	if (game->GetPlayers().Num() < 2)
	{
		characterPtr->SetCanMove(true);
		game->GetPlayers()[0]->OnQuitMenu().RemoveDynamic(this, &AStockInteractableActor::ExitMenu);
		return;
	}
	NetworkExitMenu();
	DebugMenu();
}

void AStockInteractableActor::NetworkExitMenu()
{
	if (HasAuthority())
	{
		characterPtr->SetCanMove(true);
		SetCharacterVisibility(game->GetPlayers()[0], true);
		SetCharacterVisibility(game->GetPlayers()[1], true);
		NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[0], true);
		NetMuticastRPC_SetCharacterVisibility(game->GetPlayers()[1], true);
	}
	else
	{
		SetCharacterVisibility(game->GetPlayers()[1], true);
		SetCharacterVisibility(game->GetPlayers()[0], true);
		ACustomPlayerController* _customPlayerController = Cast<ACustomPlayerController>(
			GetWorld()->GetFirstPlayerController());
		UNetworkGPEComponent* NetworkComponent = _customPlayerController->FindComponentByClass<UNetworkGPEComponent>();
		if (NetworkComponent)
		{
			NetworkComponent->ServerRPC_RequestSetInvisibleForTarget(game->GetPlayers()[0], false);
			NetworkComponent->ServerRPC_RequestSetInvisibleForTarget(game->GetPlayers()[1], false);
		}
	}
	int num = game->GetPlayers().Num();
	for (int i = 0; i < num; i++)
	{
		game->GetPlayers()[i]->OnQuitMenu().RemoveDynamic(this, &AStockInteractableActor::ExitMenu);
	}
}

void AStockInteractableActor::DebugMenu()
{
	//LOG(true, INTSTR(game->GetPlayers()[0]->OnQuitMenu().GetAllObjects().Num()));
	LOG(true, "DebugMenu Interact");
}

void AStockInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Ajoutez ici les propriétés devant être répliquées avec la macro DOREPLIFETIME
	DOREPLIFETIME(AStockInteractableActor, currentCharacter);
	DOREPLIFETIME(AStockInteractableActor, isInteracting);
}
