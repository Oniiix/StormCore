#include "PlantLootActor.h"
#include <RobotHunter/Game/Interactable/Bag/BagActor.h>
#include "../../../../CCC/Character/MainCharacter.h"
#include "../../../../Utils/DebugUtils.h"
#include <RobotHunter/Network/OnlineManagerSubsystem.h>
#include <RobotHunter/Network/Component/NetworkGPEComponent.h>

void APlantLootActor::Interacted(AMainCharacter* _character)
{
	if (_character)
	{
		/*SetDropName("Biomass");
		UInventoryComponent* _inventory = _character->GetInventoryComponent();*/
		ULifeComponent* _life = _character->GetLifeComponent();

		/*if (_inventory)
		{
			if (isRandom)
				numberLoot = FMath::RandRange(minDrop, maxDrop);

			_inventory->AddItemToInventory(dropName, numberLoot);

			if (HasAuthority())
				SetLifeSpan(0.1);
			else
				GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_DestroyCustomActor(this);
		}*/

		if (_life)
		{
			onInteract.Broadcast(_character);

			if (HasAuthority())
				network->ClientRpc_CallOnInteractEvent(this, _character);
			else
				network->ServerRpc_CallOnInteractEvent(this, _character);

			LaunchAnimation(_character);
			_life->RestoreLife(givenLife);
			canBeDetected = false;
			widgetUI->SetVisibility(ESlateVisibility::Hidden);

			UInteractionComponent* _interactComp = _character->GetInteractionComponent();

			if (_interactComp)
				_interactComp->RemoveActor(GetName());
		}
	}
}