#include "UI.h"

#include "HUD/MainHUD.h"
#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "RobotHunter/Utils/FileLogger.h"


class UCommonInputSubsystem;

void UUI::Init()
{
	/*KeyToDirectionMap = {
		{EKeys::Gamepad_DPad_Down, EInputDirection::Down},
		{EKeys::Gamepad_DPad_Up, EInputDirection::Up},
		{EKeys::Gamepad_DPad_Left, EInputDirection::Left},
		{EKeys::Gamepad_DPad_Right, EInputDirection::Right}
	};*/
	
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Init");
	onInit.Broadcast();
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UUI::InitWithDelay, 1.f, false, 0.2f);

	if (GetGameInstance())
		inputSubsystem = GetGameInstance()->GetSubsystem<UInputSubsystem>();
}

void UUI::RegisterEvents()
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Register Events");
	onRegisterEvents.Broadcast();

	CHECK_NULL(GETFPC, "FPC is null");
	CHECK_NULL(GETFPC->GetLocalPlayer(), "Local Player is null");

	GetGameInstance()->GetSubsystem<UInputSubsystem>()->OnInputControllerChange().AddUniqueDynamic(
		this, &UUI::HandleInputChanged);
}

void UUI::InitWithDelay()
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Init with delay");
	onInitWithDelay.Broadcast();
	RegisterEvents();
}

FReply UUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	static const TMap<FKey, EInputDirection> KeyToDirectionMap = {
		{EKeys::Gamepad_DPad_Down, EInputDirection::Down},
		{EKeys::Gamepad_DPad_Up, EInputDirection::Up},
		{EKeys::Gamepad_DPad_Left, EInputDirection::Left},
		{EKeys::Gamepad_DPad_Right, EInputDirection::Right}
	};
	
	if (const EInputDirection* Direction = KeyToDirectionMap.Find(InKeyEvent.GetKey()))
	{
		inputSubsystem->OnInputGamepadReceive().Broadcast(*Direction);
		return FReply::Handled();
	}
    
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUI::SetParent(UUI* _parent)
{
	ParentClass = _parent;
}

void UUI::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UUI::Refresh()
{
	onRefresh.Broadcast();
}

void UUI::Setup(const TObjectPtr<AMainHUD>& _hud)
{
	hud = _hud;
}

TObjectPtr<UWidget> UUI::GetFirstWidget()
{
	return nullptr;
}

TObjectPtr<UWidget> UUI::GetLastWidget()
{
	return nullptr;
}

void UUI::HandleInputChanged(bool isGamepad)
{
	//GETFPC->SetShowMouseCursor(!isGamepad);
}

void UUI::ExitUI()
{
}
