// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightning.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/Game/GPE/Flower/StormFlower.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/DamageZoneFlower/StormFlower/StormFlowerV2.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "Net/UnrealNetwork.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "Components/SphereComponent.h"

ALightning::ALightning()
{
	sphere = CreateDefaultSubobject<USphereComponent>("light sphere");
	sphere->SetupAttachment(RootComponent);
	bReplicates = true;
}

void ALightning::BeginPlay()
{
	Super::BeginPlay();
	if (sphere)
	{
		sphere->OnComponentEndOverlap.AddDynamic(this, &ALightning::EndOverlap);
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ALightning::BeginOverlap);
	}
	CheckCeiling();
}

void ALightning::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	GeneratePreview();
}


void ALightning::GeneratePreview()
{
	//TODO draw lightning
}

void ALightning::CheckCeiling()
{
	FHitResult _result;
	const FVector _ground = GetActorLocation() + FVector::DownVector * GetActorLocation().Z * 2;

	const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(this, GetActorLocation(), _ground, layersObstacle, false, {}, EDrawDebugTrace::ForDuration, _result, true);
	if (_hit)
	{
		LOG_COLOR(LIGHTNING_USE_LOG, "Lightning touch ", YELLOW);
		SetActorLocation(_result.ImpactPoint);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(_result.ImpactPoint, _result.ImpactPoint + _result.Normal) + FRotator(90, 0, 0));
	}
	INVOKE(onCall)
		GenerateLightning();
}

void ALightning::GenerateLightning()
{
	if (sphere)
		sphere->SetSphereRadius(radiusDamage, true);

	CheckStormFlower();
	LookForStormFlowerV2();

	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, FName("GenerateDamageZone"));
	GetWorld()->GetTimerManager().SetTimer(timerPreviewTime, timerDelegate, previewTime, false, -1.0f);
}

void ALightning::GenerateDamageZone()
{
	CLEAR_TIMER(WORLD, timerPreviewTime);
	INVOKE(onLightningStrike);
	if (!HasAuthority())
		return;

	if (stormflowerTouching)
		stormflowerTouching->ActiveFlowerPhase();
	stormflowerTouching = nullptr;

	//V2
	if (touchedStormFlower)
	{
		touchedStormFlower->LightningActivation();
		touchedStormFlower = nullptr;
	}

	for (AActor* _item : actorTouchingByLight)
	{
		//TODO heritance
		AMainCharacter* _onePlayer = Cast<AMainCharacter>(_item);
		AAI_Base* _oneAI = Cast<AAI_Base>(_item);
		if (_onePlayer && _onePlayer->GetLifeComponent())
		{
			LOG_COLOR(LIGHTNING_USE_LOG, "ALightning ZAP dammage " + _onePlayer->GetName(), PINK);
			_onePlayer->GetLifeComponent()->Damage(damage, this);
			//TODO Stun
		}

		if (_oneAI && _oneAI->GetLifeComponent())
		{
			_oneAI->GetLifeComponent()->Damage(damage, this);
			//TODO Stun
		}
	}
	Destroy();
}

void ALightning::CheckStormFlower()
{
	for (AActor* _item : actorTouchingByLight)
	{
		AStormFlower* _stormflower = Cast<AStormFlower>(_item);
		if (_stormflower)
		{
			LOG_COLOR(LIGHTNING_USE_LOG, "ALightning STORM FLOWER!!", PINK);
			SetActorLocation(_stormflower->GetActorLocation());
			stormflowerTouching = _stormflower;
			return;
		}
	}
}

void ALightning::LookForStormFlowerV2()
{
	for (AActor* _actor : actorTouchingByLight)
	{
		AStormFlowerV2* _stormflower = Cast<AStormFlowerV2>(_actor);

		if (_stormflower)
		{
			LOG_COLOR(LIGHTNING_USE_LOG, "[Lightning] Found StormFlowerV2", PINK);
			SetActorLocation(_stormflower->GetActorLocation());
			touchedStormFlower = _stormflower;
			return;
		}
	}
}

void ALightning::Destroy()
{
	//LOG_ERROR(LIGHTNING_USE_LOG, "ALightning::Destroy");
	SetLifeSpan(0.1f);
}

void ALightning::DrawDebug()
{
	speedLightning += DELTA_TIME;
	const float radius = FMath::Lerp(0, radiusDamage, speedLightning / previewTime);

	DRAW_CIRCLE(GetActorLocation(), FMath::Clamp(radius, 0, radiusDamage), FColor::Red, GetActorForwardVector(), GetActorRightVector());
	DRAW_CIRCLE(GetActorLocation(), radiusDamage, FColor::Red, GetActorForwardVector(), GetActorRightVector());
}

void ALightning::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALightning, speedLightning);
}

void ALightning::BeginOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _weepResult)
{
	//LOG_COLOR_NO_GAME(1, "ALightning::BeginOverlap "+ _otherActor->GetName(), PINK);
	if (_otherActor && !actorTouchingByLight.Contains(_otherActor))
		actorTouchingByLight.Add(_otherActor);
}

void ALightning::EndOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	//LOG_COLOR_NO_GAME(1, "ALightning::EndOverlap "+ _otherActor->GetName(), TURQUOISE);
	if (_otherActor)
		if (actorTouchingByLight.Contains(_otherActor))
			actorTouchingByLight.Remove(_otherActor);
}