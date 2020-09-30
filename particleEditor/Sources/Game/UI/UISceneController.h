#ifndef UISceneController_h
#define UISceneController_h

#include "Defines.h"
#include <Shared/UI/UIDefines.h>



class UISceneModel;



class UISceneController {
    
    //
    bool _mouseRDown = false;
    bool _mouseLDown = false;
    bool _mouseMoved = false;
    
    UISceneModel *_model;
    
    float _yaw;
    float _pitch;
    
    void updateCursor();
    
    UIUserClickResult findClickedObect(const IntVector2 &pos);
    
    void handleClickedNothing(const UIUserClickResult &clickResult);
    void handleCursorChanged(const UIUserClickResult &clickResult);
    
    void createGUI();
    
public:
    
    Scene *_scene = nullptr;
    Node *_cameraNode = nullptr;
    Vector3 _cameraMove;
    
    std::function<void(const UIUserClickResult &clickResult)> _onClicked = nullptr;
    
    UISceneController(Scene *scene, Node *cameraNode);
    ~UISceneController();
    
    void onSceneUpdate(float time);
};

#endif
