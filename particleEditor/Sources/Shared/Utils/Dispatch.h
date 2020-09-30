//
//  Dispatch.h
//  MAX
//
//  Created by Katekov Anton on 9/4/17.
//  Copyright © 2017 AntonKatekov. All rights reserved.
//

#ifndef Dispatch_hpp
#define Dispatch_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <Shared/Utils/DispatchThread.h>



class DispatchQueue;
class DispatchOperation;



typedef enum {
    
    DispatchOperationState_Created,
    DispatchOperationState_Running,
    DispatchOperationState_Finished,
    DispatchOperationState_Cancelled
    
} DispatchOperationState;

typedef std::shared_ptr<DispatchOperation> DispatchOperationP;
typedef std::weak_ptr<DispatchQueue> DispatchQueueW;
typedef std::function<void()> DispatchWork;



class DispatchMutex {
    
    std::mutex _mutex;
    
    DispatchMutex(const DispatchMutex &o){};
    void operator=(const DispatchMutex &o){}
    
public:
    
    DispatchMutex();
    
    bool TryLock();
    void Lock();
    void Unlock();
    
};



class DispatchOperation {
    
    friend class DispatchQueue;
    
    double _desirableExecutionTime;
    
    DispatchWork _work;
    std::atomic<DispatchOperationState> _state;
    
    void SetState(DispatchOperationState state);
    
public:
    
    std::function<void()> OnStateChanged;
    
    DispatchOperation(const DispatchWork &work);

    DispatchOperationState GetState() { return _state; };
    
    void Cancel();
    
};



class DispatchQueue {
    
    bool _removing;
    
    friend class Dispatch;
    
protected:
    
    std::atomic<double> _time;
    
    std::atomic<size_t> _count;
    std::vector<DispatchOperationP> _operations;
    std::mutex _mutexOperations;
    
    bool ReadyToExecuteOperation(const DispatchOperationP &operation);
    
    size_t GetOpertaionsCount();
    bool ExecuteNext();
    virtual void LaunchExecution(double time);
    virtual bool GetIsBusy();
    
public:
    
    DispatchQueue();
    virtual ~DispatchQueue();
    
    void AddOperation(DispatchOperationP operation);
    void RemoveOperation(DispatchOperationP operation);
    
    
    DispatchOperationP Perform(const DispatchWork &work);
    DispatchOperationP PerformWithDelay(const DispatchWork &work, double delay);
    DispatchOperationP PerformWithTime(const DispatchWork &work, double time);
    
};



class DispatchMainQueue : public DispatchQueue {
    
protected:
    
    void LaunchExecution(double time) override;
    
public:
    
    DispatchMainQueue();
    
};



class DispatchBackgroundQueue : public DispatchQueue, public DispatchThreadDelegate {
    
    std::mutex _mutex;
    DispatchThread *_thread;
    
protected:
    
    void LaunchExecution(double time) override;
    bool GetIsBusy() override;
    
public:
    
    DispatchBackgroundQueue();
    ~DispatchBackgroundQueue();
    
#pragma mark - DispatchThreadDelegate
    void ThreadExecuting(DispatchThread *sender) override;
    void ThreadDidFinish(DispatchThread *sender) override;
    
};

typedef std::shared_ptr<DispatchQueue> DispatchQueueP;



class Dispatch {
    
protected:
    
    std::map<std::string, DispatchQueueP> _backgroundQueuesMap;
    std::vector<DispatchQueueP> _backgroundQueues;
    std::mutex _mutexBackgroundQueues;
    
    std::thread::id _mainThreadId;
    DispatchQueueP _mainQueue;
    
    virtual void OnAssigned();
    
public:
    
    Dispatch();
    static Dispatch *SharedDispatch();
    
    void FlushMainThread(double time);
    bool IsMainThread();
    DispatchQueueP GetQueueWithId(const std::string &queueId);
    DispatchQueueP CreateQueueWithId(const std::string &queueId);
    
    static DispatchOperationP PerformOnMainQueue(const DispatchWork &work);
    static DispatchOperationP PerformOnMainQueueWithDelay(const DispatchWork &work, double delay);
    static DispatchOperationP PerformOnMainQueueWithTime(const DispatchWork &work, double time);
    static DispatchOperationP PerformOnBackgroundQueue(const DispatchWork &work);
    
    static DispatchOperationP PerformSynchronousOnMainQueue(const DispatchWork &work);
    static DispatchOperationP PerformSynchronousOnBackgroundQueue(const DispatchWork &work);
    static DispatchOperationP PerformSynchronousOnBackgroundQueue(const DispatchWork &work, DispatchQueueP queue);
    
    static void Sleep(unsigned long long miliseconds);
    
};
#endif /* Dispatch_h */
