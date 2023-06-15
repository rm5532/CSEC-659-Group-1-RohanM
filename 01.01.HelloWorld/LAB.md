# **01.01.HelloWorld**

Getting started with your first lab!

## **OBJECTIVES**

* Ability to build a VS project in the IDE.
* Ability to build a VS project from the CLI.
* Update VS project settings to match desired configuration.

## **STEPS**

### **1). Creating the Project**

Create an empty C++ Project in Visual Studio. 

Name of the project: `HelloWorld`

Right Click `Source Files` -> Add -> 'Add New Item`

Filename: `main.cpp`

Add the following contents to main.cpp:

```cpp
#include <windows.h>
#include <stdio.h>
int main(void) {
  printf("Hello World!\n");
  getchar();
  printf("Goodbye!\n");
  return 0;
}
```

Compile and run the project in Visual Studio.

Create a compile.bat with following contents.

```bat
@ECHO OFF
msbuild .\HelloWorld.sln /p:configuration=Release
copy x64\Release\HelloWorld.exe
```

### **2). Updating Configuration Settings**

Open the Project Properties:

* Select the Project within the Solution Explorer.
* Right-Click -> Properties (or press alt+enter)

--

Configuration: **Debug**:

* Platform: **All Properties**
* General -> Windows SDK Version: **10.0.18362.0** (this is SDK 10.1)
* C/C++ -> General -> Warnings: **/W4**

Configuration: **Release**:

* Platform: **All Properties**
* General -> Windows SDK Version: **10.0.18362.0** (this is SDK 10.1)
* C/C++ -> General -> Runtime Library: **Multi-threaded /MT** (as known as static linking)
* Linker -> Debugger -> Generate Debug Info: **No**
