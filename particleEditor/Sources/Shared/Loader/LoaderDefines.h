#ifndef LoaderDefines_h
#define LoaderDefines_h

#include <Shared/SharedDefines.h>
#include <map>



struct LoaderSpawnActorInfo 
{
    // alternative to player - instead of setting player id we will set player name or faction name
    std::string _faction;
    // actor position on spawn
    Urho3D::IntVector2 _position;
    
};

#endif
