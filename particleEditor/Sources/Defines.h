#ifndef PROJECTN_DEFINES_H
#define PROJECTN_DEFINES_H

// include math
#include <Shared/SharedMath.h>
#include <string>

// предопределения классов и типов для использования в других h-файлах
// глобальные дефайны и функции

namespace Urho3D
{
    class Context;
    class Octree;
    class ResourceCache;
    class Node;
    class Scene;
    class Sprite;
    class Object;
    class Input;
    class ScriptFile;
        
    class Window;
    class DropDownList;
    class CheckBox;
    class Context;
    class JSONValue;
    
    class FileSystem;

    class StaticModelGroup;
    class AnimationState;
    class AnimatedModel;
    
    class Cursor;
    class Graphics;
    class Drawable;
    class ResourceCache;
    class Timer;
    class UIBatch;
    class UIElement;
    class XMLElement;
    class XMLFile;
    class RenderSurface;
    class UIComponent;
    class Material;
    class Technique;
    
    class ShaderVariation;
    class Font;
    class FontFace;
    struct FontGlyph;
    class VertexBuffer;
    class IndexBuffer;
    class Texture2D;
}


// классы проекта
class UrhoEditor;
class Keyboard;
class GUI;

struct Atlas;
struct Frame;

class GUIText;
class GUIElement;
class GUIGeometryBuilder; 
 

using namespace Urho3D;

#include <memory>

#ifndef NDEBUG
#ifndef URHO3D_LOGGING
#define URHO3D_LOGGING
#endif
#endif

#include <cfloat>
#include <Shared/Utils/Containers.h>
#include <Shared/Utils/Convert.h>
#include <Shared/Utils/StringUtils.h>
#include <Urho3D/IO/Log.h>
#include <Shared/Utils/Log.h>


// дефайн дебаггер - вызывает точку останова (можно юзать как ассерт, который не убивает все)
#if !defined(NDEBUG) & (defined(PROJECT_TARGET_OS_LINUX) | defined(PROJECT_TARGET_OS_MAC))
    #include <csignal>
    #define DEBUGGER(a, ...) { if (a) { URHO3D_LOGERRORF(__VA_ARGS__); std::raise(SIGINT); } }
#else
    #define DEBUGGER(a, ...) { if (a) { URHO3D_LOGERRORF(__VA_ARGS__); } }
#endif

// внутри объекта добавляет тип объекта к логу
#define DEBUGGERO(a, b, ...) DEBUGGER(a, "%s: " b, GetTypeNameStatic().CString(), __VA_ARGS__)



#define SSCANF sscanf 


#include <Shared/Utils/Events.h>
#include <Shared/Utils/PTime.h>
#include <Shared/Utils/ForEach.h>

namespace std
{
    template<> struct hash<String>
    {
        std::size_t operator()(String const& s) const noexcept
        {
            std::size_t hash = 0;
            unsigned char* c = (unsigned char*)s.CString();
            while (*c)
            {
                hash = SDBMHash(hash, *c++);
            }
            return hash;
        }
    };
}

#endif
