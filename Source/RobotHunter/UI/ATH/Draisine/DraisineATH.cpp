#include "DraisineATH.h"
#include "RobotHunter/Utils/DebugUtils.h"


void UDraisineATH::SetRotationAngleSync(const float _angleRotation)
{
	SetRotationAngleValue(true, _angleRotation);
}

void UDraisineATH::SetRotationAngle(ESliderColor _slider, const float _angleRotation)
{
	SetRotationAngleValue(false, _angleRotation, _slider);
}

void UDraisineATH::SetAngle(ESliderColor _slider, const bool _sync, float _sizeAngleStart, float _sizeAngleEnd) const
{
	const TObjectPtr<URadialSlider> _radialSlider = GetSliderFromEnum(_slider);

	if (!_radialSlider)
	{
		return;
	}

	_sizeAngleStart = CLAMP(_sizeAngleStart, 0.0f, 360.0f);
	_sizeAngleEnd = CLAMP(_sizeAngleEnd, 0.f, 360.0f);

	_radialSlider->SetSliderHandleStartAngle(_sizeAngleStart);
	_radialSlider->SetSliderHandleEndAngle(_sync ? 360.f - _sizeAngleStart : _sizeAngleEnd);
}

void UDraisineATH::SetValueProgress(float _value) const
{
	if (!progressSlider)
	{
		return;
	}

	_value = CLAMP(_value, 0, 1);
	progressSlider->SetValue(_value);
}

void UDraisineATH::SetProgressAngularOffset(float _angleRotation)
{
	if (!progressSlider)
	{
		return;
	}

	const float _minValue = GetMinSlider(BlackSlider);
	const float _maxValue = GetMaxSlider(BlackSlider);

	_angleRotation = _angleRotation <= _minValue
		                 ? _minValue
		                 : _angleRotation >= _maxValue
		                 ? _maxValue
		                 : _angleRotation;

	progressSlider->SetAngularOffset(_angleRotation);
}

void UDraisineATH::SetProgressColor(const ESliderColor& _color)
{
	if (!progressSlider)
	{
		return;
	}

	URadialSlider* _slider = GetSliderFromEnum(_color);

	if (!_slider)
	{
		return;
	}

	FLinearColor _sliderColor = _slider->SliderBarColor;
	progressSlider->SetSliderHandleColor(_sliderColor);
}

void UDraisineATH::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (overrideBlackSliderValue)
	{
		InitializeRadialSlider(blackRadialSlider, blackSliderInformation);
	}
	if (overrideRedSliderValue)
	{
		InitializeRadialSlider(redRadialSlider, redSliderInformation);
	}
	if (overrideYellowSliderValue)
	{
		InitializeRadialSlider(yellowRadialSlider, yellowSliderInformation);
	}
	if (overrideGreenSliderValue)
	{
		InitializeRadialSlider(greenRadialSlider, greenSliderInformation);
	}
}

void UDraisineATH::InitializeRadialSlider(const TObjectPtr<URadialSlider>& _radialSlider,
                                          FSliderInformation& _information) const
{
	_radialSlider->SetSliderHandleStartAngle(_information.sliderStartAngle);

	if (_information.syncAngle)
	{
		_information.sliderEndAngle = 360.f - _information.sliderStartAngle;
	}

	_radialSlider->SetSliderHandleEndAngle(_information.sliderEndAngle);
	_radialSlider->SetAngularOffset(_information.angleRotation);
}

float UDraisineATH::GetMinSlider() const
{
	float _min = 0;

	constexpr int _size = SliderCount;
	for (int i = 0; i < _size; i++)
	{
		float _startSlider = GetSliderFromEnum(static_cast<ESliderColor>(i))->SliderHandleStartAngle;
		if (_startSlider < _min)
		{
			_min = _startSlider;
		}
	}
	return _min;
}

float UDraisineATH::GetMinSlider(const ESliderColor& _color) const
{
	URadialSlider* _slider = GetSliderFromEnum(_color);

	if (!_slider)
	{
		return 0.0f;
	}

	return _slider->SliderHandleStartAngle;
}

float UDraisineATH::GetMaxSlider() const
{
	int _max = 0;
	constexpr int _size = SliderCount;
	for (int i = 0; i < _size; i++)
	{
		float _endSlider = GetSliderFromEnum(static_cast<ESliderColor>(i))->SliderHandleEndAngle;
		if (_endSlider > _max)
		{
			_max = _endSlider;
		}
	}
	return _max;
}

float UDraisineATH::GetMaxSlider(const ESliderColor& _color) const
{
	URadialSlider* _slider = GetSliderFromEnum(_color);

	if (!_slider)
	{
		return 0.0f;
	}

	return _slider->SliderHandleEndAngle;
}

#if WITH_EDITOR
void UDraisineATH::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FString parent = PropertyChangedEvent.MemberProperty->GetName().ToLower();
	const FString propertyName = PropertyChangedEvent.Property->GetName().ToLower();

	if (parent.Equals("blackSliderInformation", ESearchCase::IgnoreCase))
	{
		InitializeRadialSlider(blackRadialSlider, blackSliderInformation);
		progressSlider->SetSliderHandleStartAngle(blackRadialSlider->SliderHandleStartAngle);
		progressSlider->SetSliderHandleEndAngle(blackRadialSlider->SliderHandleEndAngle);
	}
	else if (parent.Equals("redSliderInformation", ESearchCase::IgnoreCase))
	{
		InitializeRadialSlider(redRadialSlider, redSliderInformation);
	}
	else if (parent.Equals("yellowSliderInformation", ESearchCase::IgnoreCase))
	{
		InitializeRadialSlider(yellowRadialSlider, yellowSliderInformation);
	}
	else if (parent.Equals("greenSliderInformation", ESearchCase::IgnoreCase))
	{
		InitializeRadialSlider(greenRadialSlider, greenSliderInformation);
	}
	else if (propertyName.Equals("angleRotation", ESearchCase::IgnoreCase))
	{
		InitializeRadialSlider(greenRadialSlider, greenSliderInformation);
		InitializeRadialSlider(yellowRadialSlider, yellowSliderInformation);
	}
}
#endif

void UDraisineATH::SetRotationAngleValue(const bool _sync, float _angleRotation, const ESliderColor _sliderColor)
{
	_angleRotation = CLAMP(_angleRotation, 0.0f, 360.0f);

	if (_sync)
	{
		if (greenRadialSlider && yellowRadialSlider)
		{
			greenRadialSlider->SetAngularOffset(_angleRotation);
			yellowRadialSlider->SetAngularOffset(_angleRotation);
		}
	}
	else
	{
		URadialSlider* _slider = GetSliderFromEnum(_sliderColor);

		if (!_slider)
		{
			return;
		}

		_slider->SetAngularOffset(_angleRotation);
	}
}
