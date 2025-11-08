// Fill out your copyright notice in the Description page of Project Settings.


#include "TongueProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"

ATongueProjectile::ATongueProjectile()
{
	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	triggerBox->SetupAttachment(projectileCollision);
}

void ATongueProjectile::InitProjectile(const FVector& _targetLocation)
{
	Super::InitProjectile(_targetLocation);
	tongueStat = Cast<UTongueProjectileStat>(stats);
	float _lenghtTongue = tongueStat->GetLengthTongue();
	maxScale = GetActorScale() + FVector(_lenghtTongue / 100, 0, 0);
	newLocation = GetActorLocation() + (GetActorForwardVector() * _lenghtTongue);
	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATongueProjectile::TriggerCollision);
	canMove = false;
	speed = tongueStat->GetSpeed();
}

void ATongueProjectile::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (newScale.X >= maxScale.X && !returnMode)
		return;
	if (newScale.X <= 0.1 && returnMode)
		Destroy();
	SizeTongue();
}

#pragma region SizeActor
void ATongueProjectile::SizeTongue()
{
	initScale = GetActorScale();

	newScale = FMath::VInterpConstantTo(GetActorScale(), maxScale, GetWorld()->DeltaTimeSeconds, speed);
	SetActorScale3D(FVector(newScale.X, initScale.Y, initScale.Z));

	const FVector& _newLocation = GetActorLocation() + (GetActorForwardVector() * ((newScale.X / 2) - (initScale.X / 2)) * 100);
	SetActorLocation(_newLocation);
	if (newScale.X >= maxScale.X &&!returnMode)
		GetWorldTimerManager().SetTimer(timerReturn, this, &ATongueProjectile::ReturnMode, tongueStat->GetDelayWait());
}

void ATongueProjectile::ReturnMode()
{
	returnMode = true;
	maxScale = FVector::Zero();
	speed = tongueStat->GetSpeedTongueBack();
}
#pragma endregion

void ATongueProjectile::TriggerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* _player = Cast<AMainCharacter>(OtherActor);
	if (!_player)
	{
		newScale = maxScale;
		LOG(TONGUE_PROJECTILE_USE_LOG, OtherActor->GetName());
		return;
	}
	player = _player;

	FVector _direction;
	if (!bFromSweep)
	{
		FHitResult _result;
		bool _hit = UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), newScale, GetActorRotation(), outLayer, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, _result, true);
		if (_hit)
			_direction = player->GetActorLocation() - _result.Location;
	}
	else
		_direction = player->GetActorLocation() - SweepResult.Location;
	const float& _forcePush = tongueStat->GetForcePush();
	const FVector& _punch = -(_direction * _forcePush) + (player->GetActorUpVector() * _forcePush);
	player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	player->LaunchCharacter(_punch , true, true);
}

