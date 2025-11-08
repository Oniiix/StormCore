#include "CustomEditorSettings.h"
#if WITH_EDITORONLY_DATA
#include "RobotHunter/Utils/DebugUtils.h"
#include "Serialization/JsonSerializer.h"

#define DEBUGGER_FIELD TEXT("Debugger")
#define AI_STATE_FIELD TEXT("AIState")
#define AI_TARGET_FIELD TEXT("AITarget")
#define AI_DAMAGE_FIELD TEXT("ThreatValue")
#define THREAT_VALUE_FIELD TEXT("AIDamage")
#define SEAMLESS_FIELD TEXT("Seamless")

void UCustomEditorSettings::InitSettings()
{
	// Create Json file if didn't exist
	if (!FPaths::FileExists(SETTINGS_FILE_PATH))
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetBoolField(DEBUGGER_FIELD, false);
		debugger = false;
		JsonObject->SetBoolField(AI_STATE_FIELD, false);
		AIState = false;
		JsonObject->SetBoolField(AI_TARGET_FIELD, false);
		AITarget = false;
		JsonObject->SetBoolField(THREAT_VALUE_FIELD, false);
		threatValue = false;
		JsonObject->SetBoolField(AI_DAMAGE_FIELD, false);
		AIDamage = false;
		JsonObject->SetBoolField(SEAMLESS_FIELD, false);
		seamless = false;
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else // Get & update saved values
	{
		bool Result;
		TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
		bool Success = JsonObject->TryGetBoolField(DEBUGGER_FIELD, Result);
		debugger = Success ? Result : false;
		Success = JsonObject->TryGetBoolField(AI_STATE_FIELD, Result);
		AIState = Success ? Result : false;
		Success = JsonObject->TryGetBoolField(AI_TARGET_FIELD, Result);
		AITarget = Success ? Result : false;
		Success = JsonObject->TryGetBoolField(THREAT_VALUE_FIELD, Result);
		threatValue = Success ? Result : false;
		Success = JsonObject->TryGetBoolField(AI_DAMAGE_FIELD, Result);
		AIDamage = Success ? Result : false;
		Success = JsonObject->TryGetBoolField(SEAMLESS_FIELD, Result);
		seamless = Success ? Result : false;
	}
}

bool UCustomEditorSettings::GetDebugger()
{
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(DEBUGGER_FIELD, Result);
	if (Success)
		debugger = Result;
	return debugger;
}

bool UCustomEditorSettings::GetAIState()
{
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(AI_STATE_FIELD, Result);
	if (Success)
		AIState = Result;
	return AIState;
}

bool UCustomEditorSettings::GetAITarget()
{
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(AI_TARGET_FIELD, Result);
	if (Success)
		AITarget = Result;
	return AITarget;
}

bool UCustomEditorSettings::GetAIDamage()
{
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(AI_DAMAGE_FIELD, Result);
	if (Success)
		AIDamage = Result;
	return AIDamage;
}

bool UCustomEditorSettings::GetThreatValue()
{
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(THREAT_VALUE_FIELD, Result);
	if (Success)
		threatValue = Result;
	return threatValue;
}

void UCustomEditorSettings::ToggleDebugger()
{
	// Read value
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(DEBUGGER_FIELD, Result);
	if (Success)
	{
		debugger = !Result;
		// Write value
		JsonObject->SetBoolField(DEBUGGER_FIELD, debugger);
		if (!debugger)
		{
			JsonObject->SetBoolField(AI_STATE_FIELD, false);
			AIState = false;
			JsonObject->SetBoolField(AI_TARGET_FIELD, false);
			AITarget = false;
			JsonObject->SetBoolField(THREAT_VALUE_FIELD, false);
			threatValue = false;
			JsonObject->SetBoolField(AI_DAMAGE_FIELD, false);
			AIDamage = false;
		}
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to get debugger field in json"));
}

void UCustomEditorSettings::ToggleAIState()
{
	// Read value
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(AI_STATE_FIELD, Result);
	if (Success)
	{
		AIState = !Result;
		onDebugAIState.Broadcast(AIState);
		// Saved value
		JsonObject->SetBoolField(AI_STATE_FIELD, AIState);
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to get AIState field in json"));
}

void UCustomEditorSettings::ToggleAITarget()
{
	// Read value
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(AI_TARGET_FIELD, Result);
	if (Success)
	{
		AITarget = !Result;
		// Write value
		JsonObject->SetBoolField(AI_TARGET_FIELD, AITarget);
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to get AITarget field in json"));
}

void UCustomEditorSettings::ToggleThreatValue()
{
	// Read value
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(THREAT_VALUE_FIELD, Result);
	if (Success)
	{
		threatValue = !Result;
		// Write value
		JsonObject->SetBoolField(THREAT_VALUE_FIELD, threatValue);
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to get ThreatValue field in json"));
}

void UCustomEditorSettings::ToggleAIDamage()
{
	// Read value
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(AI_DAMAGE_FIELD, Result);
	if (Success)
	{
		AIDamage = !Result;
		// Write value
		JsonObject->SetBoolField(AI_DAMAGE_FIELD, AIDamage);
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to get AIDamage field in json"));
}

bool UCustomEditorSettings::GetSeamless()
{
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(SEAMLESS_FIELD, Result);
	if (Success)
		seamless = Result;
	return seamless;
}

void UCustomEditorSettings::ToggleSeamless()
{
	// Read value
	bool Result;
	TSharedPtr<FJsonObject> JsonObject = ReadJson(SETTINGS_FILE_PATH);
	const bool Success = JsonObject->TryGetBoolField(SEAMLESS_FIELD, Result);
	if (Success)
	{
		seamless = !Result;
		// Write value
		JsonObject->SetBoolField(SEAMLESS_FIELD, seamless);
		WriteJSon(SETTINGS_FILE_PATH, JsonObject);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to get Seamless field in json"));
}

TSharedPtr<FJsonObject> UCustomEditorSettings::ReadJson(FString JsonPath)
{
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *JsonPath);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	TSharedPtr<FJsonValue> JsonResult;

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize Json string"));
		return nullptr;
	}

	return JsonObject;
}

void UCustomEditorSettings::WriteJSon(FString JsonPath, TSharedPtr<FJsonObject> JsonObject)
{
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString, 0);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		FFileHelper::SaveStringToFile(JsonString, *JsonPath);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize Json string"));
}
#endif