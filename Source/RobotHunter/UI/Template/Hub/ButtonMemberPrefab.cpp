#include "ButtonMemberPrefab.h"

#include "Animation/UMGSequencePlayer.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/GPE/Members/MembersActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/Menu/Hub/StockMenu/NewStock.h"


UButtonMemberPrefab::UButtonMemberPrefab()
{
	menu = nullptr;
	handlers = {
		{ALeftArmActor::StaticClass(), EWeaponType::LEFT},
		{ARightArmActor::StaticClass(), EWeaponType::RIGHT},
		{ACalfActor::StaticClass(), EWeaponType::CALF},
		{AThighActor::StaticClass(), EWeaponType::THIGH},
	};
}

void UButtonMemberPrefab::SetMember(const TSubclassOf<AMembersActor>& member, const TObjectPtr<UNewStock>& stockMenu)
{
	const TObjectPtr<ACustomPlayerController> _pc = Cast<ACustomPlayerController>(
		GetWorld()->GetFirstPlayerController());
	CHECK_NULL(_pc, "Player Controller is null");

	currentMemberReference = member;
	menu = stockMenu;
	currentMember = currentMemberReference.GetDefaultObject();
	isEquip = IsEquipped(currentMember, _pc);

	CHECK_NULL(textBase, "Text is null");
	CHECK_NULL(currentMember, "Current member is null");
	SET_TEXT_STR(textBase->GetText(),
	             FORMAT(format, currentMember->GetMemberName().ToString()));

	if (isEquip)
	{
		PlayAnimation(EquippedAnimation);
		menu->SetMemberButtonSelected(GetType(), this);
	}
	else
	{
		PlayAnimation(CanEquipAnimation);
	}
}

void UButtonMemberPrefab::EquipMember()
{
	if (isEquip)
	{
		return;
	}
	isEquip = true;

	//const TObjectPtr<UCustomGameInstance> _gi = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	const TObjectPtr<ACustomPlayerController>& _pc = Cast<ACustomPlayerController>(
		GetWorld()->GetFirstPlayerController());
	CHECK_NULL(_pc, "Game instance is null");

	EquipMember(_pc);
	menu->Callback_OnChangeMember();
}

void UButtonMemberPrefab::EquipMember(const TObjectPtr<ACustomPlayerController>& _playerController)
{
	//On crée un tableau de map, qui prend en clé la classe
	//On Crée une pair associé, qui sera une fonction et une enum
	const TMap<UClass*, TPair<TFunction<void(const TSubclassOf<AMembersActor>&)>, EWeaponType>> _handlers = {
		{
			ARightArmActor::StaticClass(),
			{
				[_playerController](const TSubclassOf<AMembersActor>& Member)
				{
					_playerController->SetRightArm(Member);
				},
				EWeaponType::RIGHT
			}
		},
		{
			ALeftArmActor::StaticClass(),
			{
				[_playerController](const TSubclassOf<AMembersActor>& Member)
				{
					_playerController->SetLeftArm(Member);
				},
				EWeaponType::LEFT
			}
		},
		{
			ACalfActor::StaticClass(),
			{
				[_playerController](const TSubclassOf<AMembersActor>& Member) { _playerController->SetCalf(Member); },
				EWeaponType::CALF
			}
		},
		{
			AThighActor::StaticClass(),
			{
				[_playerController](const TSubclassOf<AMembersActor>& Member) { _playerController->SetThigh(Member); },
				EWeaponType::THIGH
			}
		},
	};

	if (const TObjectPtr<UButtonMemberPrefab> _buttonMember = menu->GetButtonMember(GetType()))
	{
		_buttonMember->StopAllAnimations();
		_buttonMember->PlayAnimation(CanEquipAnimation);
		_buttonMember->SetEquip(false);
	}

	for (const TPair<UClass*, TPair<TFunction<void(const TSubclassOf<AMembersActor>&)>, EWeaponType>>& _handler :
	     _handlers)
	{
		//Verifie si le membre est du type de la clé
		if (currentMember->IsA(_handler.Key))
		{
			isEquip = true;

			StopAllAnimations();
			PlayAnimation(EquippedAnimation);

			_handler.Value.Key(currentMemberReference); //appel de la fonction
			menu->SetMemberButtonSelected(_handler.Value.Value, this); //set dans le menu, la value, et le bouton
			break; //stop de la boucle quand trouver
		}
	}
}

void UButtonMemberPrefab::HoverInformation()
{
	menu->SetMemberSelectedTemporary(currentMember);
}

void UButtonMemberPrefab::ClearInformation()
{
	menu->SetMemberSelectedTemporary(nullptr);
}

void UButtonMemberPrefab::RegisterEvents()
{
	Super::RegisterEvents();

	BIND(OnClick, this, UButtonMemberPrefab::EquipMember);
	BIND(OnHover, this, UButtonMemberPrefab::HoverInformation);
	BIND(OnUnHover, this, UButtonMemberPrefab::ClearInformation);
}


bool UButtonMemberPrefab::IsEquipped(const TObjectPtr<AMembersActor>& _member,
                                     const TObjectPtr<ACustomPlayerController>& _pc) const
{
	for (const TObjectPtr<AMembersActor>& _membersActor : Cast<AMainCharacter>(GETFPC->GetCharacter())->GetUsingMembers())
	{
		CHECK_NULL_WITH_RETURN(_membersActor, "_member Actor is null", return false);
		CHECK_NULL_WITH_RETURN(_member, "_member is null", return false);

		if (_membersActor->GetMemberType() == _member->GetMemberType())
		{
			return true;
		}
	}
	return false;
}

EWeaponType UButtonMemberPrefab::GetType() const
{
	for (const TTuple<UClass*, EWeaponType>& _handler : handlers)
	{
		if (currentMember->IsA(_handler.Key))
		{
			return _handler.Value;
		}
	}
	return EWeaponType::None;
}
