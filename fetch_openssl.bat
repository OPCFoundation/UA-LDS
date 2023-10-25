@ECHO off
REM ****************************************************************************************************************
REM ** This script builds the debug version of OpenSSL for use by the ANSI C samples.
REM ** It requires that PERL be installed in the path and it must be run from a Visual Studio command line.
REM ****************************************************************************************************************
SETLOCAL

set ROOT=%~dp0
set SRCDIR=openssl-3.1.4
set GITTAG=openssl-3.1.4

set GIT=C:\Program Files\Git\bin\git.exe
IF NOT EXIST "%GIT%" SET GIT=C:\Program Files (x86)\Git\bin\git.exe

cd %ROOT%\stack
IF NOT EXIST %SRCDIR% "%GIT%" clone https://github.com/openssl/openssl.git %SRCDIR%
cd %SRCDIR%
"%GIT%" checkout tags/%GITTAG%

cd %ROOT%\stack
perl -pi.back -e "s/openssl-3.\?.\?/%SRCDIR%/" build_win32.bat
