#include "PTime.h"

#include <Urho3D/Core/Timer.h>

static PTime GameTime;
static PTime SystemTime;

PTime& getGameTime() { 
    return GameTime;
}
PTime& getSystemTime(){
    return SystemTime;
}


/// устанавливает таймер на время delay (в секундах)
Timeout* PTime::setTimeout(std::function<void()> callback, double delay, bool repeat){
    double ts = now() + delay;
    auto timeout = new Timeout( callback, repeat ? delay : 0.0 );
    while(containsKey(timeouts_, ts)){
        ts += 0.0000001;
    }
    timeouts_[ts] = timeout;
    return timeout;
}
 

/// удаляет установленный таймер 
void PTime::remove(Timeout* t){
    if (inProgress_){
        t->needRemove = true;
    } else {
        ::remove( timeouts_, t );
        
        delete t;
        t = nullptr;
    }
    
}


void PTime::update(){
    if (timeouts_.size() > 0) {
        double tm = now();
        
        
        inProgress_ = true;
        
        for (auto it = timeouts_.begin(); it != timeouts_.end();) {
            Timeout* t = it->second;
            if (!t || t->needRemove) {
                it = timeouts_.erase(it);
                delete t;
            }
            else {
                double ts = it->first;
                if (ts <= tm) {
                    
                    if (t->interval) {
                        ts += t->interval;
                        // TODO: тут возникает фиговая ситуация
                        // когда апдейт не вызывался долгое время, то будут многократные вызовы интервалов
                        // чтоб такого не было такое вот. может это не совсем правильно
                        if (ts <= tm){
                            ts = tm + t->interval;
                        }
                        
                        t->callback();
                        timeouts_[ts] = t;
                        
                    } else {
                        t->callback();
                        delete t;
                    }
                    
                    it = timeouts_.erase(it);
                    
                } else {
                    break;
                }
            }
        }
        
        inProgress_ = false;
        
    }
}

double PTime::now(){
    static HiresTimer timer;
    return timer.GetUSec(false)/ 1000000.0;
}

static int timeMeasureLevel = -1;
    
debug::TimeMeasure::TimeMeasure(const String& s)
{
    _desc = s;
    _time = getSystemTime().now();
    timeMeasureLevel++;
}

double debug::TimeMeasure::getElapsedTime(){
    return getSystemTime().now() - _time;    
}

debug::TimeMeasure::~TimeMeasure()
{
    #ifndef NDEBUG
        float tm = getElapsedTime();
        int tl2 = timeMeasureLevel * 2;
        // заполняем уровень через тире
        char* buf = new char[tl2+2];
        memset(buf, '-', tl2);
        if (tl2 > 0) {
            buf[tl2-1] = ' ';
        }
        buf[tl2] = 0;
        
        if (tm < 0.0001f) { // такая точность обычно не нужна
            LOGD("TimeMeasure: %s%s < 0.0001 sec", buf, _desc.CString());
        } else 
        if (tm < 1.0f) {
            LOGD("TimeMeasure: %s%s %0.5f sec", buf, _desc.CString(), tm);
        } else 
        {
            LOGD("TimeMeasure: %s%s %0.2f sec", buf, _desc.CString(), tm);
        }
        timeMeasureLevel--;
        delete[] buf;
    #endif
}


