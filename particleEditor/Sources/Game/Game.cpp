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
#include <Urho3D/Graphics/Texture2D.h>
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

// UI headers
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/ToolTip.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>


#include <Shared/Loader/LoaderScene.h>
#include <Game/EditorController.h>

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

}

void Game::Start()
{
    initGraphicsMode();
    
    // Execute base class startup
    BaseApplication::Start();
    
    CreateGui();
    
    CreateMap();
    
    // Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    // like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    // could subscribe in the constructor instead.
    SubscribeToEvents();
    
    _editor = new EditorController(scene_, _cameraNode);
}

void Game::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Game, HandleUpdate));
    
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Game, HandlePostRenderUpdate));
}

void Game::CreateGui()
{
    GetSubsystem<Input>()->SetMouseVisible(true);

    // Load XML file containing default UI style sheet
    auto* cache = GetSubsystem<ResourceCache>();
    auto* style = cache->GetResource<XMLFile>("_System/DefaultStyle.xml");

    uiRoot_ = GetSubsystem<UI>()->GetRoot();
    // Set the loaded style as default style
    uiRoot_->SetDefaultStyle(style);

    // Initialize Window
    InitWindow();

    // Create and add some controls to the Window
    InitControls();

    // Create a draggable Fish
    CreateDraggableFish();

    // Set the mouse mode to use in the sample
    InitMouseMode(MM_FREE);
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
}

void Game::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;
    float timeStep = eventData[P_TIMESTEP].GetFloat();
    _editor->handleUpdate(timeStep);
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




void Game::InitControls()
{
    // Create a CheckBox
    auto* checkBox = new CheckBox(context_);
    checkBox->SetName("CheckBox");

    // Create a Button
    auto* button = new Button(context_);
    button->SetName("Button");
    button->SetMinHeight(24);

    // Create a LineEdit
    auto* lineEdit = new LineEdit(context_);
    lineEdit->SetName("LineEdit");
    lineEdit->SetMinHeight(24);

    // Add controls to Window
    window_->AddChild(checkBox);
    window_->AddChild(button);
    window_->AddChild(lineEdit);

    // Apply previously set default style
    checkBox->SetStyleAuto();
    checkBox->SetSize(30, 30);
    button->SetStyleAuto();
    lineEdit->SetStyleAuto();
}

void Game::InitWindow()
{
    // Create the Window and add it to the UI's root node
    window_ = new Window(context_);
    uiRoot_->AddChild(window_);

    // Set Window size and layout settings
    window_->SetMinWidth(384);
    window_->SetLayout(LM_VERTICAL, 10, IntRect(10, 10, 10, 10));
    window_->SetAlignment(HA_CUSTOM, VA_CUSTOM);
    
    window_->SetName("Window");

    // Create Window 'titlebar' container
    auto* titleBar = new UIElement(context_);
    titleBar->SetMinSize(0, 24);
    titleBar->SetVerticalAlignment(VA_TOP);
    titleBar->SetLayoutMode(LM_HORIZONTAL);

    // Create the Window title Text
    auto* windowTitle = new Text(context_);
    windowTitle->SetName("WindowTitle");
    windowTitle->SetText("Hello GUI!");
    windowTitle->SetFontSize(40);

    // Create the Window's close button
    auto* buttonClose = new Button(context_);
    buttonClose->SetName("CloseButton");

    // Add the controls to the title bar
    titleBar->AddChild(windowTitle);
    titleBar->AddChild(buttonClose);

    // Add the title bar to the Window
    window_->AddChild(titleBar);

    // Apply styles
    window_->SetStyleAuto();
    windowTitle->SetStyleAuto();
    windowTitle->SetFontSize(20);
    buttonClose->SetStyle("CloseButton");

    // Subscribe to buttonClose release (following a 'press') events
    SubscribeToEvent(buttonClose, E_RELEASED, URHO3D_HANDLER(Game, HandleClosePressed));

    // Subscribe also to all UI mouse clicks just to see where we have clicked
    SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(Game, HandleControlClicked));
}

void Game::CreateDraggableFish()
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* graphics = GetSubsystem<Graphics>();

    // Create a draggable Fish button
    auto* draggableFish = new Button(context_);
    draggableFish->SetTexture(cache->GetResource<Texture2D>("Textures/UrhoDecal.dds")); // Set texture
    draggableFish->SetBlendMode(BLEND_ADD);
    draggableFish->SetSize(128, 128);
    draggableFish->SetPosition((graphics->GetWidth() - draggableFish->GetWidth()) / 2, 200);
    draggableFish->SetName("Fish");
    uiRoot_->AddChild(draggableFish);

    // Add a tooltip to Fish button
    auto* toolTip = new ToolTip(context_);
    draggableFish->AddChild(toolTip);
    toolTip->SetPosition(IntVector2(draggableFish->GetWidth() + 5, draggableFish->GetWidth() / 2)); // slightly offset from close button
    auto* textHolder = new BorderImage(context_);
    toolTip->AddChild(textHolder);
    textHolder->SetStyle("ToolTipBorderImage");
    auto* toolTipText = new Text(context_);
    textHolder->AddChild(toolTipText);
    toolTipText->SetStyle("ToolTipText");
    toolTipText->SetText("Please drag me!");

    // Subscribe draggableFish to Drag Events (in order to make it draggable)
    // See "Event list" in documentation's Main Page for reference on available Events and their eventData
    SubscribeToEvent(draggableFish, E_DRAGBEGIN, URHO3D_HANDLER(Game, HandleDragBegin));
    SubscribeToEvent(draggableFish, E_DRAGMOVE, URHO3D_HANDLER(Game, HandleDragMove));
    SubscribeToEvent(draggableFish, E_DRAGEND, URHO3D_HANDLER(Game, HandleDragEnd));
}

void Game::HandleDragBegin(StringHash eventType, VariantMap& eventData)
{
    // Get UIElement relative position where input (touch or click) occurred (top-left = IntVector2(0,0))
    dragBeginPosition_ = IntVector2(eventData["ElementX"].GetInt(), eventData["ElementY"].GetInt());
}

void Game::HandleDragMove(StringHash eventType, VariantMap& eventData)
{
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
    UIElement* draggedElement = static_cast<UIElement*>(eventData["Element"].GetPtr());
    draggedElement->SetPosition(dragCurrentPosition - dragBeginPosition_);
}

void Game::HandleDragEnd(StringHash eventType, VariantMap& eventData) // For reference (not used here)
{
}

void Game::HandleClosePressed(StringHash eventType, VariantMap& eventData)
{
//    if (GetPlatform() != "Web")
        engine_->Exit();
}

void Game::HandleControlClicked(StringHash eventType, VariantMap& eventData)
{
    // Get the Text control acting as the Window's title
    auto* windowTitle = window_->GetChildStaticCast<Text>("WindowTitle", true);

    // Get control that was clicked
    auto* clicked = static_cast<UIElement*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());

    String name = "...?";
    if (clicked)
    {
        // Get the name of the control that was clicked
        name = clicked->GetName();
    }

    // Update the Window's title text
    windowTitle->SetText("Hello " + name + "!");
}

