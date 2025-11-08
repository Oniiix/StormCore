#include "CustomActor.h"

ACustomActor::ACustomActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//RootComponent = mesh;
#if WITH_EDITOR 
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif
}

void ACustomActor::CallSpawnEvent(ACustomActor* _actor)
{
	onSpawnActor.Broadcast(_actor);
}

void ACustomActor::BeginPlay()
{
	Super::BeginPlay();
	isRuntime = true;
}

void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (useDebug)
		DrawDebug();
}

bool ACustomActor::ShouldTickIfViewportsOnly() const
{
	return shouldTickIfViewportsOnly;
}

void ACustomActor::DrawDebug() {}

