@ECHO off
REM ****************************************************************************************************************
REM ** This script builds the debug version of OpenSSL for use by the ANSI C samples.
REM ** It requires that PERL be installed in the path and it must be run from a Visual Studio command line.
REM ****************************************************************************************************************
SETLOCAL

set SRCDIR=openssl-1.0.2l
set GITTAG=OpenSSL_1_0_2l
set GIT=C:\Program Files (x86)\Git\bin\git.exe

cd %~dp0\stack
IF NOT EXIST %SRCDIR% "%GIT%" clone https://github.com/openssl/openssl.git %SRCDIR%
cd %SRCDIR%
"%GIT%" checkout tags/%GITTAG%

cd %~dp0\stack
perl -pi.back -e "s/openssl-1.0.\?\?/%SRCDIR%/" build_win32.bat

cd %~dp0