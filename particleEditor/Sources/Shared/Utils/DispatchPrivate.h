//
//  DispatchPrivate.h
//  MAX
//
//  Created by Katekov Anton on 9/5/17.
//  Copyright © 2017 AntonKatekov. All rights reserved.
//

#ifndef DispatchPrivate_h
#define DispatchPrivate_h

#include <stdio.h>
#include <mutex>
#include <condition_variable>
#include "Dispatch.h"



class DispatchLock {

public:
    bool _locked;
    bool _processed;
    std::condition_variable _cv;
    std::mutex _m;
    
    DispatchLock();
    ~DispatchLock();
    
    void Lock();
    void Unlock();
    
};



class DispatchImpl: public Dispatch {
    
public:
    void OnAssigned() override;
    
};



class DispatchPrivate {
    
public:
    static void SetSharedDispatch(DispatchImpl *dispatch);
    static Dispatch *GetSharedDispatch();
};

#endif /* DispatchPrivate_h */
