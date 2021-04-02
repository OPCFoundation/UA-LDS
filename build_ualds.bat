@ECHO off
REM ****************************************************************************************************************
REM ** This script builds the UA-LDS.
REM ** This must be run from a Visual Studio command line.
REM ****************************************************************************************************************
SETLOCAL

set SRCDIR=%~dp0
set INSTALLDIR=%~dp0
set SIGNTOOL=C:\Build\sign_output.bat
set VS_CONFIG=RelWithDebInfo

set GIT=C:\Program Files\Git\bin\git.exe
IF NOT EXIST "%GIT%" SET GIT=C:\Program Files (x86)\Git\bin\git.exe

ECHO STEP 1) Deleting Output Directories
REM IF EXIST %INSTALLDIR%\bin rmdir /s /q %INSTALLDIR%\bin

IF "%1"=="no-stack" GOTO noStack
ECHO STEP 2) Fetch OpenSSL
REM IF EXIST %INSTALLDIR%\stack\openssl rmdir /s /q %INSTALLDIR%\stack\openssl
CALL fetch_openssl.bat

ECHO STEP 3) Building Stack
cd %SRCDIR%\stack
CALL build_win32.bat
:noStack

ECHO STEP 4) Update Build Number
cd %SRCDIR%

IF NOT DEFINED BUILD_VERSION SET BUILD_VERSION=403
IF NOT DEFINED BUILD_NUMBER SET BUILD_NUMBER=1
ECHO Building Version: %BUILD_VERSION%.%BUILD_NUMBER%

perl -pi.bak -e "s/XXX/%BUILD_VERSION%/;" .\config.h 
perl -pi.bak -e "s/YYY/%BUILD_NUMBER%/;" .\config.h
del /Q *.bak

ECHO STEP 5) Building LDS
cd %SRCDIR%
CALL build_win32.bat

ECHO STEP 6) Sign the Binaries
IF EXIST "%SIGNTOOL%" CALL "%SIGNTOOL%" %INSTALLDIR%\build\bin\%VS_CONFIG%\*.dll /dual
IF EXIST "%SIGNTOOL%" CALL "%SIGNTOOL%" %INSTALLDIR%\build\bin\%VS_CONFIG%\*.exe /dual

ECHO *** ALL DONE ***
GOTO theEnd

:theEnd
ENDLOCAL