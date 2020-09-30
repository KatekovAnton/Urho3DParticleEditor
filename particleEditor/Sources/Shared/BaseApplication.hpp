#ifndef BaseApplication_h
#define BaseApplication_h

#include <stdio.h>

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Scene/Scene.h>
#include <Shared/Engine/EngineTimer.h>
#include <Defines.h>


namespace Urho3D {
class Sprite;
}

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

const float TOUCH_SENSITIVITY = 2.0f;
class Dispatch;

class BaseApplication : public Application
{
    
    // Enable type information.
    URHO3D_OBJECT(BaseApplication, Application);
    
public:
    /// Construct.
    BaseApplication(Context* context);
    
    /// Setup before engine initialization. Modifies the engine parameters.
    virtual void Setup() override;
    /// Setup after engine initialization. Creates the logo, console & debug HUD.
    virtual void Start() override;
    /// Cleanup after the main loop. Called by Application.
    virtual void Stop() override;
    
    bool onetimeflag = false;
    
    float GetTimeFull();
    
protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const { return String::EMPTY; }
    /// Initialize touch input on mobile platform.
    void InitTouchInput();
    /// Initialize mouse mode on non-web platform.
    void InitMouseMode(MouseMode mode);
    /// Control logo visibility.
    void SetLogoVisible(bool enable);
    
    /// Logo sprite.
    SharedPtr<Sprite> logoSprite_;
    /// Scene.
    SharedPtr<Scene> scene_;
    
    /// Flag to indicate whether touch input has been enabled.
    bool touchEnabled_;
    /// Mouse mode option to use in the sample.
    MouseMode useMouseMode_;
    
    EngineTimer _timerFull;
    
    virtual void onSceneUpdate(double timeStep);
    
    template <typename T>
    T getParameter(const String& parameter, const Variant& defaultValue){
        auto it = engineParameters_.Find(StringHash(parameter));
        return it != engineParameters_.End() ? to<T>(it->second_) : to<T>(defaultValue);
    }

    bool hasParameter(const String& parameter);
    void setParameter(const String& parameter, const Variant& value);
    
private:
    /// Set custom window Title & Icon
    void SetWindowTitleAndIcon();
    /// Create console and debug HUD.
    void CreateConsoleAndDebugHud();
    /// Handle request for mouse mode on web platform.
    void HandleMouseModeRequest(StringHash eventType, VariantMap& eventData);
    /// Handle request for mouse mode change on web platform.
    void HandleMouseModeChange(StringHash eventType, VariantMap& eventData);
    /// Handle scene update event to control camera's pitch and yaw for all samples.
    void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle main update event
    void HandleMainUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle touch begin event to initialize touch input on desktop platform.
    void HandleTouchBegin(StringHash eventType, VariantMap& eventData);
    
    /// Screen joystick index for navigational controls (mobile platforms only).
    unsigned screenJoystickIndex_;
    /// Screen joystick index for settings (mobile platforms only).
    unsigned screenJoystickSettingsIndex_;
    
    /// Background dispatcher
    Dispatch *_dispatch = nullptr;
};

#endif /* BaseApplication_hpp */
