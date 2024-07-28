
SETLOCAL EnableDelayedExpansion
@echo off
cls
del build\*.o

SET "debug="
SET "cwDWARF="
if "%1" equ "-d" SET "debug=-debug -map=^"%appdata%\Dolphin Emulator\Maps\RMCP01.map^" -readelf=^"C:\MinGW\bin\readelf.exe^""
if "%1" equ "-d" SET "cwDWARF=-g"


:: Destination (change as necessary)
SET "SOURCE=RetroRewind6"
SET "RIIVO=%appdata%\Dolphin Emulator\Load\Riivolution\RetroRewind6"
SET "ENGINE=..\Pulsar\KamekInclude"
echo %ENGINE%


:: CPP compilation settings
SET CC="../Pulsar/cw/mwcceppc.exe"
SET CFLAGS=-I- -i "../Pulsar/KamekInclude" -i "../Pulsar/GameSource" -i "../Pulsar/GameSource/game" -i "../Pulsar/GameSource/core" -i "../Pulsar/PulsarEngine" -i "../Pulsar" -i code^
  -opt all -inline auto -enum int -proc gekko -fp hard -sdata 0 -sdata2 0 -maxerrors 1 -func_align 4 %cwDWARF%
SET DEFINE=

:: CPP Sources
SET CPPFILES=
for /R "../Pulsar/PulsarEngine" %%f in (*.cpp) do SET "CPPFILES=%%f !CPPFILES!"
for /R code %%f in (*.cpp) do SET "CPPFILES=%%f !CPPFILES!"

:: Compile CPP
%CC% %CFLAGS% -c -o "build/kamek.o" "%ENGINE%\kamek.cpp"

SET OBJECTS=
FOR %%H IN (%CPPFILES%) DO (
    ::echo "Compiling %%H..."
    %CC% %CFLAGS% %DEFINE% -c -o "build/%%~nH.o" "%%H"
    SET "OBJECTS=build/%%~nH.o !OBJECTS!"
)

:: Link
echo Linking... %time%
"../Pulsar/KamekLinker/kamek" "build/kamek.o" %OBJECTS% %debug% -dynamic -externals="../Pulsar/GameSource/symbols.txt" -versions="../Pulsar/GameSource/versions.txt" -output-combined=build\Code.pul

if %ErrorLevel% equ 0 (
    xcopy /Y build\*.pul "%RIIVO%\Binaries" >nul
    echo Binaries copied
)

:end
ENDLOCAL