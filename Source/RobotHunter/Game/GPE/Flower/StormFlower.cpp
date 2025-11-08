#include "StormFlower.h"
#include<RobotHunter/Game/GPE/Weathers/Storm/DryStorm/DryStormWeather.h>
#include<RobotHunter/Game/GPE/Weathers/Storm/WetStorm/WetStormWeather.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "PowerLine.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Net/UnrealNetwork.h"

#define DEBUG_STORM 0

AStormFlower::AStormFlower()
{
	areaEffect = CreateDefaultSubobject<USphereComponent>("Sphere Detect");
	areaEffect->SetupAttachment(RootComponent);
}



void AStormFlower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RemoveFlowerLink();
}

void AStormFlower::Init()
{
	Super::Init();

	if(areaEffect) areaEffect->bHiddenInGame = !useDebug;
	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}




	oldDamage = damage;
	oldSphereRadius = Cast<USphereComponent>(areaEffect)->GetScaledSphereRadius();
	sphereRadius = oldSphereRadius;
	if (desactivateModifier) {
		ServerDesactivateModifier();
		return;
	}

	if (IsItThisWeather(EMeteoStatus::STORM) || IsItThisWeather(EMeteoStatus::DRYSTORM))
	{
		ServerActivateModifier();
	}
	else
	{
		ServerDesactivateModifier();
	}
}
void AStormFlower::DrawDebug()
{
	DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0, 0, 200), 50, 32, isActive ? FColor::Green : FColor::Red, false);
}

void AStormFlower::SpawnPowerLine()
{
	for (size_t i = 0; i < flowerLink.Num(); i++)
	{
		FVector _flowerLinkLocation = flowerLink[i]->GetActorLocation();
		APowerLine* SpawnedActor = GetWorld()->SpawnActor<APowerLine>(spineLine, InitSpawnLocation(GetActorLocation(), _flowerLinkLocation),
			InitSpawnRotation(GetActorLocation(), _flowerLinkLocation));
		SpawnedActor->ConnectFlower(this, flowerLink[i]);
		SpawnedActor->InitLonger(GetActorLocation(), _flowerLinkLocation);
	}

}

void AStormFlower::RemoveFlowerLink()
{
	if (!isActive)
	{
		for (size_t i = 0; i < flowerLink.Num(); i++)
		{
			flowerLink[i]->flowerLink.Remove(this);
		}
		flowerLink.Empty();
	}
}

void AStormFlower::ActiveFlowerPhase()
{
	if (isCooldownFlowerPhase)
		return;
	
	GetWorld()->GetTimerManager().SetTimer(timerFlowerPhase, this, &AStormFlower::CooldownFlowerPhase, effectDuration, false);
	isActive = true;
	InitZoneTimer(actorWithEffect);
}


void AStormFlower::CooldownFlowerPhase()
{
	ClearTimer();
	FTimerHandle _TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AStormFlower::EndCooldownFlower, cooldownFlowerAlim, false);
	
	isActive = false;
	isCooldownFlowerPhase = true;
}
void AStormFlower::EndCooldownFlower()
{

	isCooldownFlowerPhase = false;
}


void AStormFlower::ZoneEffect(TArray<AActor*> _actorWithEffect)
{
	Super::ZoneEffect(_actorWithEffect);

	if (!HasAuthority())
		return;

	TArray<FHitResult> _result;
	TArray<AActor*> actorIgnore = { this };
	//Recup sphere not shape
	USphereComponent* _sphereComponent = Cast<USphereComponent>(areaEffect);
	if (!_sphereComponent)
		return;

	bool hit = UKismetSystemLibrary::SphereTraceMultiForObjects(
									 GetWorld(), GetActorLocation(),
									 GetActorLocation(), _sphereComponent->GetScaledSphereRadius(), 
									 layerFlower, true, actorIgnore, EDrawDebugTrace::ForOneFrame, _result, false);

	if (!hit || !isActive)
		return;

	for (size_t i = 0; i < _result.Num(); i++)
	{
		AStormFlower* _stormFlower = Cast<AStormFlower>(_result[i].GetActor());
		
		if (_stormFlower)
			CheckFlower(_stormFlower);
		
		
	}
}


FRotator AStormFlower::InitSpawnRotation(const FVector& _from, const FVector& _to)
{
	directionTwoFlower = UKismetMathLibrary::GetDirectionUnitVector(_from, _to);
	return FRotator(directionTwoFlower.Rotation().Pitch, directionTwoFlower.Rotation().Yaw, directionTwoFlower.Rotation().Roll);
}
FVector AStormFlower::InitSpawnLocation(const FVector& _a, const FVector& _b)
{
	return UKismetMathLibrary::VLerp(_a, _b, 0.5);
}


void AStormFlower::CheckFlower(AStormFlower* _flower)
{
	
	if (!AsFlowerAlreadyLink(_flower) && !_flower->AsFlowerAlreadyLink(this))
		CheckLink(_flower);

}
void AStormFlower::CheckLink(AStormFlower* _flower)
{
	_flower->ActiveFlowerPhase();
	_flower->GetFlowerLink().Add(_flower);
	flowerLink.Add(_flower);
	ServerRPC_SpawnPowerLine();
}
bool AStormFlower::AsFlowerAlreadyLink(AStormFlower* _flower)
{
	for (size_t j = 0; j < flowerLink.Num(); j++)
		if (flowerLink[j] == _flower)
			return true;

	return false;
}
void AStormFlower::ServerRPC_SpawnPowerLine_Implementation()
{
	SpawnPowerLine();
}
bool AStormFlower::ServerRPC_SpawnPowerLine_Validate()
{
	LOG_COLOR(DEBUG_STORM, "ServerRPC_SpawnPowerLine_Validate", FLinearColor::Black);
	return true;

}
void AStormFlower::On_RepFlowerLink()
{
	LOG_COLOR(DEBUG_STORM, "AStormFlower::On_RepFlowerLink", FLinearColor::Black);
}
void AStormFlower::OnRep_SphereRadius()
{
	Cast<USphereComponent>(areaEffect)->SetSphereRadius(sphereRadius);
}
void AStormFlower::ActivateModifier()
{
	sphereRadius *= modifierRadius;

	Cast<USphereComponent>(areaEffect)->SetSphereRadius(sphereRadius);
	damage *= modifierDamage;
}
void AStormFlower::DesactivateModifier()
{
	sphereRadius = oldSphereRadius;
	Cast<USphereComponent>(areaEffect)->SetSphereRadius(sphereRadius);
	damage = oldDamage;
}



void AStormFlower::ServerActivateModifier_Implementation()
{
	ActivateModifier();
	OnRep_SphereRadius();
}
bool AStormFlower::ServerActivateModifier_Validate()
{
	return true;
}
void AStormFlower::ServerDesactivateModifier_Implementation()
{
	DesactivateModifier();
	OnRep_SphereRadius();
}
bool AStormFlower::ServerDesactivateModifier_Validate()
{
	return true;
}



void AStormFlower::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStormFlower, sphereRadius);
	DOREPLIFETIME(AStormFlower, flowerLink);
}