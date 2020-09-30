#include "Events.h"
#include <Defines.h>



namespace events {


#ifndef NDEBUG

EventHandler* makeHandler(std::function<void()> v, DebugInfo d){
    return new EventHandler11Impl([v, d](StringHash, VariantMap&){ v(); });
}

EventHandler* makeHandler(std::function<void(VariantMap&)> v, DebugInfo d){
    return new EventHandler11Impl([v, d](StringHash, VariantMap& vm){ v(vm); });
}

EventHandler* makeHandler(std::function<void(StringHash, VariantMap&)> v, DebugInfo d){
    return new EventHandler11Impl([v, d](StringHash sh, VariantMap& vm){ v(sh, vm); });
}
    
    
    
#else

EventHandler* makeHandler(std::function<void()> v, DebugInfo d){
    return new EventHandler11Impl([v](StringHash, VariantMap&){ v(); });
}

EventHandler* makeHandler(std::function<void(VariantMap&)> v, DebugInfo d){
    return new EventHandler11Impl([v](StringHash, VariantMap& vm){ v(vm); });
}

EventHandler* makeHandler(std::function<void(StringHash, VariantMap&)> v, DebugInfo d){
    return new EventHandler11Impl(v);
}

#endif



}
