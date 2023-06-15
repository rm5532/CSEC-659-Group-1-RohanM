@ECHO OFF

REM @cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tcimplant.cpp /link /OUT:implant.exe /SUBSYSTEM:CONSOLE /MACHINE:x64
REM @del *.obj
msbuild CreateProcess/CreateProcess.sln /p:Platform=x64 /p:Configuration=Release
copy /y CreateProcess\x64\Release\CreateProcess.exe