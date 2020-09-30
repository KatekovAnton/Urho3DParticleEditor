#ifndef PROJECTN_LOG_H
#define PROJECTN_LOG_H

#include <Defines.h>


void LOG(int level, const char* format, ...);
#define LOGR(...) LOG(-1, __VA_ARGS__)
#define LOGT(...) LOG(0, __VA_ARGS__)
#define LOGD(...) LOG(1, __VA_ARGS__)
#define LOGI(...) LOG(2, __VA_ARGS__)
#define LOGW(...) LOG(3, __VA_ARGS__)
#define LOGE(...) LOG(4, __VA_ARGS__)


#endif
