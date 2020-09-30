#ifndef PROJECTN_EVENTS_H
#define PROJECTN_EVENTS_H

#include <Defines.h>
#include <Urho3D/Core/Object.h>

// вспомогательная штука, поэтому отдельный нэймспейс чтоб никому не мешать
// пользоваться лучше макросом E_H из Defines.h

namespace events {



struct DebugInfo { std::string fileName; int line = 0; };

#ifndef NDEBUG

// если евент вызывается как-то отложенно, то по файлу и строке можно отловить откуда он появился
#define E_H(...) (events::makeHandler(__VA_ARGS__, { __FILE__, __LINE__ }))
#define makeHandler(...) makeHandler(__VA_ARGS__, DebugInfo d)
#else
#define E_H(...) (events::makeHandler(__VA_ARGS__))
#define makeHandler(...) makeHandler(__VA_ARGS__)
#endif
    
EventHandler* makeHandler(std::function<void()> v);
EventHandler* makeHandler(std::function<void(VariantMap&)> v);
EventHandler* makeHandler(std::function<void(StringHash, VariantMap&)> v);
    
#undef makeHandler
    
}


#endif

