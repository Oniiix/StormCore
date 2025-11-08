// Fill out your copyright notice in the Description page of Project Settings.


#include "CorosiveLake.h"

ACorosiveLake::ACorosiveLake()
{
	acidMesh = CreateDefaultSubobject<UStaticMeshComponent>("AcidMesh");
	acidMesh->SetupAttachment(RootComponent);
}

void ACorosiveLake::InitReceiver()
{
	Super::InitReceiver();
	initScale = acidMesh->GetRelativeScale3D();
	maxScale = initScale + FVector(0, 0, maxHeight);
}

void ACorosiveLake::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (hasFinishAction)
		return;

	isReceiverActive ? GrowAcid() : ReduceAcid();
}

void ACorosiveLake::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AMainCharacter* _mainChara = Cast<AMainCharacter>(OtherActor);

	CHECK_NULL(_mainChara, "Cast to MainCharacter failed !");

	player = _mainChara;
	if (HasAuthority())
		GetWorldTimerManager().SetTimer(timerDamage, this, &ACorosiveLake::DamagePlayer, damageTime, true);
}

void ACorosiveLake::NotifyActorEndOverlap(AActor* _otherActor)
{
	Super::NotifyActorEndOverlap(_otherActor);
	AMainCharacter* _mainChara = Cast<AMainCharacter>(_otherActor);

	CHECK_NULL(_mainChara, "Cast to MainCharacter failed !");

	player = nullptr;
	GetWorldTimerManager().ClearTimer(timerDamage);
}

void ACorosiveLake::Action()
{
	Super::Action();
	hasFinishAction = false;
}

void ACorosiveLake::GrowAcid()
{
	const FVector _newScale = FMath::VInterpConstantTo(acidMesh->GetRelativeScale3D(), maxScale, DELTA_TIME, growSpeed);
	acidMesh->SetRelativeScale3D(_newScale);
	if (acidMesh->GetRelativeScale3D().Equals(maxScale))
		hasFinishAction = true;
}

void ACorosiveLake::ReduceAcid()
{
	const FVector _newScale = FMath::VInterpConstantTo(acidMesh->GetRelativeScale3D(), initScale, DELTA_TIME, reduceSpeed);
	acidMesh->SetRelativeScale3D(_newScale);
	if (acidMesh->GetRelativeScale3D().Equals(initScale))
		hasFinishAction = true;
}

void ACorosiveLake::DamagePlayer()
{
	player->GetLifeComponent()->Damage(damage, this);
	LOG_COLOR(true, "Corrosive Lake => Damage", RED);
}


