#ifndef Timer_h
#define Timer_h

#include <chrono>



namespace projectn { namespace utils {

class Timer {
    
    std::chrono::high_resolution_clock::time_point _startTime;
    double _duration = -1;
    
public:
    
    void start();
    void stop();
    
    double duration() const {
        return _duration;
    }
    
};

}}

#endif 
