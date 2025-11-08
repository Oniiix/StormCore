#include "LifebarWidgetComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <Kismet/GameplayStatics.h>


ULifebarWidgetComponent::ULifebarWidgetComponent()
{
	DrawSize = {200, 100};
}

void ULifebarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	mainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//if (ui = Cast<UWorldLifebarUI>(GetUserWidgetObject()))
	//{
	//	if (lifeComponent = GetOwner()->FindComponentByClass<ULifeComponent>())
	//		lifeComponent->OnUpdateLife().AddUniqueDynamic(this, &ULifebarWidgetComponent::RefreshUI);
	//}
}

void ULifebarWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (mainCharacter && lookAtPlayer)
	{
		const FRotator& _newRot = UKismetMathLibrary::FindLookAtRotation(
			GetComponentLocation(), mainCharacter->GetCameraComponent()->GetCamera()->GetComponentLocation());
		SetWorldRotation(_newRot);
	}
}

void ULifebarWidgetComponent::SetWidgetVisibility(const bool _value)
{
	SetComponentTickEnabled(_value);
	ui->SetVisibility(_value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ULifebarWidgetComponent::CallbackOnHit()
{
	if ((ui = Cast<UWorldLifebarUI>(GetWidget())))
	{
		ui->CallbackOnHit();
	}
}


void ULifebarWidgetComponent::InitLifebar(ULifeComponent* _lifeComponent)
{
	CHECK_NULL(GetUserWidgetObject(), "UserWidgetObject is null!!");

	if ((ui = Cast<UWorldLifebarUI>(GetUserWidgetObject())))
	{
		ui->SetVisibility(ESlateVisibility::Hidden);
		if ((lifeComponent = GetOwner()->FindComponentByClass<ULifeComponent>()))
		{
			lifeComponent->OnUpdateLife().AddUniqueDynamic(this, &ULifebarWidgetComponent::RefreshUI);
			ui->UpdateLifeBar(lifeComponent->GetCurrentLife(), lifeComponent->GetMaxLife());
		}
	}
}

void ULifebarWidgetComponent::RefreshUI(AActor* _owner, const int _currentLife, const int _change,
                                        AActor* _damageSource)
{
	if ((lifeComponent = GetOwner()->FindComponentByClass<ULifeComponent>()))
	{
		if (_currentLife < lifeComponent->GetMaxLife())
			ui->SetVisibility(ESlateVisibility::Visible);
	}
	ui->UpdateLifeBar(_currentLife, lifeComponent->GetMaxLife());
}
