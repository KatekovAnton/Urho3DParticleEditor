#include "FileUtils.h"
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>


//String getFullFileName(const String& fileName){
//#ifdef GAME_USES_CURRENT_DIR_PATH
//    return fileName;
//#else
//    auto fileSystem = GetAppContext()->GetSubsystem<FileSystem>();
//    auto appDir = fileSystem->GetAppPreferencesDir("projectn", "data");
//    return appDir + fileName;
//#endif
//}
//
//JSONValue loadJson(const String& fileName, bool checkBundle) {
//    
//    JSONFile jsonFile(GetAppContext());
//    
//    // in both cases - checking bundle or not -
//    // first - we checking custom user file in user's documents (overwritten)
//    auto filename = getFullFileName(fileName);
//    if (jsonFile.LoadFile(filename)) {
//        return jsonFile.GetRoot();
//    }
//
//    // if we should check bundle - check in bundle
//    if (checkBundle) {
//        ResourceCache* cache = GetAppContext()->GetSubsystem<ResourceCache>();
//        SharedPtr<File> file = cache->GetFile(fileName);
//        if (file) {
//            jsonFile.Load(*file.Get());
//            return jsonFile.GetRoot();
//        }
//    }
//    
//    return JSONValue();
//}
//
//bool saveJson(const String& fileName, const JSONValue& v) {
//    JSONFile jsonFile(GetAppContext());
//    jsonFile.GetRoot() = v;
//    return jsonFile.SaveFile(getFullFileName(fileName));
//}
//
//bool saveJson(const String& fileName, JSONValue&& v) {
//    JSONFile jsonFile(GetAppContext());
//    jsonFile.GetRoot() = std::move(v);
//    return jsonFile.SaveFile(getFullFileName(fileName));
//}
 
