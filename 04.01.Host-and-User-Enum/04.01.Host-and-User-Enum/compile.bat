@ECHO OFF

msbuild Host-and-User-Enum/Host-and-User-Enum.sln /p:Platform=x64 /p:Configuration=Release
copy /y Host-and-User-Enum\x64\Release\Host-and-User-Enum.exe