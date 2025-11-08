#include "ZoneFlowerV2.h"
#include "RobotHunter/AI/AI_Base.h"

AZoneFlowerV2::AZoneFlowerV2()
{
#pragma region Effect

#pragma region Duration
	useRandomEffectDuration = false;
	effectDuration = 4.0f;
	minEffectDuration = 0.1f;
	maxEffectDuration = 4.0f;
#pragma endregion

#pragma region Area
	actorsInArea = TArray<AActor*>();
#pragma endregion

	effectRate = 1.0f;
#pragma endregion

	disableModifiers = false;
}

void AZoneFlowerV2::InitEffectTimer()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();
	_tm.SetTimer(effectTimer, this, &AZoneFlowerV2::ApplyEffectToActorsInArea, effectRate, true);
}

void AZoneFlowerV2::ClearEffectTimer()
{
	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (_tm.IsTimerActive(effectTimer))
		_tm.ClearTimer(effectTimer);
}

void AZoneFlowerV2::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UClass* _class = OtherActor->GetClass();

		if (_class->IsChildOf<AMainCharacter>() || _class->IsChildOf<AAI_Base>())
			actorsInArea.Add(OtherActor);
	}
}

void AZoneFlowerV2::OnAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (actorsInArea.Contains(OtherActor))
			actorsInArea.Remove(OtherActor);
	}
}

void AZoneFlowerV2::Init()
{
	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}

	if (effectArea)
	{
		effectArea->OnComponentBeginOverlap.AddDynamic(this, &AZoneFlowerV2::OnAreaBeginOverlap);
		effectArea->OnComponentEndOverlap.AddDynamic(this, &AZoneFlowerV2::OnAreaEndOverlap);
		effectArea->bHiddenInGame = !useDebug;
	}

	StartCooldownPhase();
}

void AZoneFlowerV2::StartActivePhase()
{
	Super::StartActivePhase();

	ClearEffectTimer();
	InitEffectTimer();
	const float _effectDuration = useRandomEffectDuration ? FMath::RandRange(minEffectDuration, maxEffectDuration) : effectDuration;
	GetWorld()->GetTimerManager().SetTimer(activeTimer, this, &AZoneFlowerV2::StartCooldownPhase, _effectDuration, false);
}

void AZoneFlowerV2::StartCooldownPhase()
{
	ClearEffectTimer();

	Super::StartCooldownPhase();
}
