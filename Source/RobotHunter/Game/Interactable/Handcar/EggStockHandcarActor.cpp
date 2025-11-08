#include "EggStockHandcarActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"
#include "RobotHunter/Game/GPE/Nest/Egg.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"


AEggStockHandcarActor::AEggStockHandcarActor()
{

}

void AEggStockHandcarActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//DRAW_SPHERE(GetActorLocation() + FRotationMatrix(GetActorRotation()).TransformPosition(offsetTP),25,FColor::Green);
}

bool AEggStockHandcarActor::ShouldTickIfViewportsOnly() const
{
	return false;
}


void AEggStockHandcarActor::Interacted(AMainCharacter* _character)
{
	if (_character && _character->GetInventoryComponent()->GetHasEgg())
		Super::Interacted(_character);
}

void AEggStockHandcarActor::Interacted_Internal()
{
	UInteractionComponent* _interactionComp = player->GetInteractionComponent();
	AEgg* _egg = player->GetInventoryComponent()->GetCurrentEgg();
	_interactionComp->DestroyInteractable(dropEggTime);
	eggPlayer = player;
	eggPlayer->SetCanMove(false);
	eggPlayer->SetActorLocation(GetActorLocation() + FRotationMatrix(GetActorRotation()).TransformPosition(offsetTP), false, nullptr, ETeleportType::TeleportPhysics);
	eggPlayer->SetMeshRotation(FRotator(eggPlayer->GetActorRotation().Pitch, GetActorRotation().Yaw + 180, eggPlayer->GetActorRotation().Roll), false);

	Super::Interacted_Internal();
	FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]() { StopInteraction(); });
	GetWorld()->GetTimerManager().SetTimer(stopInteractionTimer, _delegate, animTime, false);
	onDropEgg.Broadcast();
}

void AEggStockHandcarActor::LaunchAnimation(AMainCharacter* _character)
{
	eggPlayer->GetCharaAnim()->ClearAllAnimWeight(false);
	eggPlayer->GetCharaAnim()->SetIsUsingEggStock(true);
}

bool AEggStockHandcarActor::CanInteract(AMainCharacter* _character)
{
	return _character->GetInventoryComponent()->GetHasEgg() && !_character->GetCustomMovementComponent()->IsFalling();
}

void AEggStockHandcarActor::StopInteraction(const bool _atStop, const bool _inAutoMode)
{
	if (_atStop)
		return;

	if (eggPlayer)
	{
		UInteractionComponent* _interactionComp = eggPlayer->GetInteractionComponent();

		if (_interactionComp)
			_interactionComp->DestroyInteractable(-1.0f);

		eggPlayer->SetCanMove(true);
		eggPlayer->GetCharaAnim()->SetIsUsingEggStock(false);
		_interactionComp->SetHasInteracted(false);
	}

	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(stopInteractionTimer))
		_tm.ClearTimer(stopInteractionTimer);

	if (handcar)
		handcar->RestoreLife(lifeStockRestoration);

	eggPlayer = nullptr;
}
