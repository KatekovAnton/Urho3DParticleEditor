//
//  DispatchThread.h
//  MAX
//
//  Created by Katekov Anton on 8/4/18.
//  Copyright © 2018 AntonKatekov. All rights reserved.
//

#ifndef DispatchThread_h
#define DispatchThread_h

#include <Shared/Utils/Thread.h>
#include <atomic>



class DispatchThread;



class DispatchThreadDelegate {
public:
    virtual void ThreadExecuting(DispatchThread *sender) = 0;
    virtual void ThreadDidFinish(DispatchThread *sender) = 0;
};



class DispatchThread: public Thread {
public:
    DispatchThreadDelegate *_delegate;
    
    DispatchThread(DispatchThreadDelegate *delegate);
    void run() override;
};

#endif /* DispatchThread_h */
