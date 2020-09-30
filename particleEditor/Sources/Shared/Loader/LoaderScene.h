#ifndef LoaderScene_h
#define LoaderScene_h

#include "Defines.h"



class GameController;
class GameSceneData;
class ConfigScene;
class LoaderDestinationInterface;
namespace Urho3D {
class SceneResolver;
}




class LoaderScene {
    
    Scene *m_DestinationScene = nullptr;
    Context *m_Context = nullptr;
    
public:
    
    std::vector<std::string> m_Messages;
    
    LoaderScene(Scene *scene, Context *context);
    
    // load custom xml (derrived from standard)
    void loadSceneFromUrhoXML(const std::string &config);
};

#endif /* LoaderScene_h */

