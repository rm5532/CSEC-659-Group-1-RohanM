@ECHO OFF

msbuild EnumProcesses\EnumProcesses.sln /p:Platform=x64 /p:Configuration=Release
copy /y EnumProcesses\x64\Release\EnumProcesses.exe