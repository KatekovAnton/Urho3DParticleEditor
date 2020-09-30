//
//  Thread.h
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Thread__
#define __MAX__Thread__

#include <thread>
#include <mutex>
#include <signal.h>


class Thread
{
    
    std::thread  m_tid;
    int        m_running;
    int        m_detached;
    
    std::mutex m_SuspendMutex;
    
public:
    
    Thread();
    virtual ~Thread();
    
    int start();
    int join();
    int detach();
    
    void waitForUnblock();
    void unblock();
    
    virtual void run() = 0;
    
};

#endif /* defined(__MAX__Thread__) */
