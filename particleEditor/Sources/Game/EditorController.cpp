#include "EditorController.h"
#include <Game/UI/UISceneController.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>



EditorController::EditorController(Urho3D::Scene *scene, Urho3D::Node *cameraNode)
:_scene(scene)
,_uiSceneController(new UISceneController(scene, cameraNode))
{
    _uiSceneController->_onClicked = [this] (const UIUserClickResult &clickResult) {
        this->runBulletHitWall(clickResult._globalPosition, clickResult._globalNormal);
    };
}

EditorController::~EditorController()
{
    delete _uiSceneController;
}

void EditorController::handleUpdate(float time)
{
    _uiSceneController->onSceneUpdate(time);
}

void EditorController::runBulletHitWall(const Vector3 &position, const Vector3 &normal)
{
    ResourceCache* cache = _scene->GetSubsystem<ResourceCache>();
//    cache->AddResourceDir("my/folder");
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
        Node* emitter = _scene->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionSmokeSlow.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
    {
        Node* emitter = _scene->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleBurst.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }

    {
        Node* emitter = _scene->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionSparks.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
    {
        Node* emitter = _scene->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleBurstSmal.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
    {
        Node* emitter = _scene->CreateChild();
        emitter->SetWorldPosition(position);
        emitter->SetWorldRotation(r);
        auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
        particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Default/particleLaserExplosionFlare.xml"));

        particleEmitter->SetEmitting(true);
        emitter->SetTemporary(true);
    }
}
