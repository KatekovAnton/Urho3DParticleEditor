#include "UISceneController.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/StaticModelGroup.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Core/Context.h>
#include <Shared/UI/UISceneModel.h>
#include <Urho3D/Input/Input.h>

UISceneController::UISceneController(Scene *scene, Node *cameraNode)
:_model(new UISceneModel())
,_yaw(65)
,_pitch(45)
,_scene(scene)
,_cameraNode(cameraNode)
{
    
    
    _model->_onClickedNothing = [this](const UIUserClickResult &clickResult) {
        this->handleClickedNothing(clickResult);
    };
    
    _model->_onCursorChanged = [this](const UIUserClickResult &clickResult) {
        this->handleCursorChanged(clickResult);
    };
    

    
    
    _cameraNode->SetPosition(Vector3(120.0f, 9, 115));
    _cameraNode->LookAt(Vector3(90.0f, -10.0f, 95.0f));
    _yaw = _cameraNode->GetRotation().YawAngle();
    _pitch = _cameraNode->GetRotation().PitchAngle();
    _cameraNode->SetRotation(Quaternion(_pitch, _yaw, 0.0f));
}

UISceneController::~UISceneController()
{
    delete _model;
}

void UISceneController::onSceneUpdate(float time)
{
    if (_scene->GetSubsystem<UI>()->GetFocusElement()) {
        return;
    }

    UI* ui = _scene->GetSubsystem<UI>();
    IntVector2 pos = ui->GetCursorPosition();
    auto clickResult = findClickedObect(pos);
    
    Input* input = _scene->GetSubsystem<Input>();
    _model->onCursorPosition(clickResult);

    if (input->GetMouseButtonDown(MOUSEB_RIGHT) && !_mouseRDown) {
        _mouseRDown = true;
    }
    else if (!input->GetMouseButtonDown(MOUSEB_RIGHT) && _mouseRDown) {
        _mouseRDown = false;
        _mouseMoved = false;
    }
    else if (_mouseRDown) {
        IntVector2 mouseMove = input->GetMouseMove();
        if (mouseMove.x_ != 0 || mouseMove.y_ != 0) {
            _mouseMoved = true;
        }
        if (_mouseMoved) {
            const float MOUSE_SENSITIVITY = 0.1f;

            // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
            IntVector2 mouseMove = input->GetMouseMove();
            _yaw += MOUSE_SENSITIVITY * mouseMove.x_;
            _pitch += MOUSE_SENSITIVITY * mouseMove.y_;
            _pitch = Clamp(_pitch, -90.0f, 90.0f);

            Vector3 pos = _cameraNode->GetPosition();
            // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
            _cameraNode->SetRotation(Quaternion(_pitch, _yaw, 0.0f));
            _cameraNode->SetPosition(pos);
        }
    }
    
    if (input->GetMouseButtonDown(MOUSEB_LEFT) && !_mouseLDown) {
        _mouseLDown = true;
    }
    else if (!input->GetMouseButtonDown(MOUSEB_LEFT) && _mouseLDown) {
        if (!_mouseMoved) {
            _model->onUserClicked(clickResult);
        }
        _mouseLDown = false;
    }
    else if (_mouseLDown) {
        
    }
    

    // Movement speed as world units per second
    const float MOVE_SPEED = 20.0f;
    
    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    if (input->GetKeyDown(KEY_W))
       _cameraNode->Translate(Vector3::FORWARD * MOVE_SPEED * time);
    if (input->GetKeyDown(KEY_S))
       _cameraNode->Translate(Vector3::BACK * MOVE_SPEED * time);
    if (input->GetKeyDown(KEY_A))
       _cameraNode->Translate(Vector3::LEFT * MOVE_SPEED * time);
    if (input->GetKeyDown(KEY_D))
       _cameraNode->Translate(Vector3::RIGHT * MOVE_SPEED * time);
    
    
    updateCursor();
}

// Private methods

void UISceneController::updateCursor()
{
    UI* ui = _scene->GetSubsystem<UI>();
    IntVector2 pos = ui->GetCursorPosition();
    

}

void UISceneController::handleClickedNothing(const UIUserClickResult &clickResult)
{
    if (_onClicked) {
        _onClicked(clickResult);
    }
}

void UISceneController::handleCursorChanged(const UIUserClickResult& clickResult)
{

}


UIUserClickResult UISceneController::findClickedObect(const IntVector2 &pos)
{
    Graphics* graphics = _scene->GetSubsystem<Graphics>();
    Camera* camera = _cameraNode->GetComponent<Camera>();
    Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());
    
    PODVector<RayQueryResult> qSceneResults;
    RayOctreeQuery sceneQuery(qSceneResults, cameraRay, RAY_TRIANGLE, 250, DRAWABLE_GEOMETRY);
    _scene->GetComponent<Octree>()->Raycast(sceneQuery);
    
    UIUserClickResult result;
    for (unsigned i = 0; i < qSceneResults.Size(); i ++) {
        RayQueryResult& qSceneResult = qSceneResults[i];
        if (!qSceneResult.node_) {
            continue;
        }
        if (qSceneResult.node_ == result._node && result._distance < qSceneResult.distance_ + 0.01) {
            continue;
        }
        
        Node *node = qSceneResult.node_;
        if (node->GetComponent<ParticleEmitter>()) {
            continue;
        }
        
        if (result._distance < qSceneResult.distance_ + 0.01) {
            continue;
        }
        
        result._node = node;
        result._drawable = qSceneResult.drawable_;
        result._globalPosition = qSceneResult.position_;
        result._globalNormal = qSceneResult.normal_;
        result._distance = qSceneResult.distance_;
    }
    
    return result;
}
