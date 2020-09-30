#include "EngineAnimationManager.h"
#include "EngineAnimationBase.h"



EngineAnimationManager::EngineAnimationManager()
:_currentTime(0)
{
    
}

EngineAnimationManager::~EngineAnimationManager()
{
    std::list<EngineAnimationBase*>::const_iterator iterator;
    for (iterator = _objects.begin(); iterator != _objects.end(); ++iterator)
    {
        EngineAnimationBase* item = *iterator;
        delete item;
    }
}

void EngineAnimationManager::AddAnimatedObject(EngineAnimationBase* object)
{
    _objects.push_back(object);
    object->BaseStart(_currentTime);
}

void EngineAnimationManager::RemoveAnimatedObject(EngineAnimationBase* object)
{
    _objectsForRemove.push_back(object);
    object->_skip = true;
}

void EngineAnimationManager::Update(double currentTime, double deltaTime)
{
    _currentTime = currentTime;
    for (int i = 0; i < _objectsForRemove.size(); i++)
    {
        EngineAnimationBase *anim = _objectsForRemove[i];
        _objects.remove(anim);
        delete anim;
    }
    _objectsForRemove.clear();
    
    std::list<EngineAnimationBase*>::const_iterator iterator;
    for (iterator = _objects.begin(); iterator != _objects.end(); ++iterator)
    {
        EngineAnimationBase* item = *iterator;
        if (!item->_skip) {
            item->BaseUpdate(currentTime, deltaTime);
        }
    }
    for (int i = 0; i < _objectsForRemove.size(); i++)
    {
        EngineAnimationBase *anim = _objectsForRemove[i];
        _objects.remove(anim);
        delete anim;
    }
    _objectsForRemove.clear();
    
    bool toRemove = true;
    while (toRemove)
    {
        toRemove = false;
    
        int count = 0;
        for (iterator = _objects.begin(); iterator != _objects.end(); ++iterator)
        {
            count ++;
            EngineAnimationBase* item = *iterator;
            if (item->IsFinished())
            {
                toRemove = true;
                item->BaseCompletlyFinish();
                break;
            }
        }
        if(toRemove)
        {
            count --;
			try
            {
				EngineAnimationBase* item = *iterator;
                delete item;
            }
            catch(...)
            {
				
            }
            _objects.remove(*iterator);
        }
    }
    
    _objectsForRemove.clear();
}
