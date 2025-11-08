// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterLaunchSettings.h"
#include "GameFramework/Character.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterLaunchSettings::Init(AActor* _owner)
{
	owner = Cast<ACharacter>(_owner);
}

void UCharacterLaunchSettings::Launch(const FVector& _dir)
{
	if (!owner)
		return;
	owner->GetCharacterMovement()->Velocity = FVector(0.0f);
	owner->GetCharacterMovement()->PendingLaunchVelocity = FVector(0.0f);
	owner->LaunchCharacter(_dir, true, true);
	LOG_COLOR(true, "Launch " + owner->GetName(), BLUE);
}
