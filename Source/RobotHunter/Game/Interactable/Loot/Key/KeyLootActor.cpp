// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyLootActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <GameFramework/PlayerState.h>
#include <GameFramework/GameStateBase.h>
#include <Net/UnrealNetwork.h>

#define USE_DEBUG_KEY_ACTOR 0
AKeyLootActor::AKeyLootActor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	SetReplicateMovement(true);

}

void AKeyLootActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);
	
	//HasAuthority() ? AttachActor(_character) : _character->ServerRPC_InteractableActorBehaviour(this);

}

//void AKeyLootActor::AttachActor(AMainCharacter* _character)
//{
//
//	if (!_character )
//		return;
//	asCharacter = true;
//	characterOwner = _character;
//	
//	On_RepAsCharacter();
//	
//}
//void AKeyLootActor::DetachActor()
//{
//	if (!characterOwner || !asCharacter)
//		return;
//
//	asCharacter = false;
//	characterOwner = nullptr;
//	SetOwner(nullptr);
//	
//}



//void AKeyLootActor::ServerRPC_Interacted()
//{
//	//Attach in server character controlled by second player
//	AttachActor(Cast<AMainCharacter>(GetWorld()->GetGameState()->PlayerArray[1]->GetPawn()));
//}



//
//void AKeyLootActor::On_RepAsCharacter()
//{
//	if(characterOwner)
//	LOG_COLOR(USE_DEBUG_KEY_ACTOR, "On_RepCharacterOwner: " + characterOwner->GetName(), PINK);
//	else
//	{
//		LOG_COLOR(USE_DEBUG_KEY_ACTOR, "On_RepCharacterOwner: NULL", PINK);
//	}
//}

void AKeyLootActor::Carry(AMainCharacter* _character)
{
	Super::Carry(_character);
}

void AKeyLootActor::Drop(AMainCharacter* _character)
{
	Super::Drop(_character);
}

//void AKeyLootActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AKeyLootActor, asCharacter);
//	DOREPLIFETIME(AKeyLootActor, characterOwner);
//}
