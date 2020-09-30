#include "Shared/Engine/EngineTimer.h"



EngineTimer::EngineTimer()
:_realElapsedTime(0)
,_realFullTime(0)
,_elapsedTime(0)
,_fullTime(0)
,_speed(1.0)
,_pause(false)
,_pauseStartTime(-1)
{}

void EngineTimer::Update(double time)
{
    _realElapsedTime = time * _speed;
    _realFullTime += _realElapsedTime;
    
    if (!_pause)
    {
        if (_pauseStartTime > 0)
        {
            double pause_time =_realFullTime - _pauseStartTime;
            _pauseStartTime = -1;
            _realFullTime -= pause_time;
        }
        
        _fullTime = _realFullTime;
        _elapsedTime = _realElapsedTime;
    }
    else
    {
        _elapsedTime = 0;
    }
}

void EngineTimer::SetPause(bool pause)
{
    if (pause == _pause) {
        return;
    }
    
    _pause = pause;
    
    if (pause)
    {
        if (_pauseStartTime < 0) {
            _pauseStartTime = _realFullTime;
        }
        else {
            // pause time will be lost
        }
    }
}
