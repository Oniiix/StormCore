// Objectif 3D Copyright


#include "PhazerBeaconActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"

APhazerBeaconActor::APhazerBeaconActor()
{
	//TODO APhazerBeaconActor : remove Debug in tick and deactivate the tick
	PrimaryActorTick.bCanEverTick = true;
}


void APhazerBeaconActor::PressBehavior()
{
	/*if (hasPressed || !canShot) return ;*/

	//hasPressed = true;

	if (!HasAuthority())
	{
		ServerRpc_SpawnFirstBeacon(mainCharacter->GetActorLocation());
	}

	firstBeacon = GetWorld()->SpawnActor<ABeaconActor>(beaconRef, mainCharacter->GetActorLocation(), FRotator::ZeroRotator);
	firstBeacon->SetOwner(mainCharacter);

	//GetWorldTimerManager().SetTimer(loadingTimer, this, &APhazerBeaconActor::LoadingFinish, loadingTime);
	GetWorldTimerManager().SetTimer(maxLimitLoadingTimer, this, &APhazerBeaconActor::MaxLimitLoadingFinish, maxLimitLoadingTime);

}

void APhazerBeaconActor::ReleaseBehavior()
{
	if (!hasPressed) return;

	if (GetWorldTimerManager().GetTimerElapsed(maxLimitLoadingTimer) > loadingTime)
	{
		//TODO APhazerBeaconActor : Change that to juste a snap in the BeginPlay of Beacon 
		FVector _lookPosition = camera->GetComponentLocation() + camera->GetForwardVector() * teleportationMaxDistance;
		FHitResult _result;
		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), camera->GetComponentLocation(), camera->GetComponentLocation() + camera->GetForwardVector() * teleportationMaxDistance,
			lookLayers, false, {}, EDrawDebugTrace::ForDuration, _result, true))
		{
			if (_result.GetActor()->GetRootComponent()->GetCollisionObjectType() != correctLayer.GetValue())
			{
				_lookPosition = camera->GetComponentLocation() + camera->GetForwardVector() * (_result.Distance - wallOffset);
				UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _lookPosition, _lookPosition + FVector::DownVector * 10000, lookLayers, false, {}, EDrawDebugTrace::None, _result, true);
			}
		}
		else
			UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _lookPosition, _lookPosition + FVector::DownVector * 10000, lookLayers, false, {}, EDrawDebugTrace::None, _result, true);

		LOG_COLOR(PHAZER_BEACON_USE_LOG, "APhazerBeaconActor : Loading Finish => TP", PINK);

		if (!HasAuthority())
			ServerRpc_Teleport(_result.Location);
		else
			Teleport(_result.Location);
		//loadingFinish = false;
		Super::ReleaseBehavior();
	}
	else
		MaxLimitLoadingFinish();

	GetWorldTimerManager().ClearTimer(maxLimitLoadingTimer);
	hasPressed = false;
}

void APhazerBeaconActor::Teleport(const FVector& _location)
{
	ABeaconActor* _secondBeacon = GetWorld()->SpawnActor<ABeaconActor>(beaconRef, _location, FRotator::ZeroRotator);

	TArray<FTeleportationData> _firstBeaconActors = firstBeacon->GetActorAround(teleportationRadius);
	LOG_COLOR(PHAZER_BEACON_USE_LOG, "APhazerBeaconActor : First Around => " + FString::FromInt(_firstBeaconActors.Num()), BLUE);

	TArray<FTeleportationData> _secondBeaconActors = _secondBeacon->GetActorAround(teleportationRadius);
	LOG_COLOR(PHAZER_BEACON_USE_LOG, "APhazerBeaconActor : Second Around => " + FString::FromInt(_secondBeaconActors.Num()), CYAN);

	firstBeacon->TeleportAllActors(_secondBeaconActors);
	_secondBeacon->TeleportAllActors(_firstBeaconActors);
	
	GetWorld()->DestroyActor(_secondBeacon);
	GetWorld()->DestroyActor(firstBeacon);
}

void APhazerBeaconActor::InitMember()
{
	Super::InitMember();
	camera = mainCharacter->GetCameraComponent()->GetCamera();
}

void APhazerBeaconActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (hasPressed && useDebug)
		DrawPhazerDebug();
}

void APhazerBeaconActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APhazerBeaconActor, firstBeacon);
}


//void APhazerBeaconActor::LoadingFinish()
//{
//	LOG_COLOR(PHAZER_BEACON_USE_LOG, "APhazerBeaconActor : Loading Finish", GREEN);
//	loadingFinish = true;
//}

void APhazerBeaconActor::MaxLimitLoadingFinish()
{
	//GetWorldTimerManager().ClearTimer(loadingTimer);
	if (!HasAuthority())
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_DestroyCustomActor(firstBeacon);
	else
		GetWorld()->DestroyActor(firstBeacon);
	//loadingFinish = false;
}

void APhazerBeaconActor::DrawPhazerDebug()
{
	FHitResult _result;
	FVector _lookPosition = camera->GetComponentLocation() + camera->GetForwardVector() * teleportationMaxDistance;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), camera->GetComponentLocation(), _lookPosition,
		lookLayers, false, {}, EDrawDebugTrace::None, _result, true))
	{
		if (_result.GetActor()->GetRootComponent()->GetCollisionObjectType() != correctLayer.GetValue())
		{
			_lookPosition = camera->GetComponentLocation() + camera->GetForwardVector() * (_result.Distance - wallOffset);
			UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _lookPosition, _lookPosition + FVector::DownVector * 10000, lookLayers, false, {}, EDrawDebugTrace::ForOneFrame, _result, true);
		}
	}
	else
		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _lookPosition, _lookPosition + FVector::DownVector * 10000, lookLayers, false, {}, EDrawDebugTrace::None, _result, true);
	DRAW_SPHERE(_result.Location, teleportationRadius, GetWorldTimerManager().GetTimerElapsed(maxLimitLoadingTimer) > loadingTime ? FColor::Green : FColor::Orange);
}






void APhazerBeaconActor::ServerRpc_SpawnFirstBeacon_Implementation(const FVector& _location)
{
	LOG_COLOR(PHAZER_BEACON_USE_LOG, "APhazerBeaconActor : Spawn First Beacon", BLUE);
	firstBeacon = GetWorld()->SpawnActor<ABeaconActor>(beaconRef, _location, FRotator::ZeroRotator);
}

void APhazerBeaconActor::ServerRpc_Teleport_Implementation(const FVector& _location)
{
	Teleport(_location);
}


void APhazerBeaconActor::OnRep_FirstBeaconUpdate()
{
	if (firstBeacon && mainCharacter->GetIsOwner())
	{
		firstBeacon->SetOwner(mainCharacter);

		//GetWorldTimerManager().SetTimer(loadingTimer, this, &APhazerBeaconActor::LoadingFinish, loadingTime);
		GetWorldTimerManager().SetTimer(maxLimitLoadingTimer, this, &APhazerBeaconActor::MaxLimitLoadingFinish, maxLimitLoadingTime);
	}
}
