#include "EngineAnimationWait.h"



EngineAnimationWait::EngineAnimationWait(double waitTime)
:EngineAnimationBase()
{
    _animDuration = waitTime;
}

EngineAnimationWait::~EngineAnimationWait()
{
    
}

void EngineAnimationWait::OnAnimationUpdate(double currentTime, double deltaTime)
{}

void EngineAnimationWait::OnAnimationStart(double currentTime)
{}


EngineAnimationWait1::EngineAnimationWait1(double waitTime)
:EngineAnimationWait(waitTime)
{
    
}

EngineAnimationWait2::EngineAnimationWait2(double waitTime)
:EngineAnimationWait(waitTime)
{
    
}
