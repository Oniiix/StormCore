#pragma once

#include "CoreMinimal.h"
#include "CustomEditorSettings.generated.h"

#define SETTINGS_FILE_PATH FPaths::ProjectSavedDir() + "CustomEditor/Settings/Settings.json"

/**
 * The settings of CustomEditor module
 */
UCLASS()
class CUSTOMEDITOR_API UCustomEditorSettings : public UClass
{
	GENERATED_BODY()
#if WITH_EDITORONLY_DATA

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugAIState, bool, _value);
	FOnDebugAIState onDebugAIState;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugAITarget, bool, _value);
	FOnDebugAITarget onDebugAITarget;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugAIThreat, bool, _value);
	FOnDebugAIThreat onDebugAIThreat;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugAIDamage, bool, _value);
	FOnDebugAIDamage onDebugAIDamage;

public:
	void InitSettings();

	FORCEINLINE FOnDebugAIState& OnDebugAIState() { return onDebugAIState; }
	FORCEINLINE FOnDebugAITarget& OnDebugAITarget() { return onDebugAITarget; }
	FORCEINLINE FOnDebugAIThreat& OnDebugAIThreat() { return onDebugAIThreat; }
	FORCEINLINE FOnDebugAIDamage& OnDebugAIDamage() { return onDebugAIDamage; }

#pragma region Combat
	UPROPERTY()
	bool debugger;
	UPROPERTY()
	bool AIState;
	UPROPERTY()
	bool AITarget;
	UPROPERTY()
	bool threatValue;
	UPROPERTY()
	bool AIDamage;

	bool GetDebugger();
	bool GetAIState();
	bool GetAITarget();
	bool GetAIDamage();
	bool GetThreatValue();

	void ToggleDebugger();
	void ToggleAIState();
	void ToggleAITarget();
	void ToggleAIDamage();
	void ToggleThreatValue();
#pragma endregion

#pragma region Online
	UPROPERTY(EditAnywhere)
	bool seamless;

	bool GetSeamless();
	void ToggleSeamless();
#pragma endregion

private:
	TSharedPtr<FJsonObject> ReadJson(FString JsonPath);
	void WriteJSon(FString JsonPath, TSharedPtr<FJsonObject> JsonObject);
#endif
};