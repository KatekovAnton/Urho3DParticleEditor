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



class EditorController;



class Game : public BaseApplication
{
    URHO3D_OBJECT(Game, BaseApplication);
    
    EditorController *_editor;

    Node *_cameraNode = nullptr;
//    float _yaw;
//    float _pitch;
    bool _touchEnabled;
    
public:
    /// Construct.
    Game(Context* context);
    ~Game();
    
    /// Setup after engine initialization and before running the main loop.
    virtual void Start() override;

private:
    
    /// The Window.
    SharedPtr<Window> window_;
    /// The UI's root UIElement.
    SharedPtr<UIElement> uiRoot_;
    /// Remembered drag begin position.
    IntVector2 dragBeginPosition_;
    
    
    
    void CreateGui();
    void CreateMap();
    void SubscribeToEvents();
    
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
    
    void initGraphicsMode();
    
    
    void InitControls();
    void InitWindow();
    void CreateDraggableFish();
    void HandleDragBegin(StringHash eventType, VariantMap& eventData);
    void HandleDragMove(StringHash eventType, VariantMap& eventData);
    void HandleDragEnd(StringHash eventType, VariantMap& eventData);
    void HandleClosePressed(StringHash eventType, VariantMap& eventData);
    void HandleControlClicked(StringHash eventType, VariantMap& eventData);

};


#endif /* Game_hpp */
