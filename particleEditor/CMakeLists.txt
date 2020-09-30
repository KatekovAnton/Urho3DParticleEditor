cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Urho3DParticleEditor)
set(TARGET_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)


IF(APPLE)
    SET (PROJECT_OS_MACOS TRUE BOOL INTERNAL)
    option(URHO3D_64BIT "Use URHO3D_64BIT" ON)
    add_definitions(-DPROJECT_TARGET_OS_MAC )
ENDIF (APPLE)
IF(WIN32)
    SET (PROJECT_OS_WIN32 TRUE BOOL INTERNAL)
    add_definitions(-DPROJECT_TARGET_OS_WIN )
   # find_package(Threads REQUIRED)
ENDIF (WIN32)
if(UNIX AND NOT APPLE) 
    SET (PROJECT_OS_LINUX TRUE BOOL INTERNAL)
    add_definitions(-DPROJECT_TARGET_OS_LINUX )
ENDIF (UNIX AND NOT APPLE)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DURHO3D_LOGGING)
    add_definitions(-DDEBUG)
endif()


set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../urho/CMake/Modules)
message("CMAKE_SOURCE_DIR is " ${CMAKE_SOURCE_DIR})

set (URHO3D_HOME ${CMAKE_SOURCE_DIR}/../urho/build)
message("URHO3D_HOME is " ${URHO3D_HOME})



set (URHO3D_COMPILE_RESULT TRUE)
set (URHO3D_LIB_TYPE STATIC)
set (URHO3D_INCLUDE_DIRS
    ${URHO3D_HOME}/include 
    ${URHO3D_HOME}/include/Urho3D 
    ${URHO3D_HOME}/include/Urho3D/ThirdParty 
    ${URHO3D_HOME}/include/Urho3D/ThirdParty/Bullet
    ${URHO3D_HOME}/include/Urho3D/ThirdParty/SDL
)
set (URHO3D_LIBRARIES
    ${URHO3D_HOME}/lib 
)
   
set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
set (URHO3D_OPENGL TRUE)

include (UrhoCommon)


set(SRC_DIR "${PROJECT_SOURCE_DIR}/Sources")
set(VENDORS_DIR "${PROJECT_SOURCE_DIR}/Vendors")
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resources)
set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../urho/bin/Data)
set(COREDATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../urho/bin/CoreData)

include_directories("${URHO3D_INCLUDE_DIRS}") 
include_directories("${SRC_DIR}")
include_directories("${VENDORS_DIR}/libJSON")
include_directories("${PROJECT_SOURCE_DIR}/Sources/Sys")
include_directories("${PROJECT_SOURCE_DIR}/Sources/Utils")
include_directories("${PROJECT_SOURCE_DIR}/Sources/Shared/Utils/zip")


file(GLOB_RECURSE PROJECT_SOURCES 
    ${SRC_DIR}/*.*
    ${SRC_DIR}/Game/*.*
    ${SRC_DIR}/Game/UI/*.*
    ${SRC_DIR}/Game/UI/SceneComponents/*.*
    ${SRC_DIR}/Shared/*.*
    ${SRC_DIR}/Shared/Engine/*.*
    ${SRC_DIR}/Shared/Model/*.*
    ${SRC_DIR}/Shared/Model/World/*.*
    ${SRC_DIR}/Shared/Model/Commands/*.*
    ${SRC_DIR}/Shared/Model/CommandTransactions/*.*
    ${SRC_DIR}/Shared/Model/CommandNotifications/*.*
    ${SRC_DIR}/Shared/Loader/*.*
    ${SRC_DIR}/Shared/DataStructures/*.*
    ${SRC_DIR}/Shared/Model/Commands/*.*
    ${SRC_DIR}/Shared/Model/Objects/*.*
    ${SRC_DIR}/Shared/Model/ObjectPrameters/*.*
    ${SRC_DIR}/Shared/Model/Configs/*.*
    ${SRC_DIR}/Shared/Utils/*.*
    ${SRC_DIR}/Shared/Utils/Hex/*.*
    ${SRC_DIR}/Shared/Sys/*.*
    ${SRC_DIR}/Shared/UI/*.*
    ${SRC_DIR}/Tools/UrhoEditor/*.*
    ${SRC_DIR}/Tools/DebugUI/*.*
    ${SRC_DIR}/UI/*.*
    ${VENDORS_DIR}/libJSON/*.cpp
    ${VENDORS_DIR}/libJSON/*.h
)



if (PROJECT_OS_MACOS)
    
endif()
if (PROJECT_OS_WIN32)
    list (APPEND PROJECT_TEST_SOURCES "${PROJECT_SOURCE_DIR}/Platform/Win/getopt.c")
    include_directories("${PROJECT_SOURCE_DIR}/Sources/Shared/Utils/pthread")
    include_directories("${PROJECT_SOURCE_DIR}/Platform/Win")
    
    add_definitions(-DHAVE_STRUCT_TIMESPEC )
    
endif()


if(CMAKE_GENERATOR STREQUAL Xcode)

    # add resources to copy
    set_source_files_properties(${RESOURCES_DIR} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(${DATA_DIR} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(${COREDATA_DIR} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
        ${RESOURCES_DIR}
        ${DATA_DIR}
        ${COREDATA_DIR}
        ${PROJECT_SOURCES})

    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/Sources/Game/Mac/GameInfo.plist)

elseif(PROJECT_OS_MACOS)

    # TODO: 2Anton rsync быстрее копирования
    set(RESULT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT_NAME}.app/Contents)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resources DESTINATION ${RESULT_FOLDER})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../urho/bin/Data DESTINATION ${RESULT_FOLDER})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../urho/bin/CoreData DESTINATION ${RESULT_FOLDER})


    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
        ${PROJECT_SOURCES})

        set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/Sources/Game/Mac/GameInfo.plist)

    add_custom_command(TARGET ${PROJECT_NAME} 
        POST_BUILD COMMAND 
        ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../../Frameworks/"
        $<TARGET_FILE:${PROJECT_NAME}>)

elseif(WIN32)

    add_executable(${PROJECT_NAME} WIN32 
        ${PROJECT_SOURCES})
        
else ()

    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND rsync -qri ../../Resources/ ./bin/)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND rsync -qri ${URHO3D_HOME}/bin/Data/ ./bin/Data/)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND rsync -qri ${URHO3D_HOME}/bin/CoreData/ ./bin/CoreData/)
        
endif()



if (PROJECT_OS_MACOS)

    list(APPEND SRE_INCLUDE_DIRS /System/Library/Frameworks)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-reinterpret-base-class -Wno-inconsistent-missing-override)
    
    target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/../urho/build/lib/libUrho3D.a)
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries(${PROJECT_NAME} "-framework AppKit")
    target_link_libraries(${PROJECT_NAME} "-framework Carbon")
    target_link_libraries(${PROJECT_NAME} "-framework ForceFeedback")
    target_link_libraries(${PROJECT_NAME} "-framework SystemConfiguration")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework GLKit")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework Security")
    target_link_libraries(${PROJECT_NAME} "-framework AudioToolbox")
    target_link_libraries(${PROJECT_NAME} "-framework CoreAudio")
    target_link_libraries(${PROJECT_NAME} "-lbsm")
    target_link_libraries(${PROJECT_NAME} "-lc")
    target_link_libraries(${PROJECT_NAME} libiconv.dylib)
    
endif()
if (PROJECT_OS_LINUX)

    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "clang++")
    
    #add_definitions(-DSSE)
    
    #SSE стоит включать в релизе. в дебаге тормозит
    option(SSE "Compile with support for SSE instructions." ON)
    option(SSE2 "Compile with support for SSE2 instructions." ON)

    if (SSE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse -mfpmath=sse")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -mfpmath=sse")
    endif()
    if (SSE41)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
    endif()
    if (SSE42)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
    endif()

    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fno-omit-frame-pointer -fcolor-diagnostics -std=c++${CMAKE_CXX_STANDARD}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD} -Wno-undefined-var-template")

    target_link_libraries (${PROJECT_NAME} m c dl z GL pthread X11 Xext Xcursor Xinerama Xi Xrandr Xrender Xss Xxf86vm ${URHO3D_HOME}/lib/libUrho3D.a)
    
endif()
if (PROJECT_OS_WIN32)

    target_link_libraries(${PROJECT_NAME} dbghelp)
    target_link_libraries(${PROJECT_NAME} debug ${PROJECT_SOURCE_DIR}/../urho/build/lib/Urho3D_d.lib )
    target_link_libraries(${PROJECT_NAME} optimized ${PROJECT_SOURCE_DIR}/../urho/build/lib/Urho3D.lib )
    target_link_libraries(${PROJECT_NAME} general ${PROJECT_SOURCE_DIR}/../urho/build/lib/Urho3D.lib )

    set(LIBS opengl32 gdi32 ole32 user32 winmm imm32 version setupapi)
    target_link_libraries(${PROJECT_NAME} ${LIBS})

endif()



if (NOT PROJECT_OS_LINUX)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Configure Tests
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

include(CTest)



# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Tests
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

file(GLOB_RECURSE PROJECT_TEST_SOURCES 
${PROJECT_SOURCE_DIR}/Tests/*.*
${PROJECT_SOURCE_DIR}/Tests/Basement/*.*

    ${SRC_DIR}/Shared/*.*
    ${SRC_DIR}/Shared/Engine/*.*
    ${SRC_DIR}/Shared/Model/*.*
    ${SRC_DIR}/Shared/Model/World/*.*
    ${SRC_DIR}/Shared/Model/Commands/*.*
    ${SRC_DIR}/Shared/Model/CommandTransactions/*.*
    ${SRC_DIR}/Shared/Model/CommandNotifications/*.*
    ${SRC_DIR}/Shared/Loader/*.*
    ${SRC_DIR}/Shared/DataStructures/*.*
    ${SRC_DIR}/Shared/Model/Commands/*.*
    ${SRC_DIR}/Shared/Model/Objects/*.*
    ${SRC_DIR}/Shared/Model/ObjectPrameters/*.*
    ${SRC_DIR}/Shared/Model/Configs/*.*
    ${SRC_DIR}/Shared/Utils/*.*
    ${SRC_DIR}/Shared/Utils/Hex/*.*
    ${SRC_DIR}/Shared/Sys/*.*
    ${SRC_DIR}/Shared/UI/*.*
    ${SRC_DIR}/Tools/UrhoEditor/*.*
    ${SRC_DIR}/Tools/DebugUI/*.*
    ${SRC_DIR}/UI/*.*

    ${VENDORS_DIR}/libJSON/*.cpp
    ${VENDORS_DIR}/libJSON/*.h
)

if (PROJECT_OS_MACOS)
    
endif()
if (PROJECT_OS_WIN32)
    list (APPEND PROJECT_TEST_SOURCES "${PROJECT_SOURCE_DIR}/Platform/Win/getopt.c")
    include_directories("${PROJECT_SOURCE_DIR}/Sources/Shared/Utils/pthread")
    include_directories("${PROJECT_SOURCE_DIR}/Platform/Win")
    
    add_definitions(-DHAVE_STRUCT_TIMESPEC )
endif()


add_executable(${PROJECT_NAME}Tests ${PROJECT_TEST_SOURCES})
add_dependencies(${PROJECT_NAME}Tests gtest)



include_directories(${CMAKE_CURRENT_BINARY_DIR}/../googletest-src/googletest/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../googletest-src/googlemock/include)



target_link_libraries(${PROJECT_NAME}Tests gtest_main)
target_link_libraries(${PROJECT_NAME}Tests gmock_main)



if (PROJECT_OS_MACOS)

    list(APPEND SRE_INCLUDE_DIRS /System/Library/Frameworks)
    target_compile_options(${PROJECT_NAME}Tests PRIVATE -Wno-reinterpret-base-class -Wno-inconsistent-missing-override)
    
    target_link_libraries(${PROJECT_NAME}Tests ${PROJECT_SOURCE_DIR}/../urho/build/lib/libUrho3D.a)
    target_link_libraries(${PROJECT_NAME}Tests "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME}Tests "-framework CoreVideo")
    target_link_libraries(${PROJECT_NAME}Tests "-framework AppKit")
    target_link_libraries(${PROJECT_NAME}Tests "-framework Carbon")
    target_link_libraries(${PROJECT_NAME}Tests "-framework ForceFeedback")
    target_link_libraries(${PROJECT_NAME}Tests "-framework SystemConfiguration")
    target_link_libraries(${PROJECT_NAME}Tests "-framework IOKit")
    target_link_libraries(${PROJECT_NAME}Tests "-framework GLKit")
    target_link_libraries(${PROJECT_NAME}Tests "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME}Tests "-framework Security")
    target_link_libraries(${PROJECT_NAME}Tests "-framework AudioToolbox")
    target_link_libraries(${PROJECT_NAME}Tests "-framework CoreAudio")
    target_link_libraries(${PROJECT_NAME}Tests "-lbsm")
    target_link_libraries(${PROJECT_NAME}Tests "-lc")
    target_link_libraries(${PROJECT_NAME}Tests libiconv.dylib)
    
endif()
if (PROJECT_OS_WIN32)

    target_link_libraries(${PROJECT_NAME}Tests dbghelp)
    target_link_libraries(${PROJECT_NAME}Tests debug ${PROJECT_SOURCE_DIR}/../urho/build/lib/Urho3D_d.lib )
    target_link_libraries(${PROJECT_NAME}Tests optimized ${PROJECT_SOURCE_DIR}/../urho/build/lib/Urho3D.lib )
    target_link_libraries(${PROJECT_NAME}Tests general ${PROJECT_SOURCE_DIR}/../urho/build/lib/Urho3D.lib )
    
    target_link_libraries(${PROJECT_NAME}Tests ${LIBS})

endif()

endif()




# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# INSTALL / DEPLOY
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

option(DUMP_SYMS "Dump symbols for crash reports" OFF)
# message(${CMAKE_BUILD_TYPE})
if (DUMP_SYMS AND CMAKE_BUILD_TYPE MATCHES Debug|RelWithDebInfo)
    set(PROJ_DUMP_SYMS ON)
else()
    set(PROJ_DUMP_SYMS OFF)
endif()
message(${PROJ_DUMP_SYMS})

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)
set(sanitize FALSE)





install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(PROJECT_OS_MACOS)
    # install(FILES cmake/package.sh cmake/bg_dmg.png DESTINATION bin)
ENDIF (PROJECT_OS_MACOS)

if(PROJECT_OS_WIN32)
    
    install(DIRECTORY ${RESOURCES_DIR} DESTINATION bin)
    install(DIRECTORY ${DATA_DIR} DESTINATION bin)
    install(DIRECTORY ${COREDATA_DIR} DESTINATION bin)
    
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION pdb OPTIONAL)
    
    # list all libraries to track crashes
    install(FILES 
        "${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT_NAME}.exe"
    DESTINATION pdb OPTIONAL)

    # list all libraries that should be delivered to user
    # install(FILES
    # ...
    # DESTINATION bin)

ENDIF(PROJECT_OS_WIN32)




if (PROJECT_OS_MACOS)
    if (CODESIGN_IDENTITY MATCHES None)
        set (CODESIGN_OPT "") 
    else()
        set (CODESIGN_OPT "-codesign=\"${CODESIGN_IDENTITY}\"")
    endif()

    add_custom_target(
        install_mac
        COMMAND rm -rf "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.app"
        COMMAND "${CMAKE_COMMAND}" --build . --target install
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

    if (PROJ_DUMP_SYMS)
        add_custom_target(
            deploy_mac
            COMMAND "${CMAKE_COMMAND}" --build . --target install_mac
            COMMAND rm -rf "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}.dSYM"
            # COMMAND "${PROJECT_SOURCE_DIR}/deploy/bundle_mac.sh" "${CMAKE_SOURCE_DIR}/ThirdParty/crashpad_mac/bin/symupload" "${CMAKE_INSTALL_PREFIX}/sym" "${BUGSPLAT_DBNAME}" "${BUGSPLAT_APPNAME}" "${VERSION_NUMBER}"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    else()
        add_custom_target(
            deploy_mac
            COMMAND "${CMAKE_COMMAND}" --build . --target install_mac
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    endif ()
endif ()
if(PROJECT_OS_WIN32)

    add_custom_target(
        install_win
        COMMAND "${CMAKE_COMMAND}" --build . --config ${CMAKE_BUILD_TYPE} --target install
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    if (PROJ_DUMP_SYMS)
        set(PROJ_PDB_FOLDER "${CMAKE_INSTALL_PREFIX}/pdb")
        add_custom_target(
            deploy_win
            COMMAND "${CMAKE_COMMAND}" --build . --config ${CMAKE_BUILD_TYPE} --target install_win
            # Deploy pdb
            # COMMAND "${CMAKE_INSTALL_PREFIX}/pdb/SendPdbs.exe" /v "${BUGSPLAT_FULL_VERSION}" /a ${BUGSPLAT_APPNAME} /u projectb@gmail.com /b crashes_projecnt_com /p ${BUGSPLAT_PASS} /d ${PROJ_PDB_FOLDER} /verbose /f
        )
    else()
        add_custom_target(
            deploy_win
            COMMAND "${CMAKE_COMMAND}" --build . --config ${CMAKE_BUILD_TYPE} --target install_win
        )
    endif ()

endif ()
