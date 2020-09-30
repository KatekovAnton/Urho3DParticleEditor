//
//  Dispatch.cpp
//  MAX
//
//  Created by Katekov Anton on 9/4/17.
//  Copyright © 2017 AntonKatekov. All rights reserved.
//

#include "Dispatch.h"
#include "DispatchPrivate.h"
#include "Thread.h"
#if defined DEBUG
#include <cassert>
#endif
#include <string>


using namespace std;




DispatchMutex::DispatchMutex()
{}

bool DispatchMutex::TryLock()
{
    return _mutex.try_lock();
}

void DispatchMutex::Lock()
{
    _mutex.lock();
}

void DispatchMutex::Unlock()
{
    _mutex.unlock();
}



DispatchOperation::DispatchOperation(const DispatchWork &work)
:_work(work)
,_state(DispatchOperationState_Created)
,_desirableExecutionTime(-1)
{}

void DispatchOperation::SetState(DispatchOperationState state)
{
     _state = state;
    if (OnStateChanged != nullptr) {
        OnStateChanged();
    }
 }

void DispatchOperation::Cancel()
{
    SetState(DispatchOperationState_Cancelled);
}



DispatchQueue::DispatchQueue()
:_removing(false)
,_time(0)
{}

DispatchQueue::~DispatchQueue()
{
    _removing = true;
    _mutexOperations.lock();
    for (int i = 0; i < _operations.size(); i++) {
        _operations[i]->Cancel();
        _operations[i]->_work();
    }
    _mutexOperations.unlock();
}


bool DispatchQueue::ReadyToExecuteOperation(const DispatchOperationP &operation)
{
    double time = operation->_desirableExecutionTime;
    double timeCurrent = _time;
    if (time < 0) {
        return true;
    }
    return time <= timeCurrent;
}

size_t DispatchQueue::GetOpertaionsCount()
{
    size_t result;
    _mutexOperations.lock();
    result = _operations.size();
    _mutexOperations.unlock();
    return result;
}

bool DispatchQueue::ExecuteNext()
{
    if (_count == 0) {
        return false;
    }
    
    DispatchOperationP operation = nullptr;
    
    _mutexOperations.lock();
    if (_operations.size() == 0)
    {
        _mutexOperations.unlock();
        return false;
    }
    int operationIndex = 0;
    operation = _operations[operationIndex];
    while (!ReadyToExecuteOperation(operation))
    {
        operationIndex ++;
        if (operationIndex == _operations.size())
        {
            _mutexOperations.unlock();
            return false;
        }
        operation = _operations[operationIndex];
    }
    _operations.erase(_operations.begin() + operationIndex);
    _count = _operations.size();
    _mutexOperations.unlock();
    
    if (operation->GetState() == DispatchOperationState_Created)
    {
        operation->SetState(DispatchOperationState_Running);
        operation->_work();
    }
    operation->SetState(DispatchOperationState_Finished);
    
    return true;
}

void DispatchQueue::LaunchExecution(double time)
{}

bool DispatchQueue::GetIsBusy()
{
    return GetOpertaionsCount() > 0;
}

void DispatchQueue::AddOperation(DispatchOperationP operation)
{
#if defined DEBUG
    if (_removing) {
        throw "adding to queue that are about to deallocate";
    }
#endif
    _mutexOperations.lock();
    _operations.push_back(operation);
    _count = _operations.size();
    _mutexOperations.unlock();
    
    if (GetIsBusy()) {
        return;
    }
    LaunchExecution(_time);
}

void DispatchQueue::RemoveOperation(DispatchOperationP operation)
{
    
}

DispatchOperationP DispatchQueue::Perform(const DispatchWork &work)
{
    DispatchOperationP p = DispatchOperationP(new DispatchOperation(work));
    AddOperation(p);
    return p;
}

DispatchOperationP DispatchQueue::PerformWithDelay(const DispatchWork &work, double delay)
{
    return PerformWithTime(work, _time + delay);
}

DispatchOperationP DispatchQueue::PerformWithTime(const DispatchWork &work, double time)
{
    DispatchOperationP p = DispatchOperationP(new DispatchOperation(work));
    p->_desirableExecutionTime = time;
    AddOperation(p);
    return p;
}



DispatchMainQueue::DispatchMainQueue()
{}

void DispatchMainQueue::LaunchExecution(double time)
{
    _time = time;
    while (ExecuteNext())
    {}
}



DispatchBackgroundQueue::DispatchBackgroundQueue()
:_thread(NULL)
{
    
}

DispatchBackgroundQueue::~DispatchBackgroundQueue()
{
    if (_thread != NULL) {
        delete _thread;
        _thread = NULL;
    }
}

void DispatchBackgroundQueue::LaunchExecution(double time)
{
    _time = time;
    _mutex.lock();
    if (_thread == NULL)
    {
        _thread = new DispatchThread(this);
        _thread->start();
    }
    _mutex.unlock();
}

bool DispatchBackgroundQueue::GetIsBusy()
{
    bool result = GetOpertaionsCount() > 0;
    if (!result) {
        return false;
    }
    _mutex.lock();
    result = _thread != NULL;
    _mutex.unlock();
    return result;
}

void DispatchBackgroundQueue::ThreadExecuting(DispatchThread *sender)
{
    while (ExecuteNext())
    {}
}

void DispatchBackgroundQueue::ThreadDidFinish(DispatchThread *sender)
{
    _mutex.lock();
    DispatchThread *t = _thread;
    _thread = NULL;
    _mutex.unlock();
    delete t;
}



Dispatch::Dispatch()
:_mainQueue(DispatchQueueP(new DispatchMainQueue()))
{
    _backgroundQueues.push_back(DispatchQueueP(new DispatchBackgroundQueue()));
    _backgroundQueuesMap.insert(std::pair<std::string, DispatchQueueP>("default", _backgroundQueues[0]));
}

Dispatch *Dispatch::SharedDispatch()
{
    return DispatchPrivate::GetSharedDispatch();
}

void Dispatch::OnAssigned()
{
    _mainThreadId = std::this_thread::get_id();
}

void Dispatch::FlushMainThread(double time)
{
#if DEBUG
    if (!IsMainThread()) {
        throw "Dispatch: FlushMainThread in wrong thread!";
    }
#endif
    for (int i = 0; i < _backgroundQueues.size(); i++) {
        _backgroundQueues[i]->_time = time;
    }
    _mainQueue->LaunchExecution(time);
}

bool Dispatch::IsMainThread()
{
    return _mainThreadId == std::this_thread::get_id();
}

DispatchQueueP Dispatch::GetQueueWithId(const std::string &queueId)
{
    if (queueId == "default") {
        return _mainQueue;
    }
    _mutexBackgroundQueues.lock();
    if (_backgroundQueuesMap.count(queueId) == 1)
    {
        DispatchQueueP result = _backgroundQueuesMap[queueId];
        _mutexBackgroundQueues.unlock();
        return result;
    }
    _mutexBackgroundQueues.unlock();
    return nullptr;
}

DispatchQueueP Dispatch::CreateQueueWithId(const std::string &queueId)
{
    if (GetQueueWithId(queueId) != nullptr)
    {
        // whaaaat?
#if defined DEBUG
        assert(false);
#endif
    }
    DispatchQueueP result = DispatchQueueP(new DispatchBackgroundQueue());
    _mutexBackgroundQueues.lock();
    _backgroundQueues.push_back(result);
    _backgroundQueuesMap.insert(std::pair<std::string, DispatchQueueP>(queueId, result));
    _mutexBackgroundQueues.unlock();
    return result;
}

DispatchOperationP Dispatch::PerformOnMainQueue(const DispatchWork &work)
{
    return SharedDispatch()->_mainQueue->Perform(work);
}

DispatchOperationP Dispatch::PerformOnMainQueueWithDelay(const DispatchWork &work, double delay)
{
    return SharedDispatch()->_mainQueue->PerformWithDelay(work, delay);
}

DispatchOperationP Dispatch::PerformOnMainQueueWithTime(const DispatchWork &work, double time)
{
    return SharedDispatch()->_mainQueue->PerformWithTime(work, time);
}

DispatchOperationP Dispatch::PerformOnBackgroundQueue(const DispatchWork &work)
{
    return SharedDispatch()->_backgroundQueues[0]->Perform(work);
}

DispatchOperationP Dispatch::PerformSynchronousOnMainQueue(const DispatchWork &work)
{
#if DEBUG
    if (SharedDispatch()->IsMainThread()) {
        throw "Dispatch: PerformSynchronousOnMainQueue from main queue cause deadlock!";
    }
#endif
    std::shared_ptr<DispatchLock> sync = std::shared_ptr<DispatchLock>(new DispatchLock());
    auto result = SharedDispatch()->_mainQueue->Perform([sync, &work]()->void {
        work();
        sync->Unlock();
    });
    sync->Lock();
    return result;
}

DispatchOperationP Dispatch::PerformSynchronousOnBackgroundQueue(const DispatchWork &work)
{
    std::shared_ptr<DispatchLock> sync = std::shared_ptr<DispatchLock>(new DispatchLock());
    auto result = SharedDispatch()->_backgroundQueues[0]->Perform([sync, &work]()->void {
        work();
        sync->Unlock();
    });
    sync->Lock();
    return result;
}

void Dispatch::Sleep(unsigned long long miliseconds)
{
    std::this_thread::sleep_for(chrono::milliseconds(miliseconds));
}
