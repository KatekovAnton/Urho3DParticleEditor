#ifndef PROJECTN_FILE_UTILS_H
#define PROJECTN_FILE_UTILS_H
#pragma once

#include <Defines.h>

#ifndef NDEBUG
#ifdef PROJECTN_TARGET_OS_LINUX
#define GAME_USES_CURRENT_DIR_PATH
#endif
#endif

//String getFullFileName(const String& fileName);
//
//JSONValue loadJson(const String& fileName, bool checkBundle = true );
//
//bool saveJson(const String& fileName, const JSONValue& v);
//bool saveJson(const String& fileName, JSONValue&& v);

//template <typename T>
//typename std::enable_if_t<Has_toJson<T>::value, bool>
//inline saveJson(const String& fileName, const T& v){
//    return saveJson(fileName, v.toJson());
//}

//template <typename T>
//typename std::enable_if_t<Has_fromJson<T>::value, T>
//inline loadJson(const String& fileName){
//    T v; 
//    v.fromJson( loadJson(fileName, true) );
//    return v;
//}


#endif
