#include "ChangeWayRailActor.h"
#include "../../Game/Interactable/Wagons/WagonActor.h"

AChangeWayRailActor::AChangeWayRailActor()
{
	trigger = CreateDefaultSubobject<UBoxComponent>("TriggerZone");
	trigger->SetupAttachment(RootComponent);
}

void AChangeWayRailActor::OnEnterTrigger(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, 
	bool _bFromSweep, const FHitResult& _SweepResult)
{
	AWagonActor* _wagon = Cast<AWagonActor>(_OtherActor);

	if (_wagon)
		_wagon->ChangeWay();
}

void AChangeWayRailActor::BeginPlay()
{
	Super::BeginPlay();

	if (trigger)
		trigger->OnComponentBeginOverlap.AddDynamic(this, &AChangeWayRailActor::OnEnterTrigger);
}

void AChangeWayRailActor::DrawDebug()
{
	FColor _debugColor = FColor::Yellow;
	FVector _sphereLoc = FVector();

	_sphereLoc = GetLocationAtPointIndex(GetStopIndex(true));
	DrawDebugSphere(GetWorld(), _sphereLoc, 10.0f, 10, _debugColor);

	_debugColor = FColor::Cyan;
	_sphereLoc = GetLocationAtPointIndex(GetStopIndex(false));
	DrawDebugSphere(GetWorld(), _sphereLoc, 10.0f, 10, _debugColor);
}
