@echo off 

cd projectn
mkdir build
cd build
cmake --build . --config RelWithDebInfo --target install_win --
