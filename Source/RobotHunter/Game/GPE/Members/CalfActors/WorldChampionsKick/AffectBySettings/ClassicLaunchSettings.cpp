// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicLaunchSettings.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/Utils/ENUM_AI.h"

void UClassicLaunchSettings::Init(AActor* _owner)
{
	Super::Init(_owner);
	owner = _owner;
	TryGetMesh();
	if (currentMesh)
	{
		physicsStateBeforeAffect = currentMesh->BodyInstance.bSimulatePhysics;
		currentMesh->SetNotifyRigidBodyCollision(true);
		ActivateHitEvent();
	}
}

void UClassicLaunchSettings::Launch(const FVector& _dir)
{
	if ((!currentMesh && !TryGetMesh()) || !isOnFloor)
		return;
	currentMesh->SetCanEverAffectNavigation(false);
	physicsStateBeforeAffect = currentMesh->BodyInstance.bSimulatePhysics;
	currentMesh->BodyInstance.SetInstanceSimulatePhysics(true, false, true);

	
	currentMesh->BodyInstance.AddImpulse(_dir, true);
	isOnFloor = false;
	
	FTimerHandle _activateHitEventTimer;
	owner->WORLD->GetTimerManager().SetTimer(_activateHitEventTimer, this, &UClassicLaunchSettings::ActivateHitEvent, 0.5f);

}

void UClassicLaunchSettings::Touch(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FBox& _ownerBox = currentMesh->Bounds.GetBox();
	if (Hit.ImpactPoint.Z < _ownerBox.Min.Z)
	{
		//LOG_COLOR_TIME(true, "MIN => " + _ownerBox.Min.ToString(), BLUE, 60);
		//LOG_COLOR_TIME(true, "MAX => " + _ownerBox.Max.ToString(), BLUE, 60);
		const FVector& _extent = _ownerBox.GetExtent();
		FVector _location = currentMesh->GetComponentLocation() + FVector::DownVector * _extent;
		FHitResult _result;

		DrawDebugBox(owner->GetWorld(), _ownerBox.GetCenter(), _ownerBox.GetExtent(), FQuat::Identity, FColor::Yellow, false, -1, 0, 3);

		if (UKismetSystemLibrary::BoxTraceSingleForObjects(this, _location, _location, FVector(_extent.X * 0.99, _extent.Y * 0.99, 1), FRotator::ZeroRotator, {EObjectTypeQuery::ObjectTypeQuery7, EObjectTypeQuery::ObjectTypeQuery1}, false, {owner}, EDrawDebugTrace::ForDuration, _result, false))
		{
			currentMesh->SetCanEverAffectNavigation(true);
			currentMesh->BodyInstance.SetInstanceSimulatePhysics(physicsStateBeforeAffect);
			currentMesh->OnComponentHit.RemoveDynamic(this, &UClassicLaunchSettings::Touch);
			isOnFloor = true;

			//TODO ClassicAffectWCKSettings : check if we can upgrade this
		}
	}
}

void UClassicLaunchSettings::ActivateHitEvent()
{
	currentMesh->OnComponentHit.AddUniqueDynamic(this, &UClassicLaunchSettings::Touch);
}

TObjectPtr<UStaticMeshComponent> UClassicLaunchSettings::TryGetMesh()
{
	if (owner->GetRootComponent()->IsA<UStaticMeshComponent>())
		currentMesh = Cast<UStaticMeshComponent>(owner->GetRootComponent());
	else
		currentMesh = owner->GetComponentByClass<UStaticMeshComponent>();
	return currentMesh;
}
