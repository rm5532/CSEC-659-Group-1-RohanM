@ECHO OFF

msbuild FindPidByName\FindPidByName.sln /p:Platform=x64 /p:Configuration=Release
copy /y FindPidByName\x64\Release\FindPidByName.exe