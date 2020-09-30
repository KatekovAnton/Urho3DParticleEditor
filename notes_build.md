# Build
## Mac

cd to urho directory
```
./script/cmake_xcode.sh ./build
```
then can open with xcode and run build


vs code:
options to build urho3d for mac:
```
{
    "cmake.configureSettings": {
        "URHO3D_PCH": false
    }
}
```
UrhoCommon.cmake line 130
```
set (INDIRECT_DEPS_EXE_LINKER_FLAGS "${INDIRECT_DEPS_EXE_LINKER_FLAGS} -Wl")
```



## Win
### Build urho
tried with 
- vs2017 (toolset v141 plus all updates because of bugs)
- vs2019 (toolset v142)


open vs2017 or vs2019 console
cd to urho directory
```
c:\Users\Anton\Documents\Projects\ProjectN\urho>script\cmake_vs2017.bat ./build -DURHO3D_OPENGL -DURHO3D_64BIT

script\cmake_vs2019.bat build -DURHO3D_OPENGL -DURHO3D_64BIT
cmake --build . --config Debug --target ALL_BUILD --
```



### Create CMD64 console
Tools->External tools
Name: CMD
command: %comspec%
params: /k %comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"


### extra
old issues was solved by
```

fixing clang compiler error in bullet:
#define BT_SHUFFLE(x, y, z, w) (((w) << 6 | (z) << 4 | (y) << 2 | (x)) & 0xff)

fixing xcode generator
UrhoCommon.cmake: line 123 - I commented it out.
```

