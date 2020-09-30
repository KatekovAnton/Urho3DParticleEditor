#ifndef EngineAnimationManager_h
#define EngineAnimationManager_h

#include <vector>
#include <list>



class EngineAnimationBase;



class EngineAnimationManager {
    
    std::list<EngineAnimationBase*> _objects;
    std::vector<EngineAnimationBase*> _objectsForRemove;
    
    double _currentTime;
    
public:
    EngineAnimationManager();
    ~EngineAnimationManager();
    
    double getCurrentTime() const { return _currentTime; }
    
    void AddAnimatedObject(EngineAnimationBase* object);
    void RemoveAnimatedObject(EngineAnimationBase* object);
    
    void Update(double currentTime, double deltaTime);
};

#endif /* defined(AnimationManager_h) */
