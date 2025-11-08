// Objectif 3D Copyright


#include "WorldInteractUI.h"

#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/Interactable/InteractableActor.h"


class UEnhancedInputUserSettings;
class UEnhancedInputLocalPlayerSubsystem;

void UWorldInteractUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//if (inSight)
	//{
	//	const float _distance = FVector::Distance(owner->GetActorLocation(), chara->GetActorLocation());
	//	if (inInteractionZone && _distance > interactionRange)
	//	{
	//		inInteractionZone = false;
	//		LOG_COLOR(true, "Exit Interaction Zone", ORANGE);
	//	}
	//	else if (!inInteractionZone && _distance <= interactionRange)
	//	{
	//		inInteractionZone = true;
	//		LOG_COLOR(true, "Enter Interaction Zone", GREEN);
	//	}
	//}
}

void UWorldInteractUI::Setup()
{
	Super::Setup();

	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetWorld()->GetFirstLocalPlayerFromController()->
	                                                                    GetSubsystem<
		                                                                    UEnhancedInputLocalPlayerSubsystem>();
	if (UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings())
	{
		UserSettings->LoadOrCreateSettings(GetWorld()->GetFirstLocalPlayerFromController());

		//get all profiles (we only have one)
		for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->
		     GetAllSavedKeyProfiles())
		{
			const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;
			if (!Profile)
			{
				LOG_ERROR(true, "Profile is null");
				return;
			}

			for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
			{
				const int _size = RowPair.Value.Mappings.Num();
				//get every ActionMapping in mapping
				for (int i = 0; i < _size; i++)
				{
					FPlayerKeyMapping _mapping = RowPair.Value.Mappings.Get(FSetElementId::FromInteger(0));

					const FString _mappingName = _mapping.GetMappingName().ToString().ToLower();
					const FString _keyLower = key.ToLower();
					if (_mappingName == _keyLower)
					{
						FString _format = FORMAT(formatText.ToString(), _mapping.GetDisplayName().ToString());
						text = _format;
						SET_TEXT_STR(interactionText->GetText(), text);
						InputKey->SetSelectedKey(_mapping.GetCurrentKey());
					}
				}
			}
		}
	}

}

void UWorldInteractUI::InitWithDelay()
{
	Super::InitWithDelay();
}

void UWorldInteractUI::Init()
{
	Super::Init();
	//Setup();
	if (GetWorld()->GetFirstPlayerController()->GetCharacter()->IsA<AMainCharacter>())
	{
		chara = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		interactionRange = chara->GetInteractionComponent()->GetInteractionRadius();
	}
}

void UWorldInteractUI::SetKey(const FText& Text)
{
	key = Text.ToString();
}