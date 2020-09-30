#include "EngineAnimationBase.h"
#include "EngineAnimationDelegate.h"
#include <math.h>
#ifdef PROJECTN_TARGET_OS_WIN
#include <corecrt_math_defines.h>
#endif



EngineAnimationBase::EngineAnimationBase()
:_isCompletlyFinish(false)
,_animStartTime(0)
,_animTime(0)
,_animDuration(0)
,_skip(false)
,_tag(0)
,_moveCurve(EngineAnimationCurve_EaseLinear)
,_delegate(nullptr)
{

}

EngineAnimationBase::~EngineAnimationBase()
{}

void EngineAnimationBase::BaseUpdate(double currentTime, double deltaTime)
{
    _animTime += deltaTime;
    if (_animTime > _animDuration) {
        _animTime = _animDuration;
    }
    
    this->OnAnimationUpdate(currentTime, deltaTime);
    
    if (_delegate) {
        _delegate->OnAnimationUpdate(this);
    }
}

float EngineAnimationBase::GetAnimElapsedPart()
{
    float result = 0;
    if (_moveCurve == EngineAnimationCurve_EaseOut)
    {
        float fromminonetoone = (2.0*_animTime/_animDuration) - 1.0;
        result = sinf(fromminonetoone * M_PI_2) * 0.5 + 0.5;
    }
    else if (_moveCurve == EngineAnimationCurve_EaseIn)
    {
        float fromminonetoone = (_animTime/_animDuration) - 1.0;
        result = sinf(fromminonetoone * M_PI_2) * 1.0 + 1.0;
    }
    else if (_moveCurve == EngineAnimationCurve_EaseInOut)
    {
        float fromminonetoone = (_animTime/_animDuration);
        result = sinf(fromminonetoone * M_PI_2) * 1.0;
    }
    else //if (_moveCurve == EngineAnimationCurve_EaseLinear)
    {
        result = _animTime/_animDuration;
    }
    return result;
}

void EngineAnimationBase::CutAnimTimeToFinish()
{
    _animTime = _animDuration;
}

void EngineAnimationBase::BaseStart(double currentTime)
{
    _animStartTime = currentTime;
    this->OnAnimationStart(currentTime);
    if (_delegate) {
        _delegate->OnAnimationStart(this);
    }
}

void EngineAnimationBase::BaseCompletlyFinish()
{
    if(_isCompletlyFinish)
        return;
    _isCompletlyFinish = true;
    this->OnAnimationFinish();
    if (_delegate) {
        _delegate->OnAnimationFinish(this);
    }
}

bool EngineAnimationBase::IsFinished()
{
    return _animTime >= _animDuration;
}

void EngineAnimationBase::OnAnimationStart(double currentTime)
{}

void EngineAnimationBase::OnAnimationUpdate(double currentTime, double deltaTime)
{}

void EngineAnimationBase::OnAnimationFinish()
{}


EngineAnimationUpdate::EngineAnimationUpdate(std::function<void(double currentTime, double deltaTime)> update)
:EngineAnimationBase()
,_update(update)
{
    _animDuration = 999999999;
}

bool EngineAnimationUpdate::IsFinished()
{
    return false;
}

void EngineAnimationUpdate::OnAnimationStart(double currentTime)
{}

void EngineAnimationUpdate::OnAnimationUpdate(double currentTime, double deltaTime)
{
    if (_update != nullptr) {
        _update(currentTime, deltaTime);
    }
}
