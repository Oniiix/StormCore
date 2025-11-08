


#include "Map3D.h"

#include "Engine/StaticMeshSocket.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

// Sets default values
AMap3D::AMap3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Crée le Static Mesh Component et l'attache en tant que RootComponent
	mapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mainMesh"));
	RootComponent = mapMesh;

}

// Called when the game starts or when spawned
void AMap3D::BeginPlay()
{
	Super::BeginPlay();
	
	InitMeshSockets();

}



// Called every frame
void AMap3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	
}

void AMap3D::InitMeshSockets()
{
	if (mapMesh && mapMesh->GetStaticMesh())
	{
		const UStaticMesh* _staticMesh = mapMesh->GetStaticMesh();
		for (const UStaticMeshSocket* _socket : _staticMesh->Sockets)
		{
			if (_socket)
			{
				travelPoint.Add(_socket->SocketName, _socket->RelativeLocation);
			}
		}
	}
	TArray<FName> _socketKeys;
	travelPoint.GetKeys(_socketKeys);
	for (const FName& _socketName : _socketKeys)
	{
		LOG_COLOR(true, TEXT("Socket trouvé : ") + _socketName.ToString(), FColor::Green);
	}
}

void AMap3D::InitActorMarkers()
{
	const int num = actorMarker.Num();
	for (int i = 0; i < num; i++)
	{
		if (ACustomHandcarActor* _handcar = Cast<ACustomHandcarActor>(actorMarker[i]))
		{
			handcar = _handcar;
			break;
		}
	}
}
