// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomExponentialHeightFog.h"
#include "Net/UnrealNetwork.h"
#include "Components/BillboardComponent.h"

ACustomExponentialHeightFog::ACustomExponentialHeightFog()
{
	Component = CreateDefaultSubobject<UCustomExponentialHeightFogComponent>(TEXT("HeightFogComponent0"));
	RootComponent = Component;

	SetHidden(false);

#if WITH_EDITORONLY_DATA
	if (!IsRunningCommandlet() && (GetSpriteComponent() != NULL))
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> FogTextureObject;
			FName ID_Fog;
			FText NAME_Fog;
			FConstructorStatics()
				: FogTextureObject(TEXT("/Engine/EditorResources/S_ExpoHeightFog"))
				, ID_Fog(TEXT("Fog"))
				, NAME_Fog(NSLOCTEXT("SpriteCategory", "Fog", "Fog"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		GetSpriteComponent()->Sprite = ConstructorStatics.FogTextureObject.Get();
		GetSpriteComponent()->SetRelativeScale3D_Direct(FVector(0.5f, 0.5f, 0.5f));
		GetSpriteComponent()->SpriteInfo.Category = ConstructorStatics.ID_Fog;
		GetSpriteComponent()->SpriteInfo.DisplayName = ConstructorStatics.NAME_Fog;
		GetSpriteComponent()->SetupAttachment(Component);
	}
#endif
}

void ACustomExponentialHeightFog::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACustomExponentialHeightFog, bEnabled)
}

void ACustomExponentialHeightFog::OnRep_bEnabled()
{
	Component->SetVisibility(bEnabled);
}

void ACustomExponentialHeightFog::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bEnabled = Component->GetVisibleFlag();
}
