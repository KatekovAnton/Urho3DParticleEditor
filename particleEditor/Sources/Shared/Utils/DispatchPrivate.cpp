//
//  DispatchPrivate.cpp
//  MAX
//
//  Created by Katekov Anton on 9/5/17.
//  Copyright © 2017 AntonKatekov. All rights reserved.
//

#include "DispatchPrivate.h"



Dispatch *staticDispatch = NULL;



DispatchLock::DispatchLock()
:_locked(false)
,_processed(false)
{}

DispatchLock::~DispatchLock()
{
    // stupid situation but ok...
    if (_locked && !_processed) {
        Unlock();
    }
}

void DispatchLock::Lock()
{
    _locked = true;
    _processed = false;
    std::unique_lock<std::mutex> lk(_m);
    _cv.wait(lk, [this] {
        return this->_processed;
    });
}

void DispatchLock::Unlock()
{
    if (_locked == false) {
        return;
    }
    _locked = false;
    _processed = true;
    _cv.notify_one();
}



void DispatchImpl::OnAssigned()
{
    Dispatch::OnAssigned();
}



void DispatchPrivate::SetSharedDispatch(DispatchImpl *dispatch)
{
    staticDispatch = dispatch;
    dispatch->OnAssigned();
}

Dispatch *DispatchPrivate::GetSharedDispatch()
{
    return staticDispatch;
}
