@echo off

if exist build rd /s /q build
mkdir build
pushd build

cmake -T "v140_xp" -G "Visual Studio 14 2015" ..
set code=%errorlevel%
if not %code%==0 goto :error

cmake --build . --config Release
set code=%errorlevel%
if not %code%==0 goto :error

cmake --build . --config RelWithDebInfo
set code=%errorlevel%
if not %code%==0 goto :error

cmake --build . --config Debug
set code=%errorlevel%
if not %code%==0 goto :error

popd
goto :end

:error
echo cmake failed with exit code %code%
popd
exit /b %code%

:end