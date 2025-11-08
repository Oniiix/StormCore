#include "PowerLineV2.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/DamageZoneFlower/StormFlower/StormFlowerV2.h"

APowerLineV2::APowerLineV2()
{
	useRandomActivationTime = false;
	activationTime = 1.0f;
	minActivationTime = 0.5f;
	maxActivationTime = 2.0f;

	firstFlower = nullptr;
	secondFlower = nullptr;

	lineStart = FVector();
	lineEnd = FVector();

	drawDebugPowerLine = false;

	lightningTimer = FTimerHandle();
}

void APowerLineV2::LightningCooldown()
{
	if (firstFlower && firstFlower->GetIsActive())
	{
		ActivateLightning();
		return;
	}

	if (secondFlower && secondFlower->GetIsActive())
		ActivateLightning();
}

void APowerLineV2::DrawDebug()
{
	if (drawDebugPowerLine)
		DrawDebugLine(GetWorld(), lineStart, lineEnd, FColor::Purple, false, -1.0f, 0U, 2.0f);
}

void APowerLineV2::Init(const FVector& _lineStart, const FVector& _lineEnd, AStormFlowerV2* _firstFlower, AStormFlowerV2* _secondFlower)
{
	firstFlower = _firstFlower;
	secondFlower = _secondFlower;

	lineStart = _lineStart;
	lineEnd = _lineEnd;
}

void APowerLineV2::ActivateLightning()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(lightningTimer))
		_tm.ClearTimer(lightningTimer);

	const float _activationTime = useRandomActivationTime ? FMath::RandRange(minActivationTime, maxActivationTime) : activationTime;
	_tm.SetTimer(lightningTimer, this, &APowerLineV2::LightningCooldown, _activationTime, false);
	onLightningActivation.Broadcast();
}

void APowerLineV2::DestroyPowerLine()
{
	if (firstFlower)
		firstFlower->RemovePowerLine(this);

	if (secondFlower)
		secondFlower->RemovePowerLine(this);

	SetLifeSpan(0.1f);
}
