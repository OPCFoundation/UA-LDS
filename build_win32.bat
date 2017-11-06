@echo off
if exist build rd /s /q build
mkdir build
pushd build
cmake -T "v120_xp" -G "Visual Studio 12 2013" ..
cmake --build . --config Release
cmake --build . --config RelWithDebInfo
cmake --build . --config Debug
popd
