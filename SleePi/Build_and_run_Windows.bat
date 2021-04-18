@echo off
cd %~dp0
if not exist "build" mkdir build
cd build
cmake -G "MinGW Makefiles" ../
cmake --build .
cd ../bin
SleePi.exe