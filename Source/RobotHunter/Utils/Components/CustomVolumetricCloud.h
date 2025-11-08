

#pragma once

#include "CoreMinimal.h"
#include "Components/VolumetricCloudComponent.h"
#include "CustomVolumetricCloudComponent.h"
#include "CustomVolumetricCloud.generated.h"

/**
 * 
 */
UCLASS(showcategories = (Movement, Rendering, Transformation, DataLayers, "Input|MouseInput", "Input|TouchInput"), ClassGroup = Fog, hidecategories = (Info, Object, Input))
class ROBOTHUNTER_API ACustomVolumetricCloud : public AInfo
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Atmosphere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCustomVolumetricCloudComponent> VolumetricCloudComponent;

public:
	ACustomVolumetricCloud();

#if WITH_EDITOR
	virtual bool ActorTypeSupportsDataLayer() const override { return true; }
#endif

};
