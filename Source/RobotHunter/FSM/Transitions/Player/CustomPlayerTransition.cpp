#include "CustomPlayerTransition.h"
#include "../../Components/PlayerFSMComponent.h"

UCustomPlayerTransition::UCustomPlayerTransition()
{
	player = nullptr;
}

void UCustomPlayerTransition::RetrievePlayer()
{
	if (player)
		return;

	if (fsm)
	{
		UPlayerFSMComponent* _playerFSMComp = dynamic_cast<UPlayerFSMComponent*>(fsm->GetOwner());

		if (_playerFSMComp)
			player = _playerFSMComp->GetPlayer();
	}
}
