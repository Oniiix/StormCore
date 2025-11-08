#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UCustomActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCustomActorComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/// <summary>
	/// Si vous voulez ajouter un ActorComponent dans votre CustomActorComponent, vous aurez besoin de cette fonction. -------
	/// 1. Créez votre CustomActorComponent dans le constructeur de votre actor. ----------------------------------------------
	/// 2. Appelez cette fonction : customActorComponent->SetupAttachment(RootComponent); ------------------------------------
	/// </summary>
	/// <param name="_root">RootComponent de l'owner</param>
	virtual void SetupAttachment(USceneComponent* _root);
};
