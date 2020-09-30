#ifndef EngineAnimationWait_h
#define EngineAnimationWait_h

#include "EngineAnimationBase.h"



class EngineAnimationWait : public EngineAnimationBase {
    
public:
    EngineAnimationWait(double waitTime);
    ~EngineAnimationWait();
    
    virtual void OnAnimationStart(double currentTime) override;
    virtual void OnAnimationUpdate(double currentTime, double deltaTime) override;
    
};

class EngineAnimationWait1: public EngineAnimationWait {
public:
    EngineAnimationWait1(double waitTime);
};

class EngineAnimationWait2: public EngineAnimationWait {
public:
    EngineAnimationWait2(double waitTime);
};

#endif /* defined(EngineAnimationWait_h) */
