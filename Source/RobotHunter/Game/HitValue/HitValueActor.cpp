// 


#include "HitValueActor.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/GameModeBase.h"
#include "RobotHunter/UI/FloatingUI/LifeHit/HitValue.h"


AHitValueActor::AHitValueActor()
{
	PrimaryActorTick.bCanEverTick = true;

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	SetRootComponent(widgetComponent);
	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComponent->SetDrawAtDesiredSize(true);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/00_Prog/Game/HitValue/Bp_HitValue"));
	if (WidgetClass.Succeeded())
	{
		widgetComponent->SetWidgetClass(WidgetClass.Class);
	}
}

void AHitValueActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifeSpan);
	SetActorLocation(GetActorLocation() + FVector(1, 0, 0), false);
}

/*
void AHitValueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector actorLocation = GetActorLocation();
    
	const float amplitude = amplitudeOfWidget;
	const float _newHeight = amplitude * (FMath::Sin(amplitude * (ResultTime + DeltaTime)) - FMath::Sin(amplitude * ResultTime));

	actorLocation.X += 1.f;
	actorLocation.Z += _newHeight;

	LOG_DEBUG(actorLocation.ToString());
    
	ResultTime += DeltaTime;
	SetActorLocation(actorLocation * speedMoving);
}*/

void AHitValueActor::SetHitDamage(const int _damage, const bool _critical)
{
	CHECK_NULL(widgetComponent, "Widget component is null");
	CHECK_NULL(widgetComponent->GetWidget(), "Widget is null");

	UHitValue* HitWidget = Cast<UHitValue>(widgetComponent->GetWidget());
	if (!HitWidget) return;

	damage = _damage;
	HitWidget->SetTextDamage(_damage, _critical);
	CALLBACK_AFTER_TIME(timeToStartFade, &AHitValueActor::StartFade, GetWorld());
}

void AHitValueActor::StartFade()
{
	if (UHitValue* _hitValueWidget = Cast<UHitValue>(widgetComponent->GetWidget()))
		_hitValueWidget->Spawn(lifeSpan - timeToStartFade);
}

#if WITH_EDITOR

void AHitValueActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FString StrLower = PropertyName.ToString().ToLower();

	TMap<FString, TFunction<void()>> Events =
	{
		{
			"timeToStartFade", [&]
			{
				if (timeToStartFade > lifeSpan)
					timeToStartFade = lifeSpan;
			}
		}
	};

	if (Events.Contains(StrLower))
		Events[StrLower]();
}
#endif
