// Fill out your copyright notice in the Description page of Project Settings.


#include "MemberGiver.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/GPE/Members/CalfActors/CalfActor.h"
#include "RobotHunter/Game/GPE/Members/LeftArmActors/LeftArmActor.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/RightArmActor.h"
#include "RobotHunter/Game/GPE/Members/ThighActors/ThighActors.h"

AMemberGiver::AMemberGiver() : AInteractableActor()
{
	widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	widget->SetupAttachment(RootComponent);
	sphereTrigger = CreateDefaultSubobject<USphereComponent>("SphereTrigger");
	sphereTrigger->SetupAttachment(RootComponent);
}

void AMemberGiver::Interacted(AMainCharacter* _character)
{
	Super::Interacted(_character);

	if (!playerController || !memberRef)
		return;
	if (ARightArmActor::StaticClass()->GetName() == memberRef->GetSuperClass()->GetSuperClass()->GetName())
		playerController->SetRightArm(memberRef);
	else if (ALeftArmActor::StaticClass()->GetName() == memberRef->GetSuperClass()->GetSuperClass()->GetName())
		playerController->SetLeftArm(memberRef);
	else if (ACalfActor::StaticClass()->GetName() == memberRef->GetSuperClass()->GetSuperClass()->GetName())
		playerController->SetCalf(memberRef);
	else if (AThighActor::StaticClass()->GetName() == memberRef->GetSuperClass()->GetSuperClass()->GetName())
		playerController->SetThigh(memberRef);
}

void AMemberGiver::LaunchAnimation(AMainCharacter* _character)
{
	Super::LaunchAnimation(_character);

}

void AMemberGiver::BeginPlay()
{
	Super::BeginPlay();
	sphereTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMemberGiver::OnEnterMemberGiverZone);
	sphereTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &AMemberGiver::OnExitMemberGiverZone);
	playerController = GetWorld()->GetFirstPlayerController<ACustomPlayerController>();
}

void AMemberGiver::SetInSight(AMainCharacter* _character)
{
	Super::SetInSight(_character);
}

void AMemberGiver::SetNotInSight()
{
	Super::SetNotInSight();
}

void AMemberGiver::OnEnterMemberGiverZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	widget->SetVisibility(true);
}

void AMemberGiver::OnExitMemberGiverZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	widget->SetVisibility(false);
}
