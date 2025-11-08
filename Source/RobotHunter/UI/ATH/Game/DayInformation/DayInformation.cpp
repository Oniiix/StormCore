// 


#include "DayInformation.h"

#include "Kismet/KismetTextLibrary.h"
#include "RobotHunter/Game/Managers/Time/TimeManager.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"


void UDayInformation::UpdateTime(const int Hour, const int Minute)
{
	const FString _formatted = FORMAT(stringToShow, CONV_INT(Hour, 2), CONV_INT(Minute, 2));

	SET_TEXT_STR(textDayWidget->GetText(), FORMAT(format, _formatted));

	const float _minuteInGameDuration = timeManager->GetInGameDurationInMinutes();
	const float _minuteElapsed = timeManager->GetInGameElapsedInMinutes();

	const float _percent = _minuteElapsed / _minuteInGameDuration;

	SetCircleFilled(circleFill, _percent);
}

void UDayInformation::ChangeColor(UImage* _image, const FSettingsCircleFilled& settingsCircle)
{
	_image->GetDynamicMaterial()->SetVectorParameterValue("Color", settingsCircle.Color);
}

void UDayInformation::ChangeColorText(const FLinearColor& _color) const
{
	textDayWidget->SetColorAndOpacity(_color);
}

void UDayInformation::NativePreConstruct()
{
	Super::NativePreConstruct();
	ChangeColor(circleFill, settingsCircleFill);
	ChangeColor(circleEmpty, settingsCircleEmpty);
}

void UDayInformation::ChangeColor(UImage* _image, const FLinearColor& color)
{
	_image->GetDynamicMaterial()->SetVectorParameterValue("Color", color);
}


void UDayInformation::Init()
{
	Super::Init();
	useDebug = false;
	fill = 0.f;
	SetEnableWidget<UTextPrefab>(textDayWidget, false);
	SetCircleFilled(circleFill, 0.f);
	ChangeColor(circleFill, settingsCircleFill);
	ChangeColor(circleEmpty, settingsCircleEmpty);
	ChangeColorText(colorNormal);
}

void UDayInformation::InitWithDelay()
{
	TArray<AActor*> actorsFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeManager::StaticClass(), actorsFound);

	if (actorsFound.Num() == 0)
	{
		LOG_ERROR(true, "No time manager in level");
		return;
	}
	timeManager = Cast<ATimeManager>(actorsFound[0]);

	const FString _formatted = FORMAT(stringToShow, CONV_INT(timeManager->GetInGameHour(), 2), CONV_INT(0, 2));
	SetEnableWidget<UTextPrefab>(textDayWidget, true);
	SET_TEXT_STR(textDayWidget->GetText(), _formatted);
	Super::InitWithDelay();
}

void UDayInformation::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(timeManager, OnUpdateTime, UDayInformation, UpdateTime);
}

void UDayInformation::SetCircleFilled(UImage* _circle, const float _percent) const
{
	if (!_circle)
	{
		return;
	}

	const float _normalize = _percent * settingsCircleEmpty.maxFill;
	const float _percentNormalize = percentChangeColor * settingsCircleFill.maxFill;

	if (_normalize >= _percentNormalize)
	{
		_circle->GetDynamicMaterial()->SetVectorParameterValue("Color", colorWarning);
		ChangeColorText(colorWarningText);
	}

	_circle->GetDynamicMaterial()->SetScalarParameterValue("Percentage", _normalize);
}

#if WITH_EDITOR

void UDayInformation::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FString parent = PropertyChangedEvent.GetMemberPropertyName().ToString();
	const FString parameter = parent + "." + PropertyName.ToString();

	GLog->Log(parameter);
	TMap<FString, TFunction<void()>> Events =
	{
		{
			"settingsCircleEmpty.maxFill", [&]
			{
				settingsCircleFill.maxFill = settingsCircleEmpty.maxFill;
				if (!circleFill || !circleEmpty)
				{
					return;
				}
				SetCircleFilled(circleEmpty, 1);
				SetCircleFilled(circleFill, fill);
			}
		},
		{
			"settingsCircleFill.maxFill", [&]
			{
				settingsCircleEmpty.maxFill = settingsCircleFill.maxFill;
				if (!circleFill || !circleEmpty)
				{
					return;
				}
				SetCircleFilled(circleEmpty, 1);
				SetCircleFilled(circleFill, fill);
			}
		},
		{
			"settingsCircleFill.angleOffset", [&]
			{
				settingsCircleEmpty.angleOffset = settingsCircleFill.angleOffset;
			}
		},
		{
			"settingsCircleEmpty.angleOffset", [&]
			{
				settingsCircleFill.angleOffset = settingsCircleEmpty.angleOffset;
			}
		},
		{
			"settingsCircleFill.Color", [&]
			{
				if (!circleFill)
				{
					return;
				}
				ChangeColor(circleFill, settingsCircleFill);
			}
		},
		{
			"fill.fill", [&]
			{
				if (!circleFill)
				{
					return;
				}
				SetCircleFilled(circleFill, fill);
			}
		}
	};

	if (Events.Contains(parameter))
	{
		Events[parameter]();
	}
}
#endif
