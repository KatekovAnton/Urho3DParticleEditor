#include "LoaderScene.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Terrain.h>

#include <Urho3D/ThirdParty/Bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Resource/XMLFile.h>



using namespace std;

//{
//    {
//        Node* objectNode = scene_->CreateChild("box2");
//        objectNode->SetPosition(Vector3(mashroomPos.x_, 0.5, mashroomPos.z_ + 0.5));
//        // Create a rotation quaternion from up vector to terrain normal
//        objectNode->SetRotation(Quaternion(Vector3::UP, Vector3(0, 1, 0)));
//        StaticModel* object = objectNode->CreateComponent<StaticModel>();
//        object->SetModel(cache->GetResource<Model>("Default/cube.mdl"));
//        object->SetMaterial(cache->GetResource<Material>("Default/cubeMaterial.xml"));
//        object->SetCastShadows(true);
//        RigidBody* body = objectNode->CreateComponent<RigidBody>();
//        body->SetMass(1.0);
//        body->SetFriction(1);
//        CollisionShape* shape = objectNode->CreateComponent<CollisionShape>();
//        shape->SetBox(Vector3(1,1,1));
//    }
//}

LoaderScene::LoaderScene(Scene *scene, Context *context)
:m_DestinationScene(scene)
,m_Context(context)
{}

void LoaderScene::loadSceneFromUrhoXML(const std::string &config)
{
    ResourceCache* cache = m_Context->GetSubsystem<ResourceCache>();
    auto file = cache->GetFile(String(config.c_str(), static_cast<unsigned>(config.length())));
    
    if (!file) {
        m_Messages.push_back("cannot open scene file");
        return;
    }
    
    m_DestinationScene->InstantiateXML(*file.Get(), Vector3(), Quaternion::IDENTITY);
}
