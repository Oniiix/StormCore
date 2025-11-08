#include "HitDamage.h"
#include "HitDamageWidget.h"

AHitDamage::AHitDamage()
{
	PrimaryActorTick.bCanEverTick = true;

	widget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	RootComponent = widget;
}

void AHitDamage::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(lifeSpan);
}

void AHitDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector _newLocation = GetActorLocation() + (FVector::UpVector * speed);
	SetActorLocation(_newLocation);
}

void AHitDamage::InitHitDamage(const int _damage)
{
	UHitDamageWidget* _hitDamageWidget = Cast<UHitDamageWidget>(widget->GetWidget());
	if (_hitDamageWidget)
	{
		_hitDamageWidget->InitWidget(_damage);
	}
}
