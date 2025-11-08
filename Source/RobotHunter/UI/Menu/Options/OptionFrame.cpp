// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionFrame.h"

#include "Components/ScrollBoxSlot.h"
#include "Components/Spacer.h"
#include "Data/Accessibilty/Lang/LanguageOption.h"
#include "Data/Graphics/Module/GraphicsOptionBase.h"
#include "Data/Graphics/Module/Preset/PresetGraphics.h"
#include "Data/Keybind/KeyBindMapping.h"
#include "GameFramework/GameUserSettings.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Template/Prefab/CheckBoxPrefab/CheckboxPrefab.h"
#include "RobotHunter/UI/Template/Prefab/OptionablePrefab.h"
#include "RobotHunter/UI/Template/Prefab/SeparatorPrefab/SeparatorPrefab.h"
#include "RobotHunter/Utils/FileLogger.h"

void UOptionFrame::Refresh()
{
	Super::Refresh();
	SET_TEXT_STR(descriptionOption->GetText(), "");
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Refresh OptionFrame");
	//ShowCategory(static_cast<int>(ECategoryOption::None));
	/*for (UWidget* AllChild : ScrollboxCategory->GetScrollbox()->GetAllChildren())
	{
		if (UButtonPrefab* Button = Cast<UButtonPrefab>(AllChild))
		{
			if (Button->CanKeepSelected())
			{
				Button->Callback_OnUnSelected();
			}
		}
	}*/
}

void UOptionFrame::AddChildParameter(const TObjectPtr<UOptionablePrefab>& _parameter) const
{
	TObjectPtr<UScrollBoxSlot> _panel = Cast<UScrollBoxSlot>(optionParameter->GetScrollbox()->AddChild(_parameter));
	_panel->SetPadding(optionsSettings->GetPaddingBetweenElement());
}

void UOptionFrame::AddCategory(const TSubclassOf<USeparatorPrefab>& _prefab, const FString& _name)
{
	USeparatorPrefab* _pref = CreateWidget<USeparatorPrefab>(GetWorld(), _prefab);
	//if (!first)
	//{
	//	first = true;
	//	//_pref->SetFirst();
	//}
	TObjectPtr<UScrollBoxSlot> _panel = Cast<UScrollBoxSlot>(optionParameter->GetScrollbox()->AddChild(_pref));
	_panel->SetPadding(optionsSettings->GetPaddingBetweenElement());

	_pref->SetCategoryName(_name);
}

void UOptionFrame::ShowCategory(const int _cat)
{
	if (!hasModification)
		ConfirmShowCategory(_cat);
	else
	{
		categoryToShow = _cat;
		onCategoryChange.Broadcast();
	}
}

void UOptionFrame::ConfirmShowCategory(const int _cat)
{
	categoryToShow = -1;
	ECategoryOption _category = static_cast<ECategoryOption>(_cat);
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "Show category " + GET_ENUM_STRING(_category));
	if (currentCategory == _category)
	{
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Category already set");
		return;
	}

	if (currentCategory == ECategoryOption::None)
	{
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Call event first time launch");
	}
	else
	{
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Call Category change");
	}

	oldCategory = currentCategory;
	currentCategory = _category;
	resetBtn->SetVisibility(optionsSettings->GetOptionByCategory(currentCategory).bCanReset ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	const TObjectPtr<UScrollBox> _scrollbox = optionParameter->GetScrollbox();
	if (_scrollbox->GetChildrenCount() > 0)
	{
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Clear all children");
		_scrollbox->ClearChildren();
		if (oldCategory != ECategoryOption::None)
		{
			optionsSettings->ResetOptions(oldCategory);
		}
		SetEnableWidget<UButtonPrefab>(applyParameter, hasModification = false);
	}

	if (currentCategory == ECategoryOption::None)
	{
		return;
	}
	const bool _isInMainMenu = instance->GetGameStatus() == MAINMENU;
	const TArray<FOptionCategory> _datas = optionsSettings->GetOptionByCategory(currentCategory).options;

	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Get option from category");
	for (const FOptionCategory& OptionDatas : _datas)
	{
		for (const TObjectPtr<UOptionData>& OptionData : OptionDatas.options)
		{
			if (INSTANCEOF(OptionData, UKeyBindMapping))
			{
				UKeyBindMapping* _key = Cast<UKeyBindMapping>(OptionData);
				_key->Refresh();
				break;
			}
			UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
			                      "Get option: " + OptionData->GetNameOption());
			CHECK_NULL(OptionData, "Option data is not set in OptionFrame");
			CHECK_NULL(OptionData->GetPrefab(), "Prefab of option " + OptionData.GetName() + " is null");
			TObjectPtr<UOptionablePrefab> _prefab = CreateWidget<
				UOptionablePrefab>(GetWorld(), OptionData->GetPrefab());
			UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Create widget from prefab");
			CHECK_NULL_WITH_RETURN(_prefab, "Slot failed to create", break);

			SetupPrefab(OptionData, _prefab, _isInMainMenu ? true : OptionData->CanBeModifyInGame());
		}
		TObjectPtr<USpacer> _spacer = NewObject<USpacer>(this, optionsSettings->GetSpacerReference());
		CHECK_NULL_WITH_RETURN(_spacer, "Spacer is null", continue);
		_spacer->SetSize(FVector2D(0, optionsSettings->GetSpaceBetweenCategory()));
		_scrollbox->AddChild(_spacer);
	}
}

void UOptionFrame::ChangePresetOption(const TObjectPtr<UOptionData>& _dataPrefab) const
{
	if (INSTANCEOF(_dataPrefab, UPresetGraphicsOption))
	{
		const UPresetGraphicsOption* _preset = Cast<UPresetGraphicsOption>(_dataPrefab);
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
		                      "Get current value preset: " + INTSTR(_preset->GetCurrentValue()));
		if (_preset->GetCurrentValue() == static_cast<int>(EGraphicsOptionValue::CUSTOM))
		{
			return;
		}

		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Change value for other");

		const TArray<UWidget*> _child = optionParameter->GetScrollbox()->GetAllChildren();
		for (const TObjectPtr<UWidget>& Child : _child)
		{
			if (INSTANCEOF(Child, UOptionablePrefab)) //ON check si le child est bien un truc d'option graphique
			{
				const TObjectPtr<UOptionablePrefab> _p = Cast<UOptionablePrefab>(Child);
				if (_p->GetData() == _preset || !INSTANCEOF(_p->GetData(), UGraphicsOptionBase))
				{
					continue;
				}
				_p->SetCurrentIndexWithoutEvent(_preset->GetCurrentValue());
			}
		}
	}
	else
	{
		ChangePresetOptionByOtherOption(_dataPrefab);
	}
}

void UOptionFrame::ChangePresetOptionByOtherOption(const TObjectPtr<UOptionData>& _dataPrefab) const
{
	if (INSTANCEOF(_dataPrefab, UGraphicsOptionBase)) //Check si la modif est un asset graphique
	{
		const TArray<UWidget*> _child = optionParameter->GetScrollbox()->GetAllChildren();
		for (const TObjectPtr<UWidget>& Child : _child)
		{
			if (INSTANCEOF(Child, UOptionablePrefab)) //ON check si le chdil est bien un truc d'option
			{
				TObjectPtr<UOptionablePrefab> _p = Cast<UOptionablePrefab>(Child);
				const TObjectPtr<UOptionData> _data = _p->GetData();
				if (INSTANCEOF(_data, UPresetGraphicsOption)) //Quand j'arrive à trouver l'option du preset
				{
					TObjectPtr<UPresetGraphicsOption> _graphics = Cast<UPresetGraphicsOption>(_data);
					_graphics->RefreshValue();
					break;
				}
			}
		}
	}
}

void UOptionFrame::ShowValueChangedDetected(const UOptionablePrefab* _prefab)
{
	SetEnableWidget<UButtonPrefab>(applyParameter, hasModification = true);
	const TObjectPtr<UOptionData> _dataPrefab = _prefab->GetData();
	ChangePresetOption(_dataPrefab);
}

void UOptionFrame::ApplyAllParameter()
{
	const TArray<FOptionCategory> _datas = optionsSettings->GetOptionByCategory(currentCategory).options;
	for (const FOptionCategory& OptionDatas : _datas)
	{
		for (const TObjectPtr<UOptionData>& OptionData : OptionDatas.options)
		{
			CHECK_NULL(OptionData, "Option data is not set in OptionFrame");
			if (OptionData->HasChanged())
				OptionData->Apply();
		}
	}
	gus->ApplySettings(true);
	gus->SaveSettings();
	gus->ApplyResolutionSettings(true);
	GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->SaveSettings();
	//LOG_DEBUG("Saved settings !");

	SetEnableWidget<UButtonPrefab>(applyParameter, hasModification = false);
}

void UOptionFrame::ShowDescriptionOption(const FString& Msg)
{
	SET_TEXT_STR(descriptionOption->GetText(), Msg);
}

void UOptionFrame::Back()
{
	if (hasModification)
	{
		onBack.Broadcast();
		return;
	}
	onDirectBack.Broadcast();
	ConfirmBack();
}

void UOptionFrame::ConfirmBack()
{
	for (UWidget* AllChild : optionParameter->GetScrollbox()->GetAllChildren())
	{
		if (INSTANCEOF(AllChild, UOptionablePrefab))
		{
			UOptionablePrefab* _option = Cast<UOptionablePrefab>(AllChild);
			_option->SetCurrentIndexWithoutEvent(_option->GetData()->GetDefaultIndex());
		}
	}
	SetEnableWidget<UButtonPrefab>(applyParameter, hasModification = false);
	
	if (GetGameInstance<UCustomGameInstance>()->GetGameStatus() == MAINMENU)
	{
		if (AMainHUD* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AMainHUD>())
		{
			
			_hud->SetState(EStateUI::MainMain);
			
		}
	}
	else
	{
		owner->ShowGame();
	}
}

void UOptionFrame::Reset()
{
	onResetConfirmation.Broadcast();
}

void UOptionFrame::ConfirmReset()
{
	const TArray<FOptionCategory> _datas = optionsSettings->GetOptionByCategory(currentCategory).options;
	for (const FOptionCategory& OptionDatas : _datas)
	{
		for (const TObjectPtr<UOptionData>& OptionData : OptionDatas.options)
		{
			CHECK_NULL(OptionData, "Option data is not set in OptionFrame");
			if (!INSTANCEOF(OptionData, ULanguageOption))
			{
				OptionData->ResetToDefault();
				OptionData->GetCurrentPrefab()->SetCurrentIndexWithoutEvent(OptionData->GetCurrentValue());
			}
		}
	}
	LOG_DEBUG("Confirm reset");
}

void UOptionFrame::Init()
{
	Super::Init();

	resetBtn->SetVisibility(ESlateVisibility::Collapsed);

	CHECK_NULL_WITH_RETURN(optionsSettings, "Options Settings is not set in OptionFrame", return);
	instance = GetGameInstance<UCustomGameInstance>();

	SET_TEXT_STR(descriptionOption->GetText(), "");

	gus = UGameUserSettings::GetGameUserSettings();
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(), "Trying Get GameUserSettings...");
	if (!gus)
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::SubSystem, GetClass(), "Failed Get GameUserSettings");


	USaveManagerSubsystem* _manager = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(),
	                      "Trying to get saveManager Subsystem...");
	if (!_manager)
		UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::SubSystem, GetClass(), "Manager failed to load");


	USettingsSave* _settings = _manager->GetSaveSettings();
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::SubSystem, GetClass(),
	                      "Trying to get SettingsSave from manager...");

	optionsSettings->LoadOptions(this, gus, GetWorld(), _settings);

	SetEnableWidget<UButtonPrefab>(applyParameter, false);

	const TArray<FString> _categories = optionsSettings->GetCategoryName();
	const TArray<ECategoryOption> _categoryKey = optionsSettings->GetCategory();
	const int size = _categories.Num();
	const TObjectPtr<UScrollBox> _scrollbox = ScrollboxCategory->GetScrollbox();
	for (int i = 0; i < size; i++)
	{
		TObjectPtr<UButtonCategoryPrefab> _button = CreateWidget<UButtonCategoryPrefab>(
			GetWorld(), optionsSettings->GetButtonReference());
		CHECK_NULL_WITH_RETURN(_button, "Button of category failed to create", continue);

		TObjectPtr<UScrollBoxSlot> _slot = Cast<UScrollBoxSlot>(_scrollbox->AddChild(_button));
		CHECK_NULL_WITH_RETURN(_button, "Slot failed to cast", continue);

		//_slot->SetPadding(FMargin(0, i == 0 ? 0 : 20, 0, i == size - 1 ? 0 : 20));

		SET_TEXT_STR(_button->GetText()->GetText(), _categories[i]);
		_button->Setup(this, _categoryKey[i]);
	}
	gus->SaveSettings();
}

void UOptionFrame::RegisterEvents()
{
	Super::RegisterEvents();

	BIND_ACTION(applyParameter, OnClick, UOptionFrame, ApplyAllParameter);
	BIND_ACTION(backBtn, OnClick, UOptionFrame, Back);
	BIND_ACTION(resetBtn, OnClick, UOptionFrame, Reset);
}

void UOptionFrame::SetupPrefab(const TObjectPtr<UOptionData>& OptionData, const TObjectPtr<UOptionablePrefab>& _prefab,
                               const bool _canBeEdited)
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(),
	                      "Setup for " + OptionData->GetNameOption());
	AddChildParameter(_prefab);
	_prefab->SetIsEnabled(_canBeEdited);
	BIND_ACTION(_prefab, OnChangeDetected, UOptionFrame, ShowValueChangedDetected);
	BIND_ACTION(_prefab, OnHover, UOptionFrame, ShowDescriptionOption);
	_prefab->SetOptionData(OptionData);
}

TArray<TObjectPtr<UOptionablePrefab>> UOptionFrame::GetOptionsWidget() const
{
	TArray<TObjectPtr<UOptionablePrefab>> options;
	const TObjectPtr<UScrollBox> _scrollbox = optionParameter->GetScrollbox();

	TArray<UWidget*> _widgets = _scrollbox->GetAllChildren();
	for (const TObjectPtr<UWidget>& _widget : _widgets)
	{
		if (INSTANCEOF(_widget, UOptionablePrefab))
		{
			options.Add(Cast<UOptionablePrefab>(_widget));
		}
	}
	return options;
}
