#ifndef PROJECTN_TIMER_H
#define PROJECTN_TIMER_H

#include <Defines.h>

/// @todo тут не все оптимально. надо дорабатывать

class PTime;

class Timeout
{
    friend class PTime;
    
//специально убрано все в приват, т.к. таймауты самоудаляются
private:
    Timeout(std::function<void()> callback_, double interval_) : 
    callback(callback_), interval(interval_) { }
    std::function<void()> callback;
    double interval = 0.0;
    bool needRemove = false;
};

/// В игре может быть разное время.
/// например игровое может замедляться или ускоряться.
/// пока это набросок для создания таймаута
/// в урхе уже свой Time есть, тут типа project time

class PTime {
    
    friend class Timeout;
    
public:    

    /// устанавливает таймер на время delay (в секундах)
    Timeout* setTimeout(std::function<void()> callback, double delay, bool repeat = false);
    
    /// удаляет установленный таймер
    void remove(Timeout* t);
    
    /// надо вызывать откуда-то каждый кадр
    /// обновляет таймауты
    void update();

    /// текущий таймстамп
    double now();
    
protected:
    
    std::map<double, Timeout*> timeouts_;
    bool inProgress_ = false;
    
};


PTime& getGameTime();
PTime& getSystemTime();



/// простейший измеритель времени на затрачиваемые операции
namespace debug {
    struct TimeMeasure {
        TimeMeasure(const String& s);
        ~TimeMeasure();
        double getElapsedTime();
        String _desc;
        double _time;
    };
}

#define TIME_MEASURE(str) ::debug::TimeMeasure t(str);


#endif
