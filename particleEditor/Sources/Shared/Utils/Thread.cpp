//
//  Thread.cpp
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//


#include "Thread.h"



static void* runThread(void* arg)
{
    ((Thread*)arg)->run();
    return 0;
}



Thread::Thread()
:m_running(0)
,m_detached(0)
{
    
}

Thread::~Thread()
{
    if (m_running == 1) {
        #ifndef ANDROID
        //pthread_cancel(m_tid);
        #endif
    }
    if (m_running == 1 && m_detached == 0) {
        detach();
    }
}

int Thread::start()
{
    m_tid = std::thread(runThread, this);
    m_running = 1;
    return 0;
}

int Thread::join()
{
    int result = -1;
    if (m_running == 1)
    {
        m_tid.join();
        result = 0;
        if (result == 0) {
            m_detached = 0;
        }
    }
    return result;
}

int Thread::detach()
{
    int result = -1;
    if (m_running == 1 && m_detached == 0)
    {
        m_tid.detach();
        result = 0;
        if (result == 0) {
            m_detached = 1;
        }
    }
    return result;
}

void Thread::waitForUnblock()
{
    if (m_running == 0) {
        return;
    }
    
    m_SuspendMutex.lock();
    m_SuspendMutex.lock();
    m_SuspendMutex.unlock();
}

void Thread::unblock()
{
    if (m_running == 0) {
        return;
    }
    
    m_SuspendMutex.unlock();
}

