@echo off
del *.sln
rmdir /s /q ".vs", "Binaries", "DerivedDataCache", "Intermediate", "Saved/Autosaves", "CustomLog", "Saved/Collections", "Saved/Crashes", "Saved/Logs", "Saved/UnrealBuildTool", "Saved\Config\CrashReportClient"
del /q /f .vsconfig, MySolution.sln

echo All files are deleted successfully.

set MyUVS="C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
set Build="C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat"

rem change Transformation to your own project name
set MyFullPath="%cd%\RobotHunter"


%MyUVS% /projectfiles %MyFullPath%.uproject 

%MyFullPath%.sln

%Build% RobotHunterEditor Win64 Development -Project="%MyFullPath%\RobotHunter.uproject" -WaitMutex -FromMsBuild"

pause
