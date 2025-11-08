// 


#include "ButtonPopupPrefab.h"

#include "RobotHunter/UI/Template/Popup/PopupConfirmation.h"

void UButtonPopupPrefab::SetClickedButtonPopup()
{
	parent->SetClickedButton(this);
}

void UButtonPopupPrefab::Setup(UPopupConfirmation* PopupConfirmation, const FString& Key)
{
	parent = PopupConfirmation;

	OnClick().AddUniqueDynamic(this, &UButtonPopupPrefab::SetClickedButtonPopup);

	CALLBACK_LAMBDA_AFTER_TIME(1.0f, SET_TEXT_STR(textBase->GetText(), Key););

    //FTimerHandle handle;
    //FString KeyCopy = Key; // Copie sécurisée
    //
    //GetWorld()->GetTimerManager().SetTimer(handle,
    //    FTimerDelegate::CreateLambda([this, KeyCopy]()  // Captures sécurisées
    //        {
    //            if (IsValid(this) && textBase)  // Toujours vérifier la validité
    //            {
    //                //SET_TEXT_STR(textBase->GetText(), KeyCopy);
    //                textBase->GetText()->SetText(FText::FromString(KeyCopy));
    //            }
    //        }),
    //    1.0f, FTimerManagerTimerParameters());

}