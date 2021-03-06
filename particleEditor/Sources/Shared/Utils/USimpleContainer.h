#ifndef USimpleContainer_h
#define USimpleContainer_h

#include <vector>
#include <cstring>

namespace Utils {
    
    using namespace std;
    
    template<typename T>
    class USimpleContainer {
        T *_array;
        int         _baseSize;
        int         _count;
        int         _currentSize;
        
    public:
        
        T *GetArrayPointer() {return _array;};
        
        
        USimpleContainer();
        USimpleContainer(int baseSize);
        ~USimpleContainer();
        
        void remove(int index);
        bool removeObject(const T& object);
        void addObject(const T& object);
        int indexOf(const T& object);
        void addPlace(unsigned int itemCount);
        void unsafeObjectsAdded(int count) { _count += count; }
        void AddObjects(const USimpleContainer<T>* objects);
        void clear();
        void replaceObjectAtIndex(int index, const T &object);
        T objectAtIndex(int index) const;
        void sort(int (* pointer)(const void *, const void *));
        int GetSize() const { return _currentSize; };
        int GetCount() const { return _count; };
        vector<T> ToSTDVector();
    };
    
    template <typename T>
    vector<T> USimpleContainer<T>::ToSTDVector() {
		vector<T> result;
        for (int i = 0; i < _count; i++) {
            result.push_back(_array[i]);
        }
        return result;
    }

    template <typename T>
    USimpleContainer<T>::USimpleContainer()
    :_count(0) {
		_baseSize = 100;
        _array = new T[_baseSize];
        memset(_array, 0, sizeof(T) * _baseSize);
        _currentSize = _baseSize;
    }
    
    template<typename T>
    USimpleContainer<T>::USimpleContainer(int baseSize)
    :_count(0) {
        _baseSize = baseSize;
        _array = new T[_baseSize];
        memset(_array, 0, sizeof(T) * _baseSize);
        _currentSize = _baseSize;
    }
    
    template <typename T>
    USimpleContainer<T>::~USimpleContainer() {
        clear();
        delete [] _array;
    }
    
    template <typename T>
    void USimpleContainer<T>::remove(int index) {
        if (index < _count) {
//            T obj  = _array[index];
            
            _array[index] = _array[_count - 1];
            _count--;
        }
    }
    
    template <typename T>
    int USimpleContainer<T>::indexOf(const T &object) {
         for (int i = 0; i < _count; i++) {
            if(_array[i] == object)
                return i;
        }
        return -1;
    }
    
    template <typename T>
    void USimpleContainer<T>::addPlace(unsigned int itemCount)
    {
        T *tmp = _array;
        _currentSize = _currentSize + itemCount;
        _array = new T[_currentSize];
        for (int i = 0; i < _count; i++)
            _array[i] = tmp[i];
        delete [] tmp;
    
        _count += itemCount;
    }

    template <typename T>
    void USimpleContainer<T>::clear() {
        delete [] _array;
        _array = new T[_currentSize];
        memset(_array, 0, _currentSize*sizeof(T));
        _count = 0;
    }
    
    template <typename T>
    bool USimpleContainer<T>::removeObject(const T& object) {
        for (int i = 0; i < _count; i++) {
            if (_array[i] == object) {
                remove(i);
                return true;
            }
        }
        return false;
    }
    
    template <typename T>
    void USimpleContainer<T>::addObject(const T& object) {
        if (_count == _currentSize) {
            T *tmp = _array;
            _currentSize = _currentSize + _baseSize;
            _array = new T[_currentSize];
            for (int i = 0; i < _count; i++)
                _array[i] = tmp[i];
            delete [] tmp;
        }
        _array[_count] = object;
        _count ++;
    }
    
    template <typename T>
    void USimpleContainer<T>::replaceObjectAtIndex(int index, const T &object)
    {
        _array[index] = object;
    }
    
    template <typename T>
    T USimpleContainer<T>::objectAtIndex(int index) const {
        T result = _array[index];
        return result;
    }
    
    template <typename T>
    void USimpleContainer<T>::sort(int (*pointer)(const void *, const void *)) {
        qsort(_array, _count, sizeof(T), pointer);
    }
    
    template <typename T>
    void USimpleContainer<T>::AddObjects(const USimpleContainer<T>* objects)
    {
        int newCount = _count + objects->GetCount();
        if (_currentSize < newCount)
        {
            int need = newCount - _currentSize;
            int times = need/_baseSize;
            if (times * _baseSize < need) 
                times ++;
            int newSize = _currentSize + _baseSize * times;
            
            T *tmp = _array;
            _array = new T[newSize];
            for (int i = 0; i < _count; i++)
                _array[i] = tmp[i];
            
            _currentSize = newSize;
            delete [] tmp;
        }
        memcpy(_array + _count, objects->_array, objects->GetCount() * sizeof(T));
        
        _count = newCount;
    }
}

#endif
