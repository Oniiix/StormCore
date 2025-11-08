#include "FlowerV2.h"

AFlowerV2::AFlowerV2()
{
#pragma region Cooldown
	useRandomCooldown = false;
	cooldown = 3.0f;
	minCooldown = 0.1f;
	maxCooldown = 3.0f;

	cooldownTimer = FTimerHandle();
#pragma endregion

	isActive = false;
	activeTimer = FTimerHandle();

	bReplicates = true;
	SetReplicateMovement(true);
}

void AFlowerV2::StartActivePhase()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(cooldownTimer))
		_tm.ClearTimer(cooldownTimer);
	onActivate.Broadcast();
	isActive = true;
}

void AFlowerV2::StartCooldownPhase()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(activeTimer))
		_tm.ClearTimer(activeTimer);

	isActive = false;
	const float _cooldown = useRandomCooldown ? FMath::RandRange(minCooldown, maxCooldown) : cooldown;
	_tm.SetTimer(cooldownTimer, this, &AFlowerV2::StartActivePhase, _cooldown, false);
}

void AFlowerV2::GoodWeatherCallback()
{
	EnableModifiers();
}

void AFlowerV2::WrongWeatherCallback()
{
	DisableModifiers();
}

void AFlowerV2::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void AFlowerV2::DrawDebug()
{
	DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0.0f, 0.0f, 200.0f), 50.0f, 10.0f, isActive ? FColor::Green : FColor::Red, false);
}
