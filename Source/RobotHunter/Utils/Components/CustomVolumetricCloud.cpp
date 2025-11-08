


#include "CustomVolumetricCloud.h"

ACustomVolumetricCloud::ACustomVolumetricCloud()
{
	VolumetricCloudComponent = CreateDefaultSubobject<UCustomVolumetricCloudComponent>("CustomVolumetricCloudComponent");
	RootComponent = VolumetricCloudComponent;
}
