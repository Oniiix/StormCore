#include "FileLogger.h"

#include "DebugUtils.h"

FString UFileLogger::currentLog = "";

void UFileLogger::WriteLog(const ELogSeverity _severity, const ELogCategory _category, const UClass* _clazz,  const FString& _msg)
{
#if WRITE_CUSTOM_LOG
	const FDateTime dateTime = FDateTime::Now();
	if (currentLog.IsEmpty())	{
		FString _path = FPaths::GetProjectFilePath();		
		int lastIndex;
		_path.FindLastChar ('/', lastIndex);
		_path = _path.Mid(0, lastIndex);
		_path += "/CustomLog/" + dateTime.ToString() + ".txt";
		currentLog = _path;
	}
	
		
	//[Hour] [Severity] [Type] [class] msg
	FString _format = "[{0}] [{1}] [{2}] [{3}] {4}";
	_format = FORMAT(_format, dateTime.ToString(*FString("%H:%M:%S")), Format(GET_ENUM_STRING(_severity)),Format(GET_ENUM_STRING(_category)), _clazz ? _clazz->GetName() : "nullptr", _msg);
	FFileHelper::SaveStringToFile(_format + "\n", *currentLog,FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
}

FString UFileLogger::GetClassName(const TObjectPtr<UClass>& _class)
{
	const FString _name = _class->GetName();
	int _first;
	int _last;
	_name.FindChar('_', _first);
	_name.FindLastChar('_', _last);

	return _name.Mid(_first + 1, _last -2 - _first + 1);
}

FString UFileLogger::Format(const FString& _enum)
{
	int _last;
	_enum.FindLastChar(':', _last);
	return _enum.Mid(_last+1);
}
