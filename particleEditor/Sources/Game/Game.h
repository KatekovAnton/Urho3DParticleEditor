//
//  Game.hpp
//  projectn
//
//  Created by Katekov Anton on 9/11/19.
//  Copyright © 2019 anton.katekov. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>


#include <Urho3D/Engine/Application.h>
#include <Urho3D/Input/Input.h>

#include "Defines.h"
#include "Shared/BaseApplication.hpp"



using namespace Urho3D;



class Game : public BaseApplication
{
    URHO3D_OBJECT(Game, BaseApplication);
    
    Node *_cameraNode = nullptr;
    float _yaw;
    float _pitch;
    bool _touchEnabled;
    
public:
    /// Construct.
    Game(Context* context);
    ~Game();
    
    /// Setup after engine initialization and before running the main loop.
    virtual void Start() override;

private:
    
    void CreateMap();
//    /// Construct a new Text instance, containing the 'Hello World' String, and add it to the UI root element.
//    void CreateText();
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
    
    
    void initGraphicsMode();

    void runBulletHitWall(const Vector3 &position, const Vector3 &normal);
    
    void MoveCamera(float timeStep);

};


#endif /* Game_hpp */
