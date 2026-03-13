@echo off
if exist build rd /s /q build
mkdir build
pushd build
cmake -T "v143" -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cmake --build . --config Debug
popd
