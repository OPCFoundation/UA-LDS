@ECHO off
REM ****************************************************************************************************************
REM ** This script builds the UA-LDS.
REM ** This must be run from a Visual Studio command line.
REM ****************************************************************************************************************
SETLOCAL

set SRCDIR=%~dp0
set INSTALLDIR=%~dp0
set GIT=C:\Program Files\Git\bin\git.exe
set SIGNTOOL=C:\Build\sign_output.bat

IF "%1"=="no-clean" GOTO noClean
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
IF NOT DEFINED BUILD_VERSION SET BUILD_VERSION=370
IF NOT DEFINED BUILD_NUMBER  SET BUILD_NUMBER=1
ECHO Building Version: %BUILD_VERSION%.%BUILD_NUMBER
IF %BUILD_NUMBER% GTR 0 ECHO #define BUILD_NUMBER %BUILD_NUMBER% > buildversion.h

ECHO STEP 4) Building LDS
cd %SRCDIR%
REM msbuild ualds.sln /p:Configuration=Release
build_win32.bat

ECHO STEP 5) Sign the Binaries
#IF EXIST "%SIGNTOOL%" CALL "%SIGNTOOL%" %INSTALLDIR%\bin\Release\*.exe /dual
IF EXIST "%SIGNTOOL%" CALL "%SIGNTOOL%" %INSTALLDIR%\build\bin\Release\*.exe /dual

ECHO *** ALL DONE ***
GOTO theEnd

:theEnd
ENDLOCAL