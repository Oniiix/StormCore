// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerLine.h"
#include <Kismet/KismetMathLibrary.h>
#include "../Flower/StormFlower.h"
#include "../../../Utils/DebugUtils.h"
#include <RobotHunter/CCC/Character/MainCharacter.h>
#include "Net/UnrealNetwork.h"


#define POWER_LINE_USE_LOG false


APowerLine::APowerLine()
{
	PrimaryActorTick.bCanEverTick = true;
	areaEffect = CreateDefaultSubobject<UBoxComponent>("BoxDetectTrigger");
	areaEffect->SetupAttachment(RootComponent);
	bReplicates = true;


}

void APowerLine::BeginPlay()
{
	Super::BeginPlay();
	Init();

	
}
void APowerLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DestroyActor();
}
void APowerLine::DrawDebug()
{
	if (useDebug)
		DrawDebugBox(GetWorld(), GetActorLocation(), areaEffect->GetScaledBoxExtent(), GetActorQuat(), FColor::Blue, false, -1, 0, 2);
}


void APowerLine::Init()
{
	if (!areaEffect)
		return;
	areaEffect->OnComponentBeginOverlap.AddDynamic(this, &APowerLine::OnBoxBeginOverlap);
}

void APowerLine::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* _character = Cast<AMainCharacter>(OtherActor))
		_character->LaunchCharacter(-_character->GetMesh()->GetForwardVector() * LaunchForce + GetActorUpVector() * LaunchUpForce, true, true);
}


void APowerLine::InitLonger(FVector _from, FVector _to)
{
	distanceTwoFlower = FVector::Distance(_from, _to);
	areaEffect->SetBoxExtent(FVector(distanceTwoFlower / 2, 32, 32));
}
void APowerLine::ConnectFlower(AStormFlower* _a, AStormFlower* _b)
{
	flowerConnect.Add(_a);
	flowerConnect.Add(_b);
}
void APowerLine::DestroyActor()
{
	if (flowerConnect.IsEmpty())
		return;
	if (flowerConnect[0]->IsActive() && flowerConnect[1]->IsActive())
		return;
	Destroy();

}

void APowerLine::Debug()
{

}

void APowerLine::OnRep_DistanceTwoFlower()
{
	areaEffect->SetBoxExtent(FVector(distanceTwoFlower / 2, 32, 32));
}

void APowerLine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerLine, distanceTwoFlower);
}