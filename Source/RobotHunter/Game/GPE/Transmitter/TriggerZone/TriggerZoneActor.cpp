// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerZoneActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>

#include "RobotHunter/Utils/FileLogger.h"

void ATriggerZoneActor::BeginPlay()
{
	Super::BeginPlay();

	CHECK_NULL(trigger, "trigger is null!!");

	if(typeActivation == ETriggerTypeActivation::ENTER_ZONE || typeActivation == ETriggerTypeActivation::BOTH)
		trigger->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZoneActor::OnEnterTrigger);
	if (typeActivation == ETriggerTypeActivation::EXIT_ZONE || typeActivation == ETriggerTypeActivation::BOTH)
		trigger->OnComponentEndOverlap.AddDynamic(this, &ATriggerZoneActor::OnExitTrigger);
}

#if WITH_EDITOR
void ATriggerZoneActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATriggerZoneActor, shapeType))
	{
		if (trigger)
		trigger->DestroyComponent();

		switch (shapeType)
		{
			case EShapeType::BOX:
				trigger = NewObject<UShapeComponent>(this, UBoxComponent::StaticClass(), FName("Trigger Box"));
				break;
			case EShapeType::SPHERE:
				trigger = NewObject<UShapeComponent>(this, USphereComponent::StaticClass(), FName("Trigger Sphere"));
				break;
			case EShapeType::CAPSULE:
				trigger = NewObject<UShapeComponent>(this, UCapsuleComponent::StaticClass(), FName("Trigger Capsule"));
				break;
		}

		trigger->SetupAttachment(GetRootComponent());
		AddInstanceComponent(trigger);
		AddOwnedComponent(trigger);
		trigger->RegisterComponent();
		trigger->SetCollisionProfileName(FName("OverlapOnlyPlayer"), true);

		Modify();
	}
}
#endif

void ATriggerZoneActor::OnEnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Activate();
}

void ATriggerZoneActor::OnExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Activate();
}
