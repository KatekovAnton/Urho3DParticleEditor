@echo off 

cd urho

echo Configuring urho....

rem generate urho project
script\cmake_vs2019.bat build -DURHO3D_OPENGL -DURHO3D_64BIT
