
#include "BaseApplication.hpp"
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/UI/Cursor.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>

#include <Shared/Utils/Dispatch.h>
#include <Shared/Utils/DispatchPrivate.h>


BaseApplication::BaseApplication(Context* context) :
Application(context),
touchEnabled_(false),
useMouseMode_(MM_ABSOLUTE),
screenJoystickIndex_(M_MAX_UNSIGNED),
screenJoystickSettingsIndex_(M_MAX_UNSIGNED)
{
    if (DispatchPrivate::GetSharedDispatch() != NULL) {
        _dispatch = DispatchPrivate::GetSharedDispatch();
    }
    else {
        DispatchImpl *dispatch = new DispatchImpl();
        _dispatch = dispatch;
        DispatchPrivate::SetSharedDispatch(dispatch);
    }
}

void BaseApplication::Setup()
{
    // Modify engine startup parameters
    engineParameters_[EP_WINDOW_TITLE] = GetTypeName();
    engineParameters_[EP_LOG_NAME]     = GetSubsystem<FileSystem>()->GetAppPreferencesDir("projectn", "logs") + GetTypeName() + ".log";
    engineParameters_[EP_FULL_SCREEN]  = false;
    engineParameters_[EP_HEADLESS]     = false;
    engineParameters_[EP_SOUND]        = false;
    
    // Construct a search path to find the resource prefix with two entries:
    // The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
    // The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
    if (!engineParameters_.Contains(EP_RESOURCE_PREFIX_PATHS))
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";../share/Resources;../share/Urho3D/Resources";
    
//    #ifdef DEBUG
//        debug::SettingsWindow::load(engineParameters_);
//    #endif
    
}

void BaseApplication::Start()
{
    if (GetPlatform() == "Android" || GetPlatform() == "iOS")
        // On mobile platform, enable touch by adding a screen joystick
        InitTouchInput();
    else if (GetSubsystem<Input>()->GetNumJoysticks() == 0)
        // On desktop platform, do not detect touch when we already got a joystick
        SubscribeToEvent(E_TOUCHBEGIN, URHO3D_HANDLER(BaseApplication, HandleTouchBegin));
    
    // Set custom window Title & Icon
    SetWindowTitleAndIcon();
    
    // Create console and debug HUD
    CreateConsoleAndDebugHud();
    
    // Subscribe scene update event
    SubscribeToEvent(E_SCENEUPDATE, URHO3D_HANDLER(BaseApplication, HandleSceneUpdate));
    // Subscribe main update event
    SubscribeToEvent(E_SCENEDRAWABLEUPDATEFINISHED, URHO3D_HANDLER(BaseApplication, HandleMainUpdate));
}

void BaseApplication::Stop()
{
    engine_->DumpResources(true);
}

float BaseApplication::GetTimeFull()
{
    return _timerFull._fullTime;
}

void BaseApplication::InitTouchInput()
{
    touchEnabled_ = true;
    
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Input* input = GetSubsystem<Input>();
    XMLFile* layout = cache->GetResource<XMLFile>("UI/ScreenJoystick_Samples.xml");
    const String& patchString = GetScreenJoystickPatchString();
    if (!patchString.Empty())
    {
        // Patch the screen joystick layout further on demand
        SharedPtr<XMLFile> patchFile(new XMLFile(context_));
        if (patchFile->FromString(patchString))
            layout->Patch(patchFile);
    }
    screenJoystickIndex_ = (unsigned)input->AddScreenJoystick(layout, cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
    input->SetScreenJoystickVisible(screenJoystickSettingsIndex_, true);
}

void BaseApplication::InitMouseMode(MouseMode mode)
{
    useMouseMode_ = mode;
    
    Input* input = GetSubsystem<Input>();
    
    if (GetPlatform() != "Web")
    {
        if (useMouseMode_ == MM_FREE)
            input->SetMouseVisible(true);
        
        Console* console = GetSubsystem<Console>();
        if (useMouseMode_ != MM_ABSOLUTE)
        {
            input->SetMouseMode(useMouseMode_);
            if (console && console->IsVisible())
                input->SetMouseMode(MM_ABSOLUTE, true);
        }
    }
    else
    {
        input->SetMouseVisible(true);
        SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(BaseApplication, HandleMouseModeRequest));
        SubscribeToEvent(E_MOUSEMODECHANGED, URHO3D_HANDLER(BaseApplication, HandleMouseModeChange));
    }
}

void BaseApplication::SetLogoVisible(bool enable)
{
    if (logoSprite_)
        logoSprite_->SetVisible(enable);
}

void BaseApplication::SetWindowTitleAndIcon()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    Image* icon = cache->GetResource<Image>("Textures/UrhoIcon.png");
    graphics->SetWindowIcon(icon);
    graphics->SetWindowTitle("ProjectN");
}


//void BaseApplication::CreateLogo()
//{
//    // Get logo texture
//    ResourceCache* cache = GetSubsystem<ResourceCache>();
//    Texture2D* logoTexture = cache->GetResource<Texture2D>("Textures/FishBoneLogo.png");
//    if (!logoTexture)
//        return;
//    
//    // Create logo sprite and add to the UI layout
//    UI* ui = GetSubsystem<UI>();
//    logoSprite_ = ui->GetRoot()->CreateChild<Sprite>();
//    
//    // Set logo sprite texture
//    logoSprite_->SetTexture(logoTexture);
//    
//    int textureWidth = logoTexture->GetWidth();
//    int textureHeight = logoTexture->GetHeight();
//    
//    // Set logo sprite scale
//    logoSprite_->SetScale(256.0f / textureWidth);
//    
//    // Set logo sprite size
//    logoSprite_->SetSize(textureWidth, textureHeight);
//    
//    // Set logo sprite hot spot
//    logoSprite_->SetHotSpot(textureWidth, textureHeight);
//    
//    // Set logo sprite alignment
//    logoSprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
//    
//    // Make logo not fully opaque to show the scene underneath
//    logoSprite_->SetOpacity(0.9f);
//    
//    // Set a low priority for the logo so that other UI elements can be drawn on top
//    logoSprite_->SetPriority(-100);
//}


void BaseApplication::CreateConsoleAndDebugHud()
{
    // Get default style
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    
    // Create console
    Console* console = engine_->CreateConsole();
    console->SetDefaultStyle(xmlFile);
    console->GetBackground()->SetOpacity(0.8f);
    
    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);
}

 

void BaseApplication::onSceneUpdate(double timeStep)
{
    _dispatch->FlushMainThread(GetTimeFull());
}

void BaseApplication::HandleSceneUpdate(StringHash /*eventType*/, VariantMap& eventData)
{
    auto timeStep = eventData[SceneUpdate::P_TIMESTEP].GetFloat();
    onSceneUpdate(timeStep);
}

void BaseApplication::HandleMainUpdate(StringHash eventType, VariantMap& eventData)
{
    auto timeStep = eventData[SceneUpdate::P_TIMESTEP].GetFloat();
    _timerFull.Update(timeStep);
    
    // Example of scene pause/resume
//    if (GetTimeFull() > 2 && !GetScenePause() && !onetimeflag) {
//        SetScenePause(true);
//        printf("HandleSceneUpdate pause\n");
//    }
//    if (GetTimeFull() > 3 && GetScenePause() && !onetimeflag) {
//        SetScenePause(false);
//        onetimeflag = true;
//        printf("HandleSceneUpdate resume\n");
//    }
}

void BaseApplication::HandleTouchBegin(StringHash /*eventType*/, VariantMap& eventData)
{
    // On some platforms like Windows the presence of touch input can only be detected dynamically
    InitTouchInput();
    UnsubscribeFromEvent("TouchBegin");
}

// If the user clicks the canvas, attempt to switch to relative mouse mode on web platform
void BaseApplication::HandleMouseModeRequest(StringHash /*eventType*/, VariantMap& eventData)
{
    Console* console = GetSubsystem<Console>();
    if (console && console->IsVisible())
        return;
    Input* input = GetSubsystem<Input>();
    if (useMouseMode_ == MM_ABSOLUTE)
        input->SetMouseVisible(false);
    else if (useMouseMode_ == MM_FREE)
        input->SetMouseVisible(true);
    input->SetMouseMode(useMouseMode_);
}

void BaseApplication::HandleMouseModeChange(StringHash /*eventType*/, VariantMap& eventData)
{
    Input* input = GetSubsystem<Input>();
    bool mouseLocked = eventData[MouseModeChanged::P_MOUSELOCKED].GetBool();
    input->SetMouseVisible(!mouseLocked);
}



bool BaseApplication::hasParameter(const String& parameter) {
    return engineParameters_.Find(StringHash(parameter)) != engineParameters_.End();
}

void BaseApplication::setParameter(const String& parameter, const Variant& value){
    engineParameters_[parameter] = value;
}
