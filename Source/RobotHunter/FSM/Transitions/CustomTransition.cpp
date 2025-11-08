#include "CustomTransition.h"
#include "../States/CustomState.h"
#include "../CustomFSM.h"
#include <Kismet/KismetSystemLibrary.h>

UCustomTransition::UCustomTransition()
{
	nextState = nullptr;
	fsm = nullptr;
}

bool UCustomTransition::IsValidTransition()
{
	return false;
}
