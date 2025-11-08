#pragma once

#include "CoreMinimal.h"
#include "FileLogger.generated.h"

UENUM()
enum class ELogCategory : uint8
{
	None,
	UI,
	AI,
	GPE,
	CCC,
	SubSystem
};

UENUM()
enum class ELogSeverity : uint8
{
	None,
	INFO,
	ERROR,
	WARNING
};

#define WRITE_CUSTOM_LOG true

UCLASS(HideDropdown, Abstract)
class ROBOTHUNTER_API UFileLogger : public UObject
{
	GENERATED_BODY()
	
	static FString currentLog;
	
public:
		/**
	 * Write string in log file
	 * @param _severity Severity of log (info, warning, error)
	 * @param _category Category of log
	 * @param _clazz class from executing
	 * @param _msg string to print in log file
	 */
	static void WriteLog(ELogSeverity _severity, ELogCategory _category, const UClass* _clazz, const FString& _msg);

	/**
 * Get the class name in c++ native
 * @param _class Class to format
 * @return String formated (BP_Main_C -> Main)
 */
	static FString GetClassName(const TObjectPtr<UClass>& _class);
	
	
	/**
	 * Format the enum string to get only enum name
	 * @param _enum String enum (GET_ENUM_TO_STRING(enum))
	 * @return String formated (ELogSeverity::INFO -> INFO)
	 */
	static FString Format(const FString& _enum);

};
