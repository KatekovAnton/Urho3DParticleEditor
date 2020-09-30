#ifndef MAX_EngineAnimationDelegate_h
#define MAX_EngineAnimationDelegate_h

class EngineAnimationBase;

class EngineAnimationDelegate {
    
public:
    
    virtual ~EngineAnimationDelegate() {}
    
    virtual void OnAnimationStart(EngineAnimationBase* animation) {};
    virtual void OnAnimationUpdate(EngineAnimationBase* animation) {};
    virtual void OnAnimationFinish(EngineAnimationBase* animation) {};
    
};

#endif
