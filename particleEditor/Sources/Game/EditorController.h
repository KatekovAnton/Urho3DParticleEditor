#ifndef EditorController_h
#define EditorController_h

#include "Defines.h"



class UISceneController;



class EditorController {

    Urho3D::Scene *_scene;

    UISceneController *_uiSceneController;
    
public:
    EditorController(Urho3D::Scene *scene, Urho3D::Node *cameraNode);
    ~EditorController();
    
    void handleUpdate(float time);
    
    void runBulletHitWall(const Vector3 &position, const Vector3 &normal);

};

#endif
