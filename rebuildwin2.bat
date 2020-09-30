@echo off 

cd urho\build

rem build urho for debug
cmake --build . --config Debug --target Urho3D --

cd ..\..
