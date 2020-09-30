#include "Log.h"


void LOG(int level, const char* format, ...)
{
    static const size_t limit = 5000;
    char* buf = new char[limit];
    
    va_list args;
    va_start(args, format);
    int l = vsnprintf(buf, limit - 1, format, args);
    va_end(args);
    
    if (l < 0) return;
    if (l > limit - 1) buf[limit - 1] = 0;
    
    Urho3D::Log::Write(level, buf);
    delete[] buf;
}
        
