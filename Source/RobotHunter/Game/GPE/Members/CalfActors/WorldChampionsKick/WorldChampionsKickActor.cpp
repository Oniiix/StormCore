// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldChampionsKickActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"


void AWorldChampionsKickActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (!mainCharacter) return;
	//if (useDebug)
	//	DrawDebug();
}


void AWorldChampionsKickActor::PressBehavior()
{
	Super::PressBehavior();
	mainCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	mainCharacter->SetHasStartWCK(true);
	if (HasAuthority())
		ClientRpc_CallAnimation();
	else
		ServerRpc_CallAnimation();


	TArray<AActor*> _actors;
	if (UKismetSystemLibrary::SphereOverlapActors(this, mainCharacter->GetActorLocation(), kickRange, kickLayers, AActor::StaticClass(), {mainCharacter}, _actors))
	{
		float _angle = 361;
		const int _size = _actors.Num();
		for (int i = 0; i < _size; i++)
		{
			_angle = UCustomMathBlueprintLibrary::Angle(mainCharacter->GetMesh()->GetForwardVector(), (_actors[i]->GetActorLocation() - mainCharacter->GetActorLocation()).GetSafeNormal());
			if (_angle > kickAngle / 2) continue;

			if (UMovableComponent* _comp = _actors[i]->GetComponentByClass<UMovableComponent>())
			{
				if (_comp->IsAffectBy(EMovementSource::WCK))
				{
					FVector _dir = (_actors[i]->GetActorLocation() - mainCharacter->GetActorLocation()).GetSafeNormal();
					_dir.Z = 0;
					FVector _launchDir = UCustomMathBlueprintLibrary::GetLocalTrigoPointXZ(ejectionAngle, 1, _dir, FVector::UpVector);
					_comp->Launch(_launchDir * ejectionForce * 100);
				}
			}

			if (ULifeComponent* _life = _actors[i]->GetComponentByClass<ULifeComponent>())
			{
				_life->Damage(damages * bonusDamage, mainCharacter);
				AddBleed(_actors[i]);
			}
		}
	}
	StartCooldown();
}

bool AWorldChampionsKickActor::CanUse()
{
	return Super::CanUse() && !mainCharacter->GetMovementComponent()->IsFalling();
}

void AWorldChampionsKickActor::ResetMember()
{
	mainCharacter->SetHasStartWCK(false);
	if (isUsing)
		mainCharacter->ReactivateMembers({ this });
	Super::ResetMember();
}

void AWorldChampionsKickActor::InitMember()
{
	Super::InitMember();
	mainCharacter->OnUseCalf().AddUniqueDynamic(this, &AWorldChampionsKickActor::FinishWCK);
}

void AWorldChampionsKickActor::SetupMember()
{
	Super::SetupMember();

}

bool AWorldChampionsKickActor::ClientRpc_CallAnimation_Validate()
{
	return !(HasAuthority());
}

void AWorldChampionsKickActor::ClientRpc_CallAnimation_Implementation()
{
	mainCharacter->SetHasStartWCK(true);
}

void AWorldChampionsKickActor::ServerRpc_CallAnimation_Implementation()
{
	mainCharacter->SetHasStartWCK(true);
}





#if WITH_EDITOR
void AWorldChampionsKickActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
	Super::PostEditChangeProperty(PropertyChangeEvent);
	if (PropertyChangeEvent.GetPropertyName() == "useRadians")
		ejectionAngle = useRadians ? FMath::DegreesToRadians(ejectionAngle) : FMath::RadiansToDegrees(ejectionAngle);
}
#endif

void AWorldChampionsKickActor::FinishWCK()
{
	isUsing = false;
	mainCharacter->SetHasStartWCK(false);
	mainCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	mainCharacter->ReactivateMembers({ this });
	mainCharacter->GetInteractionComponent()->SetCanInteract(true);
	mainCharacter->SetCanMove(true);
}

void AWorldChampionsKickActor::DrawDebug()
{
	//DRAW_CIRCLE(mainCharacter->GetActorLocation(), kickRange, FColor::Blue, FVector::RightVector, FVector::ForwardVector);
	//DrawDebugLine(GetWorld(), mainCharacter->GetActorLocation(), mainCharacter->GetActorLocation() + UCustomMathBlueprintLibrary::GetLocalTrigoPointXY(-(kickAngle / 2), kickRange, GetActorForwardVector(), GetActorRightVector()), FColor::Red);
	//DrawDebugLine(GetWorld(), mainCharacter->GetActorLocation(), mainCharacter->GetActorLocation() + UCustomMathBlueprintLibrary::GetLocalTrigoPointXY((kickAngle / 2), kickRange, GetActorForwardVector(), GetActorRightVector()), FColor::Red);
}

void AWorldChampionsKickActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AWorldChampionsKickActor, canKick);
}
