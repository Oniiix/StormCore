#include "StopHandcarActor.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "Kismet/GameplayStatics.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"

AStopHandcarActor::AStopHandcarActor()
{
	buttonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	buttonMesh->SetupAttachment(RootComponent);

	playerPosition = CreateDefaultSubobject<USceneComponent>("PlayerPosition");
	playerPosition->SetupAttachment(buttonMesh);

	stopZone = CreateDefaultSubobject<UBoxComponent>("StopZone");
	stopZone->SetupAttachment(RootComponent);
	stopZone->SetGenerateOverlapEvents(true);

	disembarkZone = CreateDefaultSubobject<UBoxComponent>("DisembarkZone");
	disembarkZone->SetupAttachment(RootComponent);
	disembarkZone->SetGenerateOverlapEvents(true);

	autoModeWays = TMap<AStopHandcarActor*, TEnumAsByte<ERailWay>>();

	manager = nullptr;
	handcar = nullptr;
	//player = nullptr;

	calledHandcar = false;

	//movePlayer = false;
	//rotatePlayer = false;
}

void AStopHandcarActor::Init()
{
	ACustomGameState* _gs = GetWorld()->GetGameState<ACustomGameState>();

	if (_gs)
	{
		manager = _gs->GetHandcarManager();

		if (manager)
			manager->AddStop(this);
	}

	APlayerController* _controller = GetWorld()->GetFirstPlayerController();

	if (_controller)
		networkComp = _controller->GetComponentByClass<UNetworkGPEComponent>();

	if (stopZone)
	{
		stopZone->OnComponentBeginOverlap.AddDynamic(this, &AStopHandcarActor::OnStopZoneBeginOverlap);
		stopZone->OnComponentEndOverlap.AddDynamic(this, &AStopHandcarActor::OnStopZoneEndOverlap);
	}

	if (disembarkZone)
	{
		disembarkZone->OnComponentBeginOverlap.AddDynamic(this, &AStopHandcarActor::OnDisembarkZoneBeginOverlap);
		disembarkZone->OnComponentEndOverlap.AddDynamic(this, &AStopHandcarActor::OnDisembarkZoneEndOverlap);
	}
}

void AStopHandcarActor::MovePlayerToPosition()
{
	//if (movePlayer && playerPosition && player)
	//{
	//	LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "MovePlayerToPosition", FColor::Blue, 1.0f);
	//	
	//	const FVector _meshForward = player->GetMesh()->GetForwardVector() * 0.5f;
	//	player->AddMovementInput(_meshForward);

	//	const FVector _position = playerPosition->GetComponentLocation();
	//	const FVector _playerLoc = player->GetActorLocation();

	//	const float _distance = FVector2D::Distance(FVector2D(_position), FVector2D(_playerLoc));

	//	if (_distance <= 50.0f)
	//		movePlayer = false;
	//}
}

void AStopHandcarActor::RotatePlayerTowardsPosition()
{
	//if (rotatePlayer && buttonMesh && playerPosition && player)
	//{
	//	LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, ":RotatePlayerTowardsPosition", FColor::Blue, 1.0f);
	//	const FVector _buttonLoc = buttonMesh->GetComponentLocation();
	//	const FVector _position = playerPosition->GetComponentLocation();
	//	const FVector _playerLoc = player->GetActorLocation();

	//	FRotator _rotation = movePlayer ? UKismetMathLibrary::FindLookAtRotation(_playerLoc, FVector(_position.X, _position.Y, _playerLoc.Z))
	//							: UKismetMathLibrary::FindLookAtRotation(_playerLoc, FVector(_buttonLoc.X, _buttonLoc.Y, _playerLoc.Z));

	//	_rotation.Yaw = _rotation.Yaw - player->GetActorRotation().Yaw;
	//	player->
	// (_rotation, !movePlayer);

	//	if (!movePlayer)
	//	{
	//		rotatePlayer = false;
	//		CallHandcar();
	//	}
	//}
}

void AStopHandcarActor::CallHandcar()
{
	if (manager)
	{
		// Vérifiez les prérequis
		if (/*!player ||*/ calledHandcar)
		{
			LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "Cannot call Handcar: Player is null or Handcar already called", FColor::Yellow, 3.0f);
			return;
		}

		LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "Calling Handcar...", FColor::Green, 3.0f);

		// Exécutez l'appel à l'acteur manager
		manager->CallHandcar(this);

		// S'assurer que la variable est mise à jour après l'appel
		calledHandcar = true;

		if (HasAuthority())
		{
			LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "[Server] Updating CalledHandcar State", FColor::Red, 3.0f);
			OnRep_UpdateCalledHandcar(calledHandcar);
		}
		else
		{
			if (networkComp)
			{
				LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "[Client] Sending RPC to update CalledHandcar State", FColor::Red, 3.0f);
				networkComp->ServerRPC_UpdateCalledHandcar(this, calledHandcar);
			}
		}
	}
	else
	{
		LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "Manager is null: Cannot call Handcar", FColor::Red, 3.0f);
	}
}

void AStopHandcarActor::OnStopZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ACustomHandcarActor>())
	{
		ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(OtherActor);

		if (_handcar)
		{
			onHandcarOverlap.Broadcast(true);

			if (calledHandcar)
			{
				if (_handcar->GetInAutoMode())
				{
					if (_handcar->GetNextStop() == this)
					{
						ABrakeHandcarActor* _brake = _handcar->GetBrakeActor();

						if (_brake)
						{
							_brake->SetReleased(false);
							_brake->InstantBrake(false);
						}
					}
				}
			}

			//_handcar->SetIsAtStop(true);
			//_handcar->SetCurrentStop(this);
			handcar = _handcar;
		}
	}
}

void AStopHandcarActor::OnStopZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ACustomHandcarActor>())
	{
		ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(OtherActor);

		if (_handcar)
		{
			if (calledHandcar)
			{
				LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "OnStopZoneEndOverlap: Resetting calledHandcar state to avoid issues.", FColor::Green, 2.0f);
				calledHandcar = false;
				handcar->RemoveCallingStop(this);
			}
			onHandcarOverlap.Broadcast(false);
			handcar = nullptr;
			LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "OnStopZoneEndOverlap: Handcar has left the stop zone.", FColor::Yellow, 2.0f);

		}
	}
}

void AStopHandcarActor::OnDisembarkZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ACustomHandcarActor>())
	{
		ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(OtherActor);

		if (_handcar)
		{
			if (calledHandcar && _handcar->GetNextStop() == this)
			{
				LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "OnDisembarkZoneBeginOverlap: Resetting calledHandcar to false.", FColor::Green, 3.0f);

				if (_handcar->GetInAutoMode())
				{
					_handcar->UpdateNextStop();
				}
				else
				{
					_handcar->RemoveCallingStop(this); // Suppression du stop en appel
				}
    
				calledHandcar = false; // Réinitialisation explicite
				LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "calledHandcar successfully reset to false.", FColor::Green, 3.0f);
			}

			_handcar->SetIsAtStop(true);
			_handcar->SetCurrentStop(this);
		}
	}
}

void AStopHandcarActor::OnDisembarkZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<ACustomHandcarActor>())
	{
		ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(OtherActor);

		if (_handcar)
		{
			_handcar->SetIsAtStop(false);
			_handcar->SetCurrentStop(nullptr);
		}
	}
}

void AStopHandcarActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _initTimer;
	GetWorld()->GetTimerManager().SetTimer(_initTimer, this, &AStopHandcarActor::Init, 0.5f, false);
}

void AStopHandcarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//RotatePlayerTowardsPosition();
	//MovePlayerToPosition();
}

void AStopHandcarActor::Interacted(AMainCharacter* _character)
{
	LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, FString("Interaction initiated: calledHandcar = ") + (calledHandcar ? "true" : "false"), FColor::Yellow, 3.0f);
	LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "Interacted: Proceeding to call handcar...", FColor::Green, 1.0f);

	//// Ajout d'une validation supplémentaire pour éviter les problèmes
	//const FVector _positionForward = buttonMesh->GetForwardVector();
	//const FVector _locationDiff = (_character->GetActorLocation() - buttonMesh->GetComponentLocation()).GetSafeNormal();
	//const float _angle = UCustomMathBlueprintLibrary::Angle(_positionForward, _locationDiff);

	//if (_angle > (interactableAngle / 2.0f))
	//{
	//	LOG_COLOR_TIME(STOP_HANDCAR_DEBUG, "Interaction failed: angle condition not met", FColor::Red, 3.0f);
	//	return;
	//}

	//player = _character;
	//player->DisablePlayerContext(player->GetCurrentPlayerContext());
	//player->GetInteractionComponent()->SetHasInteracted(true);

	//movePlayer = true;
	//rotatePlayer = true;

	Super::Interacted(_character);
}

void AStopHandcarActor::LaunchAnimation(AMainCharacter* _character)
{
	_character->SetCanMove(false);
	_character->GetCharaAnim()->SetHasInteract(true);
	//_character->GetCharaAnim()->SetAnimWeight(1, false);
	_character->GetCharaAnim()->ClearAllAnimWeight(false);
}

bool AStopHandcarActor::CanInteract(AMainCharacter* _character)
{
	return Super::CanInteract(_character) && manager && !calledHandcar/* && !player*/;
}

void AStopHandcarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStopHandcarActor, calledHandcar);
}

void AStopHandcarActor::Interacted_Internal()
{
	CallHandcar();
	Super::Interacted_Internal();
}

void AStopHandcarActor::OnRep_UpdateCalledHandcar(const bool _called)
{
	calledHandcar = _called;
}
