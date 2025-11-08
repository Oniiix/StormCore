#include "LifeStockHandcarActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ALifeStockHandcarActor::ALifeStockHandcarActor()
{
	lifeExchangeValue = 10.0f;
	lifeExchangeRate = 1.0f;

	isAtExterior = false;

	lifeComponent = CreateDefaultSubobject<ULifeComponent>("Life");
	AddOwnedComponent(lifeComponent);


	lifeStockWidget = CreateDefaultSubobject<ULifebarWidgetComponent>("LifeStockWidget");
	lifeStockWidget->SetupAttachment(RootComponent);

	lifePlayer = nullptr;

	lifeExchangeTimer = FTimerHandle();

}

void ALifeStockHandcarActor::StopInteraction(const bool _atStop, const bool _inAutoMode)
{
	if (_atStop)
		return;

	if (lifePlayer)
	{
		lifePlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		lifePlayer->SetCanMove(true);
		//lifePlayer->SetCanTurn(true);
		lifePlayer->GetInteractionComponent()->SetIsInInteraction(false);
		lifePlayer->GetInteractionComponent()->SetHasInteracted(false);
	}

	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(lifeExchangeTimer))
		_tm.ClearTimer(lifeExchangeTimer);

	lifePlayer = nullptr;
}

void ALifeStockHandcarActor::BeginPlay()
{
	Super::BeginPlay();
	/*lifeStockWidget->InitLifebar(lifeComponent);
	lifeComponent->InitLife();*/
}

void ALifeStockHandcarActor::TransferLife()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (lifePlayer)
	{

		if (_tm.IsTimerActive(lifeExchangeTimer))
			_tm.ClearTimer(lifeExchangeTimer);

		ULifeComponent* _lifeComp = lifePlayer->GetLifeComponent();

		if (_lifeComp)
		{
			const float _playerMaxLife = _lifeComp->GetMaxLife();
			const float _playerCurrentLife = _lifeComp->GetCurrentLife();
			const float _playerMissingLife = _playerMaxLife - _playerCurrentLife;

			const float _lifeExchanged = _playerMissingLife < lifeExchangeValue ? _playerMissingLife : lifeExchangeValue;

			//lifeComponent->Damage(_lifeExchanged, nullptr);
			_lifeComp->RestoreLife(_lifeExchanged);

			if ((_playerCurrentLife + _lifeExchanged) < _playerMaxLife)
			{
				_tm.SetTimer(lifeExchangeTimer, this, &ALifeStockHandcarActor::TransferLife, lifeExchangeRate, false);
				return;
			}
		}

		FTimerHandle _stopTimer;
		FTimerDelegate _stopDelegate;
		_stopDelegate.BindLambda([&]() { StopInteraction(); });
		GetWorld()->GetTimerManager().SetTimer(_stopTimer, _stopDelegate, 1.80f, FTimerManagerTimerParameters());
		onTransferLife.Broadcast(false);
		lifePlayer->GetCharaAnim()->SetIsUsingLifeStock(false);
	}
}

void ALifeStockHandcarActor::Interacted(AMainCharacter* _character)
{
	if (!lifePlayer)
	{
		Super::Interacted(_character);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(lifeExchangeTimer);
		FTimerHandle _stopTimer;
		FTimerDelegate _stopDelegate;
		_stopDelegate.BindLambda([&]() { StopInteraction(); });
		GetWorld()->GetTimerManager().SetTimer(_stopTimer, _stopDelegate, 1.80f, FTimerManagerTimerParameters());
		onTransferLife.Broadcast(false);
		lifePlayer->GetCharaAnim()->SetIsUsingLifeStock(false);
	}
}

void ALifeStockHandcarActor::LaunchAnimation(AMainCharacter* _character)
{
	_character->GetCameraComponent()->DesynchPitch(true);
	_character->GetCameraComponent()->DesynchRoll(true);
	_character->GetCameraComponent()->DesynchYaw(true);
	_character->GetCameraComponent()->GetSpringArm()->AddRelativeRotation(_character->GetActorRotation());
	lifePlayer->GetCapsuleComponent()->SetCapsuleRadius(1, false);
	lifePlayer->GetCharaAnim()->SetIsUsingLifeStock(true);
	lifePlayer->GetInteractionComponent()->SetHasInteracted(true);
	lifePlayer->GetInteractionComponent()->SetIsInInteraction(true);
	_character->SetCanMove(false);
	_character->SetCanTurn(false);
}

bool ALifeStockHandcarActor::CanInteract(AMainCharacter* _character)
{
	if (_character)
	{
		ULifeComponent* _charaLifeComp = _character->GetLifeComponent();

		float _playerMaxLife = 0.0f;
		float _playerCurrentLife = 0.0f;

		if (_charaLifeComp)
		{
			_playerMaxLife = _charaLifeComp->GetMaxLife();
			_playerCurrentLife = _charaLifeComp->GetCurrentLife();
		}

		if (_playerCurrentLife < _playerMaxLife)
		{
			bool _interactionCompSetup = false;
			UInteractionComponent* _charaInteractionComp = _character->GetInteractionComponent();

			if (_charaInteractionComp)
				_interactionCompSetup = !_charaInteractionComp->GetHasInteracted() || _charaInteractionComp->GetClosestActor() == this;

			if (_interactionCompSetup)
			{
				bool _notFalling = false;
				UCustomCharacterMovementComponent* _charaMovementComp = _character->GetCustomMovementComponent();

				if (_charaMovementComp)
					_notFalling = !_charaMovementComp->IsFalling();

				return _notFalling;
			}
		}
	}

	return false;
}

void ALifeStockHandcarActor::Interacted_Internal()
{
	lifePlayer = player;
	GetWorld()->GetTimerManager().SetTimer(lifeExchangeTimer, this, &ALifeStockHandcarActor::TransferLife, lifeExchangeRate, false);
	onTransferLife.Broadcast(true);


	const FVector& _actorLocation = GetActorLocation();
	FVector _newPos = _actorLocation + GetActorForwardVector() * 38.0f + GetActorRightVector() * 20.0f;
	_newPos.Z = lifePlayer->GetActorLocation().Z;
	//DRAW_SPHERE_TIME(_newPos, 5, FColor::Blue, 5.0f);
	lifePlayer->SetActorLocation(_newPos);

	Super::Interacted_Internal();

	const FRotator& _actorRotation = GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f);
	lifePlayer->SetMeshRotation(_actorRotation, false);
}

void ALifeStockHandcarActor::AddLifeInStock(const bool _lifeAdded)
{
	if (lifeComponent)
		lifeComponent->RestoreLife(_lifeAdded);
}

void ALifeStockHandcarActor::UpdateValuesFromDataAsset(UHandcarDataAsset* _da)
{
	if (_da)
	{
		lifeExchangeValue = _da->GetLifeExchangeValue();
		lifeExchangeRate = _da->GetLifeExchangeRate();
	}
}
