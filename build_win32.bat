@echo off
if exist build rd /s /q build
mkdir build
pushd build
cmake -T "v140_xp" -G "Visual Studio 14 2015" ..
cmake --build . --config Release
cmake --build . --config RelWithDebInfo
cmake --build . --config Debug
popd
