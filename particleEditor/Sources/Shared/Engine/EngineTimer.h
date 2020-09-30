#ifndef EngineTimer_hpp
#define EngineTimer_hpp

#include <stdio.h>



class EngineTimer {
    
public:
    
    double  _realElapsedTime;
    double  _realFullTime;
    
    double  _elapsedTime;
    double  _fullTime;
    
    double  _speed;
    bool    _pause;
    double  _pauseStartTime;
    
    EngineTimer();
    
    void Update(double time);
    
    void SetPause(bool pause);
    bool GetPause() { return _pause; }
    
};

#endif /* EngineTimer_hpp */
