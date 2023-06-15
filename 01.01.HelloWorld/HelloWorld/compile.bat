@ECHO OFF
msbuild .\HelloWorld.sln /p:configuration=Release
copy x64\Release\HelloWorld.exe