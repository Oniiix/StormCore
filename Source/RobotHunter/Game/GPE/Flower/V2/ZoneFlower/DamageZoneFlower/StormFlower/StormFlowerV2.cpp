#include "StormFlowerV2.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/Game/GPE/Weathers/Storm/DryStorm/DryStormWeather.h"
#include "RobotHunter/Game/GPE/Weathers/Storm/WetStorm/WetStormWeather.h"
#include "RobotHunter/Utils/DebugUtils.h"

AStormFlowerV2::AStormFlowerV2()
{
	effectArea = CreateDefaultSubobject<USphereComponent>("EffectArea");
	effectArea->SetupAttachment(RootComponent);

#pragma region PowerLines
	powerLineType = nullptr;
	powerLines = TArray<APowerLineV2*>();;
#pragma endregion

#pragma region NearbyFlowers
	flowerLayer = TEnumAsByte<EObjectTypeQuery>();
	nearbyFlowers = TArray<AStormFlowerV2*>();
#pragma endregion

#pragma region Modifiers
	zoneRadiusModifier = 2.0f;
	initialZoneRadius = 0.0f;
	zoneRadius = 0.0f;
#pragma endregion

	deactivatedStatus = FStatusParams(EStatusType::DEACTIVATE, 0.1f, 10.0f);
}

void AStormFlowerV2::FindNearbyFlowers()
{
	USphereComponent* _sphereComp = Cast<USphereComponent>(effectArea);

	if (_sphereComp)
	{
		TArray<FHitResult> _hitResults = TArray<FHitResult>();
		const TArray<AActor*> _actorsToIgnore = { this };
		const FVector _actorLocation = GetActorLocation();

		const bool _hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), _actorLocation, _actorLocation, zoneRadius, { flowerLayer },
			false, _actorsToIgnore, EDrawDebugTrace::None, _hitResults, true);

		if (_hit)
		{
			const int _num = _hitResults.Num();
			AStormFlowerV2* _stormFlower = nullptr;

			for (size_t i = 0; i < _num; i++)
			{
				_stormFlower = Cast<AStormFlowerV2>(_hitResults[i].GetActor());

				if (_stormFlower)
				{
					if (!nearbyFlowers.Contains(_stormFlower))
					{
						nearbyFlowers.Add(_stormFlower);
						LinkFlower(_stormFlower);
					}
				}
			}
		}
	}
}

void AStormFlowerV2::ActivateNearbyFlowers()
{
	if (nearbyFlowers.IsEmpty())
		return;

	const int _num = nearbyFlowers.Num();
	FTimerManager& _tm = GetWorld()->GetTimerManager();
	AStormFlowerV2* _flower = nullptr;
	float _activeTimer = 0.0f;

	for (size_t i = 0; i < _num; i++)
	{
		_flower = nearbyFlowers[i];

		if (_flower)
		{
			_activeTimer = _tm.GetTimerElapsed(_flower->activeTimer);

			if (_activeTimer > 0.1f || _activeTimer == -1.0f)
				_flower->StartActivePhase();
		}
	}
}

void AStormFlowerV2::SpawnPowerLine(AStormFlowerV2* _flowerToLink)
{
	if (!_flowerToLink || !powerLineType)
		return;

	const FVector _actorLoc = GetActorLocation();
	const FVector _flowerLoc = _flowerToLink->GetActorLocation();

	const FVector _spawnLoc = UKismetMathLibrary::VLerp(_actorLoc, _flowerLoc, 0.5f);
	const FRotator _spawnRot = UKismetMathLibrary::GetDirectionUnitVector(_actorLoc, _flowerLoc).Rotation();
	const FTransform _spawnTransform = FTransform(_spawnRot, _spawnLoc);

	APowerLineV2* _powerLine = GetWorld()->SpawnActor<APowerLineV2>(powerLineType, _spawnTransform);
	_powerLine->Init(_actorLoc, _flowerLoc, this, _flowerToLink);

	powerLines.Add(_powerLine);
	_flowerToLink->powerLines.Add(_powerLine);
}

void AStormFlowerV2::ActivatePowerLines()
{
	if (powerLines.IsEmpty())
		return;

	const int _num = powerLines.Num();
	APowerLineV2* _powerLine = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_powerLine = powerLines[i];

		if (_powerLine)
			_powerLine->ActivateLightning();
	}
}

void AStormFlowerV2::LinkFlower(AStormFlowerV2* _flowerToLink)
{
	if (IsFlowerLinked(_flowerToLink) == -1)
		ServerRPC_SpawnPowerLine(_flowerToLink);
}

void AStormFlowerV2::UnlinkFlower(AStormFlowerV2* _flowerToUnlink)
{
	const int _index = IsFlowerLinked(_flowerToUnlink);

	if (_index != -1)
		powerLines[_index]->DestroyPowerLine();
}

int AStormFlowerV2::IsFlowerLinked(AStormFlowerV2* _flower)
{
	if (powerLines.IsEmpty() || !_flower)
		return -1;

	const int _num = powerLines.Num();
	AStormFlowerV2* _linkedFlower = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_linkedFlower = powerLines[i]->GetLinkedFlower(this);

		if (_linkedFlower == _flower)
			return i;
	}

	return -1;
}

void AStormFlowerV2::ServerRPC_SpawnPowerLine_Implementation(AStormFlowerV2* _flowerToLink)
{
	SpawnPowerLine(_flowerToLink);
}

void AStormFlowerV2::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnAreaBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor->GetClass()->IsChildOf<AStormFlowerV2>())
	{
		AStormFlowerV2* _flower = Cast<AStormFlowerV2>(OtherActor);
		nearbyFlowers.Add(_flower);
		LinkFlower(_flower);
	}
}

void AStormFlowerV2::OnAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnAreaEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor && OtherActor->GetClass()->IsChildOf<AStormFlowerV2>())
	{
		AStormFlowerV2* _flower = Cast<AStormFlowerV2>(OtherActor);
		nearbyFlowers.Remove(_flower);
		UnlinkFlower(_flower);
	}
}

void AStormFlowerV2::Init()
{
	USphereComponent* _sphereComp = Cast<USphereComponent>(effectArea);

	if (_sphereComp)
	{
		initialZoneRadius = _sphereComp->GetScaledSphereRadius();
		zoneRadius = initialZoneRadius;
	}

	Super::Init();

	FTimerHandle _findFlowersTimer = FTimerHandle();
	GetWorld()->GetTimerManager().SetTimer(_findFlowersTimer, this, &AStormFlowerV2::FindNearbyFlowers, 1.0f, false);
}

void AStormFlowerV2::StartActivePhase()
{
	Super::StartActivePhase();
	ActivateNearbyFlowers();
	ActivatePowerLines();
}

void AStormFlowerV2::StartCooldownPhase()
{
	Super::StartCooldownPhase();
	DisableModifiers();
}

void AStormFlowerV2::ApplyEffectToActorsInArea()
{
	Super::ApplyEffectToActorsInArea();

	if (actorsInArea.IsEmpty())
		return;

	const int _num = actorsInArea.Num();
	UStatusComponent* _statusComp = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_statusComp = actorsInArea[i]->GetComponentByClass<UStatusComponent>();

		if (_statusComp)
			_statusComp->AddStatus(this, deactivatedStatus);
	}
}

void AStormFlowerV2::EnableModifiers()
{
	Super::EnableModifiers();

	zoneRadius *= zoneRadiusModifier;

	USphereComponent* _sphereComp = Cast<USphereComponent>(effectArea);

	if (_sphereComp)
		_sphereComp->SetSphereRadius(zoneRadius);
}

void AStormFlowerV2::DisableModifiers()
{
	Super::DisableModifiers();

	zoneRadius = initialZoneRadius;

	USphereComponent* _sphereComp = Cast<USphereComponent>(effectArea);

	if (_sphereComp)
		_sphereComp->SetSphereRadius(zoneRadius);
}

void AStormFlowerV2::LightningActivation()
{
	EnableModifiers();
	StartActivePhase();
}

void AStormFlowerV2::RemovePowerLine(APowerLineV2* _powerLine)
{
	if (powerLines.Contains(_powerLine))
		powerLines.Remove(_powerLine);
}
