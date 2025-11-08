


#include "HancarAnim.h"
#include <Net/UnrealNetwork.h>

void UHancarAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//LOG(true, GetOwningActor()->GetName());
	handcar= Cast<ANitroHandcarActor>(GetOwningActor());
}


void UHancarAnim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHancarAnim, handcar);
}
