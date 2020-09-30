//
//  Gamer.cpp
//  projectn
//
//  Created by Katekov Anton on 9/11/19.
//  Copyright © 2019 anton.katekov. All rights reserved.
//

#include "Game.h"
#include <Urho3D/ThirdParty/Bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/DecalSet.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/StaticModelGroup.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/TextureCube.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/StateMachineRunner.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RaycastVehicle.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/DebugNew.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>

#include <Shared/Loader/LoaderScene.h>


// Expands to this example's entry-point
URHO3D_DEFINE_APPLICATION_MAIN(Game)
using namespace Urho3D;
static Game* gameInstance = nullptr;

    
Game::Game(Context* context) :
BaseApplication(context)
{
    gameInstance = this;
    
    if (!engineParameters_.Contains(EP_RESOURCE_PREFIX_PATHS))
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";../Resources";
    
    engineParameters_[EP_RESOURCE_PATHS] = "Resources;Data;CoreData";
    
}

Game::~Game()
{

}

void Game::initGraphicsMode()
{
#ifndef NDEBUG
    // debug = settings in debug::SettingsWindow::load(engineParameters_)
#else
//    Graphics *g = GetSubsystem<Graphics>();
//    g->SetMode(g->GetWidth(), g->GetHeight(), g->GetFullscreen(), g->GetBorderless(), g->GetResizable(), g->GetHighDPI(), true, false, false, g->GetMonitor(), Min(g->GetRefreshRate(), 50));
#endif
    
    
    
//    {
//        Graphics *g = GetSubsystem<Graphics>();
//
//        {
//            float a, b, c;
//            SDL_GetDisplayDPI(0, &a, &b, &c);
//
//            // check to see if we should scale based on windowsize : drawable size ratio
//            // only relevant for OSX Retina displays?
//            SDL_Window* curwindow = g->GetWindow();
//
//            int gl_w, gl_h;
//            SDL_GL_GetDrawableSize(curwindow, &gl_w, &gl_h);
//
//            int sdl_w, sdl_h;
//            SDL_GetWindowSize(curwindow, &sdl_w, &sdl_h);
//
//            assert(sdl_w != 0);
//            float multiplier = (float)gl_w / (float)sdl_w;
//
//            GUI::options->retinaFactor = multiplier; 
//        }
//    }
}

void Game::Start()
{
    initGraphicsMode();
    
    // Execute base class startup
    BaseApplication::Start();
    
    CreateMap();
    
    // Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    // like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    // could subscribe in the constructor instead.
    SubscribeToEvents();
    
    // Set the mouse mode to use in the sample
    BaseApplication::InitMouseMode(MM_FREE);

}



void Game::runBulletHitWall(const Vector3 &position, const Vector3 &normal)
{
    ResourceCache* cache = scene_->GetSubsystem<ResourceCache>();
    
    Quaternion r = Quaternion(Vector3(0, 0, -1), normal);
//    {
//        Node* emitter = scene_->CreateChild();
//        emitter->SetWorldPosition(position);
//        emitter->SetWorldRotation(r);
//        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
//        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionSmokeFast.xml"));
//
//        particleEmitter->SetEmitting(true);
//        emitter->SetTemporary(true);
//    }
    {
        Node* emitter = scene_->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionSmokeSlow.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
    {
        Node* emitter = scene_->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleBurst.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }

    {
        Node* emitter = scene_->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionSparks.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
    {
        Node* emitter = scene_->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleBurstSmal.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
    {
        Node* emitter = scene_->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionFlare.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
}

void Game::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Game, HandleUpdate));
    
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Game, HandlePostRenderUpdate));
}

void Game::CreateMap()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    scene_ = new Scene(context_);
    // Create scene subsystem components
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<PhysicsWorld>();
    scene_->CreateComponent<DebugRenderer>();
    scene_->CreateComponent<StateMachineRunner>();
    
    {
        Node* sceneNode = scene_->CreateChild("scene");
        sceneNode->SetPosition(Vector3(0, 0.2, 0));
        sceneNode->SetRotation(Quaternion(0, 0, 0));
        
        LoaderScene loader(scene_, context_);
        
        {
            TIME_MEASURE("Game: scene loading time:");
            
            // load the scene from file
            loader.loadSceneFromUrhoXML("Scenes/SampleSceneShort.xml");
        }
        
    }
    
    // _gameController->_ui->_onClicked = [this] (const UIUserClickResult &clickResult) {
    //     this->runSomething(clickResult);
    // };
    
    {
        _cameraNode = scene_->CreateChild();
        Camera* camera = _cameraNode->CreateComponent<Camera>();
        camera->SetFarClip(500.0f);
        
        scene_->SetSmoothingConstant(50);
        auto* renderer = GetSubsystem<Renderer>();

        renderer->SetHDRRendering(!getParameter<bool>("noHDR", false));
        renderer->SetShadowQuality(getParameter<ShadowQuality>("ShadowQuality", ShadowQuality::SHADOWQUALITY_PCF_24BIT));
        
        // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, _cameraNode->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);
        // rough - red
        // met - green

        SharedPtr<RenderPath> effectRenderPath = viewport->GetRenderPath()->Clone();
        
//        if (getParameter<bool>("noSSAO", false)){
//            effectRenderPath = viewport->GetRenderPath()->Clone();
//        } else {
//            XMLFile* renderPathFile = cache->GetResource<XMLFile>("_urhoExtensions/RenderPaths/ForwardDepthSSAO.xml");
//            effectRenderPath = new RenderPath();
//            effectRenderPath->Load(renderPathFile);
//        }
        
        if (!getParameter<bool>("noFXAA2", false)){
            effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/FXAA2.xml"));
        }
        
        if (!getParameter<bool>("noGammaCorrection", false)){
            effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/GammaCorrection.xml"));
        }
        
        if (!getParameter<bool>("noTonemap", false)){
            effectRenderPath->Append(cache->GetResource<XMLFile>("System/Postprocess/Tonemap.xml"));
        }
        
        if (!getParameter<bool>("noBloom", false)){
            effectRenderPath->Append(cache->GetResource<XMLFile>("System/Postprocess/BloomHDRSimple.xml"));
        }
        
        // doesn't work well
//        effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/AutoExposure.xml"));
        GetSubsystem<Renderer>()->GetViewport(0)->SetRenderPath(effectRenderPath);
        GetSubsystem<Renderer>()->SetReuseShadowMaps(false);
        
    }
    
    {
        Node *objectNode = scene_->CreateChild();
        objectNode->SetPosition(Vector3(0, -2, 0));

        RigidBody* body = objectNode->CreateComponent<RigidBody>();
        body->SetMass(0);
        body->SetFriction(1);
        CollisionShape* shape = objectNode->CreateComponent<CollisionShape>();
        shape->SetStaticPlane();
    }
    
    _cameraNode->SetPosition(Vector3(120.0f, 9, 115));
    _cameraNode->LookAt(Vector3(95.0f, 2.0f, 115.0f));
    _yaw = _cameraNode->GetRotation().YawAngle();
    _pitch = _cameraNode->GetRotation().PitchAngle();
    
}

void Game::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}


void Game::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    // If draw debug mode is enabled, draw viewport debug geometry, which will show eg. drawable bounding boxes and skeleton
    // bones. Note that debug geometry has to be separately requested each frame. Disable depth test so that we can see the
    // bones properly
    if (false)
    {
        GetSubsystem<Renderer>()->DrawDebugGeometry(false);
    }
    
    if (false)
    {
        // Use debug renderer to output physics world debug.
        auto debugRendererComp = scene_->GetComponent<DebugRenderer>();
        auto physicsComp = scene_->GetComponent<PhysicsWorld>();
        if(physicsComp != nullptr)
        {
            physicsComp->DrawDebugGeometry(debugRendererComp, false);
        }
    }
}

void Game::MoveCamera(float timeStep)
{
    // Right mouse button controls mouse cursor visibility: hide when pressed
    auto* ui = GetSubsystem<UI>();
    auto* input = GetSubsystem<Input>();

    // Do not move if the UI has a focused element
    if (ui->GetFocusElement())
        return;

    // Movement speed as world units per second
    const float MOVE_SPEED = 10.0f;
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    // Only move the camera when the cursor is hidden
    if (input->GetMouseButtonDown(MOUSEB_RIGHT))
    {
        IntVector2 mouseMove = input->GetMouseMove();
        _yaw += MOUSE_SENSITIVITY * mouseMove.x_;
        _pitch += MOUSE_SENSITIVITY * mouseMove.y_;
        _pitch = Clamp(_pitch, -90.0f, 90.0f);

        // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
        _cameraNode->SetRotation(Quaternion(_pitch, _yaw, 0.0f));
    }

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    if (input->GetKeyDown(KEY_W))
        _cameraNode->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_S))
        _cameraNode->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_A))
        _cameraNode->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_D))
        _cameraNode->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
    
//    if (!input->GetKeyDown(KEY_R)) 
//    {
//        if (rDown_) {
//            rDown_ = false;
//            SwitchRoofs();
//        }
//    }
//    else {
//        rDown_ = true;
//    }
//    
//    if (!input->GetKeyDown(KEY_T)) 
//    {
//        if (tDown_) {
//            tDown_ = false;
//            parameterSource_->Set("Locked", !parameterSource_->Get("Locked"));
//            
//            UpdateTextLocked();
//        }
//    }
//    else {
//        tDown_ = true;
//    }
//    
//    if (!input->GetKeyDown(KEY_Y)) 
//    {
//        if (yDown_) {
//            yDown_ = false;
//            parameterSource_->Set("Opened", !parameterSource_->Get("Opened"));
//            
//            UpdateTextOpened();
//        }
//    }
//    else {
//        yDown_ = true;
//    }
}

