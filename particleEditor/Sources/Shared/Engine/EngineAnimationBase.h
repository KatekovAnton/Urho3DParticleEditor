#ifndef EngineAnimationBase_h
#define EngineAnimationBase_h


#include <functional>



class EngineAnimationDelegate;



typedef enum
{
    EngineAnimationCurve_EaseIn = 1,
    EngineAnimationCurve_EaseOut = 2,
    EngineAnimationCurve_EaseInOut = 3,
    EngineAnimationCurve_EaseLinear = 4
} EngineAnimationCurve;



class EngineAnimationBase {
    
    friend class EngineAnimationManager;
    
    void BaseStart(double currentTime);
    void BaseCompletlyFinish();
    void BaseUpdate(double currentTime, double deltaTime);
    
protected:
    
    bool    _isCompletlyFinish = false;
    double  _animStartTime = 0;
    double  _animTime = 0;
    double  _animDuration = 0;
    bool    _skip = false;
    
public:
    
    int     _tag = 0;
    
    EngineAnimationCurve _moveCurve = EngineAnimationCurve_EaseLinear;
    EngineAnimationDelegate *_delegate = nullptr;
    
    double GetStartTime() const {return _animStartTime;};
    
    EngineAnimationBase();
    virtual ~EngineAnimationBase();
    
    float GetAnimElapsedPart();
    void CutAnimTimeToFinish();
    
    virtual bool IsFinished();
    virtual void OnAnimationStart(double currentTime);
    virtual void OnAnimationUpdate(double currentTime, double deltaTime);
    virtual void OnAnimationFinish();
    
};



class EngineAnimationUpdate : public EngineAnimationBase {
    
    std::function<void(double currentTime, double deltaTime)> _update;
    
public:
    
    EngineAnimationUpdate(std::function<void(double currentTime, double deltaTime)> update);
    
    bool IsFinished() override;
    void OnAnimationStart(double currentTime) override;
    void OnAnimationUpdate(double currentTime, double deltaTime) override;
    
};

#endif /* defined(EngineAnimationObjectBase__) */
