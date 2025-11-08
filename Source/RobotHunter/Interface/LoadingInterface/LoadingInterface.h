

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoadingInterface.generated.h"


UINTERFACE(MinimalAPI)
class ULoadingInterface : public UInterface
{
	GENERATED_BODY()

};


class ROBOTHUNTER_API ILoadingInterface
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE(FOnFinishLoad);

protected:
	FOnFinishLoad onFinishLoad;

public:
	FORCEINLINE FOnFinishLoad& OnFinishLoad() { return onFinishLoad; }
};
