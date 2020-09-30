#ifndef UIDefines_h
#define UIDefines_h

#include "Defines.h"



struct UIUserClickResult {
    
    float _distance = 10000;
    Vector3 _globalPosition;
    Vector3 _globalNormal;
        
    Urho3D::Node *_node = nullptr;
    Urho3D::Drawable *_drawable = nullptr;

};  



enum UISceneHexState { Invisible = 0, Visible };



#endif
