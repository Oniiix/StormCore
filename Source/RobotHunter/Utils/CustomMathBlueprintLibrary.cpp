// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMathBlueprintLibrary.h"
#include "DebugUtils.h"

#pragma region Trigo

FVector UCustomMathBlueprintLibrary::GetWorldTrigoPointXZ(const float _angle, const float _radius)
{
	const float _x = COS(DEG_TO_RAD(_angle), _radius),
	            _z = SIN(DEG_TO_RAD(_angle), _radius);
	return FVector(_x, 0, _z);
}

FVector UCustomMathBlueprintLibrary::GetWorldTrigoPointXY(const float _angle, const float _radius)
{
	const float _x = COS(DEG_TO_RAD(_angle), _radius),
				_y = SIN(DEG_TO_RAD(_angle), _radius);
	return FVector(_x, _y, 0);
}

FVector UCustomMathBlueprintLibrary::GetWorldTrigoPointZY(const float _angle, const float _radius)
{
	const float _z = COS(DEG_TO_RAD(_angle), _radius),
				_y = SIN(DEG_TO_RAD(_angle), _radius);
	return FVector(0, _y, _z);
}

FVector UCustomMathBlueprintLibrary::GetLocalTrigoPointXZ(const float _angle, const float _radius, const FVector& _forward, const FVector& _up)
{
	const float _x = COS(DEG_TO_RAD(_angle), _radius),
				_z = SIN(DEG_TO_RAD(_angle), _radius);
	return _forward * _x + _up * _z;
}

FVector UCustomMathBlueprintLibrary::GetLocalTrigoPointXY(const float _angle, const float _radius, const FVector& _forward, const FVector& _right)
{
	const float _x = COS(DEG_TO_RAD(_angle), _radius),
				_y = SIN(DEG_TO_RAD(_angle), _radius);
	return _forward * _x + _right * _y;
}

FVector UCustomMathBlueprintLibrary::GetLocalTrigoPointZY(const float _angle, const float _radius, const FVector& _up, const FVector& _right)
{
	const float _z = COS(DEG_TO_RAD(_angle), _radius),
				_y = SIN(DEG_TO_RAD(_angle), _radius);
	return _up * _z + _right * _y;
}

float UCustomMathBlueprintLibrary::FindAngle(const FVector2D& _dir)
{
	float _result = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(FVector2D(1, 0), _dir) / (FVector2D(1, 0).Length() * _dir.Length())));
	_result = FMath::IsNegativeOrNegativeZero(_dir.Y) && _dir.Y < 0 ? -_result : _result;
	return _result;
}

float UCustomMathBlueprintLibrary::Angle(const FVector& _a, const FVector& _b)
{
	const float _rad = FVector::DotProduct(_a, _b) / (_a.Size() * _b.Size());
	return FMath::Acos(_rad) / (UKismetMathLibrary::GetPI() / 180);
}

FVector UCustomMathBlueprintLibrary::GetForwardFromRotation(const FRotator& _rotation)
{
	FVector _direction;
	_direction.X = FMath::Cos(DEG_TO_RAD(_rotation.Pitch)) * FMath::Cos(DEG_TO_RAD(_rotation.Yaw));
	_direction.Y = FMath::Cos(DEG_TO_RAD(_rotation.Pitch)) * FMath::Sin(DEG_TO_RAD(_rotation.Yaw));
	_direction.Z = -FMath::Sin(DEG_TO_RAD(_rotation.Pitch));
	return _direction;
}

FVector UCustomMathBlueprintLibrary::GetRightFromRotation(const FRotator& _rotation)
{
	FVector _direction;
	_direction.X = -FMath::Sin(DEG_TO_RAD(_rotation.Yaw));
	_direction.Y = FMath::Cos(DEG_TO_RAD(_rotation.Yaw));
	_direction.Z = 0;
	return _direction;
}

FVector UCustomMathBlueprintLibrary::GetUpFromRotation(const FRotator& _rotation)
{
	FVector _direction;
	_direction.X = FMath::Sin(DEG_TO_RAD(_rotation.Pitch)) * FMath::Cos(DEG_TO_RAD(_rotation.Yaw));
	_direction.Y = FMath::Sin(DEG_TO_RAD(_rotation.Pitch)) * FMath::Sin(DEG_TO_RAD(_rotation.Yaw));
	_direction.Z = FMath::Cos(DEG_TO_RAD(_rotation.Pitch));
	return _direction;
}

#pragma endregion

#pragma region Equals

bool UCustomMathBlueprintLibrary::IsVectorSuperiorOrEqual(const FVector& _a, const FVector& _b)
{
	return _a.X >= _b.X && _a.Y >= _b.Y && _a.Z >= _b.Z;
}

bool UCustomMathBlueprintLibrary::IsVectorInferiorOrEqual(const FVector& _a, const FVector& _b)
{
	return _a.X <= _b.X && _a.Y <= _b.Y && _a.Z <= _b.Z;
}


bool UCustomMathBlueprintLibrary::IsIntEqual(const int _a, const int _b, const int _margin)
{
	return _a >= _b - _margin && _a <= _b + _margin;
}

bool UCustomMathBlueprintLibrary::IsFloatEqual(const float _a, const float _b, const float _margin)
{
	return _a >= _b - _margin && _a <= _b + _margin;
}

void UCustomMathBlueprintLibrary::AddAndAssign(float& Target, float Value)
{
	Target += Value;
}

#pragma endregion

#pragma region Range

bool UCustomMathBlueprintLibrary::IsFloatInRange(const float _value, const float _min, const float _max)
{
	return _value >= _min && _value <= _max;
}

bool UCustomMathBlueprintLibrary::IsIntInRange(const int _value, const int _min, const int _max)
{
	return _value >= _min && _value <= _max;
}

bool UCustomMathBlueprintLibrary::IsFloatInRangeStrict(const float _value, const float _min, const float _max)
{
	return _value > _min && _value < _max;
}

bool UCustomMathBlueprintLibrary::IsIntInRangeStrict(const int _value, const int _min, const int _max)
{
	return _value > _min && _value < _max;
}
#pragma endregion

float UCustomMathBlueprintLibrary::RoundToNumberAfterPoint(const double _a, const int _n)
{
	const int _multiply = FMath::Pow(10, (float)_n);
	float _result = _a * _multiply;
	_result = FMath::RoundToInt(_result);
	return _result / _multiply;
}
#pragma endregion

#pragma region normalize
float UCustomMathBlueprintLibrary::NormalizeValueZeroMax(const float _valueToNormalize, const float _max)
{
	return NORMALIZE(_valueToNormalize, 0, _max);
}

float UCustomMathBlueprintLibrary::NormalizeValue(const float _valueToNormalize, const float _min, const float _max)
{
	return NORMALIZE(_valueToNormalize, _min, _max);
}
#pragma endregion

FVector UCustomMathBlueprintLibrary::CustomDistance(const FVector& _v1, const FVector& _v2)
{
	const float _x = FMath::Abs(_v1.X - _v2.X);
	const float _y = FMath::Abs(_v1.Y - _v2.Y);
	const float _z = FMath::Abs(_v1.Z - _v2.Z);
	return FVector(_x, _y, _z);
}

void UCustomMathBlueprintLibrary::IncrementValue(int32& _a, const int32 _b)
{
	_a+=_b;
}

void UCustomMathBlueprintLibrary::IncrementValueByOne(int32& _a)
{
	_a++;
}

void UCustomMathBlueprintLibrary::DecrementValue(int32& _a, int32 _b)
{
	_a-=_b;
}

void UCustomMathBlueprintLibrary::DecrementValueByOne(int32& _a)
{_a--;
}
void UCustomMathBlueprintLibrary::PositiveModulo(int32& _value, int32 _modulo, int32& _result)
{
	_result = _value % _modulo;
	if (_result < 0)	
		_result += _modulo;	
	_value = _result;
}
