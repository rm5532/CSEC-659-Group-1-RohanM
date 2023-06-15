# **01.04.Stack Strings**

## **OBJECTIVES**

* Ability to use Stack Strings to hide strings in PE files. 

## **STEPS**

Open VS Native Tool Command Prompt for VS2017.

Compile the program.

`compile.bat`

Run the strings.exe on the PE. Notice that the strings `evil.exe` and `evilW.exe` are not included in the output.

```bat
strings.exe main.exe |findstr evil.exe
```

Run strings searching for evil2.exe

```bat
strings.exe main.exe |findstr evil2.exe
```

Notice that `evil2.exe` is included in the output.
