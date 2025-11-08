// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AffectByNetLauncherInterface.generated.h"


UINTERFACE(MinimalAPI)
class UAffectByNetLauncherInterface : public UInterface
{
	GENERATED_BODY()
};


class ROBOTHUNTER_API IAffectByNetLauncherInterface
{
	GENERATED_BODY()

public:
	virtual void AffectByNetLauncher();
};
