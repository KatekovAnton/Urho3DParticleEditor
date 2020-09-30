@echo off 

cd urho\build

rem build urho for debug
cmake --build . --config RelWithDebInfo --target Urho3D --

cd ..\..
