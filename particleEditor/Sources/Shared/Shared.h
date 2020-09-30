#ifndef SharedDefines_h
#define SharedDefines_h

#ifdef __APPLE__
    #ifdef TARGET_OS_MAC
        #define PROJECTN_SHARED_EXPORT __attribute__((visibility("default")))
    #endif
#elif __GNUC__
    #define PROJECTN_SHARED_EXPORT __attribute__((dllexport))
#elif _MSC_VER
    #ifdef SHARED_LIBRARY
        #define PROJECTN_SHARED_EXPORT __declspec(dllexport)
    #else
        #define PROJECTN_SHARED_EXPORT __declspec(dllimport)
    #endif
#else
    #error "WRONG PROJET SETUP"
#endif

#include <Shared/Model/Configs/ConfigScene.h>
#include <Shared/Model/Configs/ConfigScenePathMap.h>
#include <Shared/Model/Configs/ConfigPlayer.h>

#include <Shared/Model/Objects/ActorModel.h>
#include <Shared/Model/Objects/ActorController.h>

#include <Shared/GameModel.h>

#include <Shared/Presentation/Presentation.h>

#endif
