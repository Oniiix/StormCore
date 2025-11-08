// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

//=========== Color ==============
#define WHITE FLinearColor(1.f, 1.f, 1.f)
#define BLACK FLinearColor(0.f, 0.f, 0.f)
#define RED FLinearColor(1.f, 0.f, 0.f)
#define GREEN FLinearColor(0.f, 1.f, 0.f)
#define BLUE FLinearColor(0.f, 0.f, 1.f)
#define YELLOW FLinearColor(1.f, 1.f, 0.f)
#define MAGENTA FLinearColor(0.5f, 0.f, 0.5f)
#define CYAN FLinearColor(0.f, 1.f, 1.f)
#define ORANGE FLinearColor(1.f, 0.5f, 0.f)
#define PINK FLinearColor(1.f, 0.f, 1.f)
#define TURQUOISE FLinearColor(0.f, 1.f, 0.5f)
//=========== Color ==============


//=========== Other ==============
#define WORLD GetWorld()
#define DELTA_TIME WORLD->DeltaTimeSeconds
#define ACTOR_NAME GetActorNameOrLabel()
#define OWNER_NAME GetOwner()->GetActorNameOrLabel()
#define OWNER_AUTHORITY GetOwner()->HasAuthority()
#define FORMAT(fstring, ...) FString::Format(*fstring, {__VA_ARGS__})
#define GAME_STATE(gameState) WORLD->GetGameState<gameState>()
#define SET_TIMER(world,timerHandle,object,classMethod,rate, isLoop, firstDelay) world->GetTimerManager().SetTimer(timerHandle, object, &classMethod, rate, isLoop, firstDelay);
#define CLEAR_TIMER(world,timerHandle) world->GetTimerManager().ClearTimer(timerHandle);
#define GETFPC GetWorld()->GetFirstPlayerController()
#define CREATE_SUBOBJECT(object, name) CreateDefaultSubobject<object>(FName(name))
#define CREATE_NEWOBJECT(outer, object, name) NewObject<object>(outer, object::StaticClass(), FName(name))
/**
 * Make an instruction after a delay specified
 * @param delay delay in float
 * @param ... instructions
 */
#define CALLBACK_LAMBDA_AFTER_TIME(delay, ...) FTimerHandle handle; \
	GetWorld()->GetTimerManager().SetTimer(handle, [&]{ __VA_ARGS__ },	0.1f, false, delay)

/**
 * call a method after a delay specified
 * @param delay delay in float
 * @param method method
 */
#define CALLBACK_AFTER_TIME(delay, method, ...) FTimerHandle handle; \
if(!GetWorld()) __VA_ARGS__->GetTimerManager().SetTimer(handle, this, method,0.1f, false, delay);\
else GetWorld()->GetTimerManager().SetTimer(handle, this, method,0.1f, false, delay)
//=========== Other ==============


//=========== Class ==============
#define CHECK_NULL(ptr, msg_error) if(!IsValid(ptr)) {LOG_ERROR(GAME_LOG, msg_error); UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(), msg_error); return; }
#define CHECK_NULL_WITH_RETURN(ptr, msg_error, return_type) if(!IsValid(ptr)) {  LOG_ERROR(GAME_LOG, msg_error); UFileLogger::WriteLog(ELogSeverity::ERROR, ELogCategory::UI, GetClass(), msg_error); return_type; }
#define INSTANCEOF(ptr, baseClass) ptr->IsA(baseClass::StaticClass())
//=========== Class ==============


//========== Event ==============
#define INVOKE(eventDelegate, ...)eventDelegate.Broadcast( __VA_ARGS__);
#define BIND(eventDelegate,obj, classMethod) eventDelegate().AddDynamic(obj, &classMethod);
//========== Event ==============


//=========== Log ==============
#define GAME_LOG true
#define LOG(display, msg) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, msg, true, true, WHITE, 8.f)
#define LOG_COLOR(display, msg, color) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, msg, true, true, color, 8.f)
#define LOG_TIME(display, msg, time) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, msg, true, true, WHITE, time)
#define LOG_TICK(display, msg) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, msg, true, true, WHITE, DELTA_TIME)
#define LOG_COLOR_TIME(display, msg, color, time) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, msg, true, true, color, time)
#define LOG_COLOR_TICK(display, msg, color) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, msg, true, false, color, DELTA_TIME)
#define LOG_WARNING(display, msg) if (GAME_LOG && display) UKismetSystemLibrary::PrintString(this, "[" + this->GetClass()->GetName() + "] " + msg, true, true, ORANGE, 8.f)
#define LOG_ERROR(display, msg) if (GAME_LOG) UKismetSystemLibrary::PrintString(this, "[" + this->GetClass()->GetName() + "] " + msg, true, true, RED, 8.f)
//=========== Log ==============


//=========== DrawDebug ==============
#define GAME_DEBUG true
#define DRAW_LINE(from, to, color) if (GAME_DEBUG) DrawDebugLine(WORLD, from, to, color)
#define DRAW_LINE_SIZE(from, to, color, size) if (GAME_DEBUG) DrawDebugLine(WORLD, from, to, color, false, -1, 0, size)
#define DRAW_LINE_TIME(from, to, color, time) if (GAME_DEBUG) DrawDebugLine(WORLD, from, to, color, false, time)
#define DRAW_LINE_SIZE_TIME(from, to, color, size, time) if (GAME_DEBUG) DrawDebugLine(WORLD, from, to, color, false, time, 0, size)
#define DRAW_CONE(location, direction, radius, angleW_deg, angleH_deg, color) if (GAME_DEBUG) DrawDebugCone(WORLD, location, direction, radius, UKismetMathLibrary::DegreesToRadians(angleW_deg), UKismetMathLibrary::DegreesToRadians(angleH_deg), 16, color)
#define DRAW_CIRCLE(location, radius, color, direction, right) if (GAME_DEBUG) DrawDebugCircle(WORLD, location, radius, 25, color, false, -1, 0, 5.f, direction, right, false)
#define DRAW_CIRCLE_TIME(location, radius, color, direction, right, time) if (GAME_DEBUG) DrawDebugCircle(WORLD, location, radius, 25, color, false, time, 0, 5.f, direction, right, false)
#define DRAW_CIRCLE_ARC(location, radius, color, direction, angle) if (GAME_DEBUG) DrawDebugCircleArc(WORLD, location, radius, direction, angle, 10, color, false, -1, 0, 5.f)
#define DRAW_CIRCLE_ARC_TIME(location, radius, color, direction, angle, time) if (GAME_DEBUG) DrawDebugCircleArc(WORLD, location, radius, direction, angle, 10, color, false, time, 0, 5.f)
#define DRAW_SPHERE(location, radius, color) if (GAME_DEBUG) DrawDebugSphere(WORLD, location, radius, 15, color)
#define DRAW_SPHERE_TIME(location, radius, color, time) if (GAME_DEBUG) DrawDebugSphere(WORLD, location, radius, 15, color, false, time)
#define DRAW_BOX(location, size, color) if (GAME_DEBUG) DrawDebugBox(WORLD, location, size, color)
#define DRAW_BOX_TIME(location, size, color, time) if (GAME_DEBUG) DrawDebugBox(WORLD, location, size, color, false, time)
#define DRAW_BOX_ROTATION(location, size, rotation, color) if (GAME_DEBUG) DrawDebugBox(WORLD, location, size, rotation, color)
//=========== DrawDebug ==============


//=========== Check ==============
#define CHECK_WITH_ARGS(ptr, msg, ...) checkf(ptr, TEXT(msg), __VA_ARGS__)
#define CHECK_OUT_OF_BOUNDS(number, min, max, msg, returnType) if(number < min || number >= max) { LOG_ERROR(true, "Index is invalid"); _Return_type_success_(); }
//=========== Check ==============


//=========== Random ==============
#define RANDOM_BOOL UKismetMathLibrary::RandomBool()
#define RANDOM_INT_RANGE(min, max) UKismetMathLibrary::RandomIntegerInRange(min, max)
#define RANDOM_FLOAT_RANGE(min, max) UKismetMathLibrary::RandomFloatInRange(min, max)
#define RANDOM_PERCENTAGE(value) (UKismetMathLibrary::RandomIntegerInRange(0, 100) <= value)
//=========== Random ==============


//=========== Math ==============
#define DEG_TO_RAD(angle) UKismetMathLibrary::DegreesToRadians(angle)
#define RAD_TO_DEG(angle) UKismetMathLibrary::RadiansToDegrees(angle)
#define COS(angle, radius) UKismetMathLibrary::Cos(angle) * radius
#define SIN(angle, radius) UKismetMathLibrary::Sin(angle) * radius
#define TAN(angle, radius) UKismetMathLibrary::Tan(angle) * radius
#define CLAMP(value, min, max) UKismetMathLibrary::Clamp(value, min, max)
#define POS_MODULO(_value, _mod, _temp) _temp = _value % _mod; \
	if (_temp < 0)	\
		_temp += _mod;\
_value = _temp; \
/**
 * Normalize value between 0 and 1 a value between min and max
 * @param value value to normalize
 * @param min minimum value
 * @param max maximum value
 */
#define NORMALIZE(value, min, max) (value - min) / (max - min)
//=========== Math ==============


//=========== UI ==============
#define GET_ENUM_STRING(enumValue) UEnum::GetValueAsString(enumValue)
#define SET_TEXT(textField, textStr) textField->SetText(textStr)
#define SET_TEXT_CLAMP(textField, textStr, clampLength) textField->SetText(FText::FromString(textStr.ToString().Mid(0, clampLength) + "..."))
#define SET_TEXT_STR(textField, text) textField->SetText(FText::FromString(text))
#define BIND_ACTION(ptr, e, owner, method) ptr->e().AddUniqueDynamic(this, &owner::method)
#define BIND_ACTION_PARAM(ptr, event, owner, method) ptr->event.AddUniqueDynamic(this, &owner::method)
#define BIND_ACTION_OTHER(ptr, event, owner, method) ptr->event().AddUniqueDynamic(ptr, &owner::method);
#define LOG_DEBUG(msg) LOG_COLOR(true, msg, PINK);
//=========== UI ==============


//=========== String ==============
#define TEXTSTR(msg) FText::FromString(msg)
#define EMPTY_TEXT FText::FromString("")
#define INTSTR(value) FString::FromInt(value)
#define FLOATSTR(value) FString::SanitizeFloat(value)
#define CONV_INT(integer, digital) UKismetTextLibrary::Conv_IntToText(integer, false, true, digital).ToString()
#define SPLIT(str, out, character) str.ParseIntoArray(out, TEXT(character), true);
//=========== String ==============

//=========== Convert ==============
#define STR_TO_INT(value) FCString::Atoi(*value)
//=========== Convert ==============

//=========== Line Trace ============
#define LINE_TRACE_SIMPLE(startLocation, endLocation, hitLayer, actorIgnore, result) UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), startLocation, endLocation ,hitLayer, true, actorIgnore, EDrawDebugTrace::None, result, true)
#define LINE_TRACE_SIMPLE_DEBUG(startLocation, endLocation, hitLayer, actorIgnore, result) UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), startLocation, endLocation ,hitLayer, true, actorIgnore, EDrawDebugTrace::ForOneFrame, result, true)
//=========== Line Trace ============
