@ECHO off
REM ****************************************************************************************************************
REM ** This script builds the UA-LDS.
REM ** This must be run from a Visual Studio command line.
REM ****************************************************************************************************************
SETLOCAL

set SRCDIR=%~dp0
set INSTALLDIR=%~dp0
set GIT=C:\Program Files (x86)\Git\bin\git.exe
set SIGNTOOL=C:\Build\sign_output.bat

IF "%1"=="no-clean" GOTO noClean
ECHO STEP 1) Deleting Output Directories
IF EXIST %INSTALLDIR%\bin rmdir /s /q %INSTALLDIR%\bin

IF "%1"=="no-stack" GOTO noStack
ECHO STEP 2) Fetch OpenSSL
REM IF EXIST %INSTALLDIR%\stack\openssl rmdir /s /q %INSTALLDIR%\stack\openssl
CALL fetch_openssl.bat

ECHO STEP 3) Building Stack
cd %SRCDIR%\stack
CALL build_win32.bat
:noStack

ECHO STEP 4) Building LDS
cd %SRCDIR%
IF %BUILD_NUMBER% EQU "" SET BUILD_NUMBER=0
IF %BUILD_NUMBER% GTR 0 ECHO #define BUILD_NUMBER %BUILD_NUMBER% > buildversion.h
msbuild ualds.sln /p:Configuration=Release

ECHO STEP 5) Sign the Binaries
IF EXIST "%SIGNTOOL%" CALL "%SIGNTOOL%" %INSTALLDIR%\bin\Release\*.exe /sha1

ECHO *** ALL DONE ***
GOTO theEnd

:theEnd
ENDLOCAL