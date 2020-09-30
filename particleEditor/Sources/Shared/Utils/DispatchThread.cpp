//
//  DispatchThread.cpp
//  MAX
//
//  Created by Katekov Anton on 8/4/18.
//  Copyright © 2018 AntonKatekov. All rights reserved.
//

#include "DispatchThread.h"



DispatchThread::DispatchThread(DispatchThreadDelegate *delegate)
:_delegate(delegate)
{}

void DispatchThread::run()
{
    _delegate->ThreadExecuting(this);
    _delegate->ThreadDidFinish(this);
}

