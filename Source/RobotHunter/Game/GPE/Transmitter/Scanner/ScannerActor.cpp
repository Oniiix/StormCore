// Fill out your copyright notice in the Description page of Project Settings.


#include "ScannerActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"

AScannerActor::AScannerActor()
{
	interactPoint = CreateDefaultSubobject<UBillboardComponent>("Point Interact");
	interactPoint->SetupAttachment(mesh);

	sphereInteractRange = CreateDefaultSubobject<USphereComponent>("SphereInteractRange");
	sphereInteractRange->SetupAttachment(RootComponent);

	canBeDetected = true;

	interactionTime = 1.0f;
}

void AScannerActor::BeginPlay()
{
	Super::BeginPlay();
	interactPointPosition = FVector2D(interactPoint->GetComponentLocation());
}

void AScannerActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (useDebug)
		DrawDebug();

	RotatePlayerToPoint();
	MovePlayerToPoint();
}

void AScannerActor::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character); 
	
	const FVector _locationDiff = (_character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const float _angle = UCustomMathBlueprintLibrary::Angle(GetActorForwardVector(), _locationDiff);

	if (_angle > (interactableAngle / 2.0f))
		return;

	player = _character;
	player->DisablePlayerContext(player->GetCurrentPlayerContext());

	//if (player->GetActorLocation().Equals(interactPoint->GetComponentLocation(), 20))
	//{
	//	RotatePlayerToPoint();
	//	return;
	//}

	movePlayer = true;
	rotatePlayer = true;
}

void AScannerActor::MovePlayerToPoint()
{
	if (movePlayer && player)
	{
		const FVector _meshForward = player->GetMesh()->GetForwardVector() * 0.5f;
		player->AddMovementInput(_meshForward);

		const FVector _playerLoc = player->GetActorLocation();

		const float _distance = FVector2D::Distance(FVector2D(interactPointPosition), FVector2D(_playerLoc));

		if (_distance <= 50.0f)
			movePlayer = false;
	}

	/*player->AddMovementInput(player->GetMesh()->GetForwardVector());
	FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), FVector(interactPointPosition.X, interactPointPosition.Y, player->GetActorLocation().Z));
	_rotation.Yaw = _rotation.Yaw - player->GetActorRotation().Yaw;
	player->SetMeshRotation(_rotation, false);
	if (FVector2D(player->GetActorLocation()).Equals(FVector2D(interactPointPosition), 15))
	{
		hasInteracted = false;
		RotatePlayerToPoint();
	}*/
}

void AScannerActor::RotatePlayerToPoint()
{
	if (rotatePlayer && player)
	{
		const FVector _actorLoc = GetActorLocation();
		const FVector _playerLoc = player->GetActorLocation();

		const FRotator& _rotation = movePlayer ? UKismetMathLibrary::FindLookAtRotation(_playerLoc, FVector(interactPointPosition.X, interactPointPosition.Y, _playerLoc.Z))
								: UKismetMathLibrary::FindLookAtRotation(_playerLoc, FVector(_actorLoc.X, _actorLoc.Y, _playerLoc.Z));

		player->SetMeshRotation(_rotation, !movePlayer);

		if (!movePlayer)
		{
			rotatePlayer = false;
			LaunchScannerAnimation();
		}
	}

	/*player->GetCharacterMovement()->Velocity = FVector(0);
	player->OnRotationEnd().AddUniqueDynamic(this, &AScannerActor::LaunchAnimation);

	FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, player->GetActorLocation().Z));
	_rotation.Yaw = _rotation.Yaw - player->GetActorRotation().Yaw;
	player->SetMeshRotation(_rotation, true);*/
}

void AScannerActor::LaunchScannerAnimation()
{
	//TODO ScannerActor : call animation and active the transmitter au momment du notify (<- juste call animation normalement)
	FTimerHandle _interactionTimer;
	GetWorld()->GetTimerManager().SetTimer(_interactionTimer, this, &AScannerActor::UseScanner, interactionTime, false);
}

void AScannerActor::UseScanner()
{
	Activate();
	if (!player)
		return;
	player->OnRotationEnd().RemoveDynamic(this, &AScannerActor::LaunchScannerAnimation);
	player->EnablePlayerContext(player->GetInputConfig()->GetGameContextInput());
	player = nullptr;
}

void AScannerActor::DrawDebug()
{
	DrawDebugSphere(GetWorld(), interactPoint->GetComponentLocation(), 10, 20, FColor::Purple);

	const FVector& _actorLocation = GetActorLocation();
	const FVector& _actorFwd = GetActorForwardVector();
	const FVector& _actorRight = GetActorRightVector();

	DrawDebugLine(GetWorld(), _actorLocation, _actorLocation + UCustomMathBlueprintLibrary::GetLocalTrigoPointXY(interactableAngle / 2, sphereInteractRange->GetUnscaledSphereRadius(), _actorFwd, _actorRight), FColor::Blue);
	DrawDebugLine(GetWorld(), _actorLocation, _actorLocation + UCustomMathBlueprintLibrary::GetLocalTrigoPointXY(-(interactableAngle / 2), sphereInteractRange->GetUnscaledSphereRadius(), _actorFwd, _actorRight), FColor::Blue);
}
