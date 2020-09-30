#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// хз зачем у урхо свой вектор
#include <Urho3D/Container/Vector.h>
using namespace Urho3D;

template <typename ContainerType>
inline void sort(ContainerType& v) {
    std::sort(v.begin(), v.end());
}

template <typename ContainerType, typename FuncType>
inline void sort(ContainerType& v, FuncType f) {
    std::sort(v.begin(), v.end(), f);
}

template <typename ContainerType, typename FuncType>
void each(const ContainerType& src, FuncType f) {
    for (const auto& it : src) {
        f(it);
    }
}

/*
template <typename KeyType, typename ItemType, typename FuncType>
void each(const std::map<KeyType, ItemType>& srcMap, std::function<void(const ItemType& v)> f) {
    for (const auto& it : srcMap) {
        f(it->second);
    }
}

template <typename KeyType, typename ItemType, typename FuncType>
void each(const std::map<KeyType, ItemType>& srcMap, std::function<void(const KeyType& k, const ItemType& v)> f) {
    for (const auto& it : srcMap) {
        f(it->first, it->second);
    }
}


template <typename KeyType, typename ItemType, typename FuncType>
void each(const std::unordered_map<KeyType, ItemType>& srcMap, std::function<void(const ItemType& v)> f) {
    for (const auto& it : srcMap) {
        f(it->second);
    }
}

template <typename KeyType, typename ItemType, typename FuncType>
void each(const std::unordered_map<KeyType, ItemType>& srcMap, std::function<void(const KeyType& k, const ItemType& v)> f) {
    for (const auto& it : srcMap) {
        f(it->first, it->second);
    }
}*/


/// дергает вектор всех ключей
template <typename TKey, typename TValue>
std::vector<TKey> keys(const std::map<TKey, TValue>& srcMap) {
    std::vector<TKey> result;
    for (const std::pair<TKey, TValue>& pairVal : srcMap) {
        result.emplace_back(pairVal.first);
    }
    return result;
}
  
/// проверяет, есть указанный объект в коллекции
template <typename ContainerType, typename ItemType>
inline bool contains(const ContainerType& src, const ItemType& object) {
    return std::find(src.begin(), src.end(), object) != src.end();
}

/// проверяет, есть указанный объект в листе
template <typename T>
inline bool contains(const std::list<T>& src, const T& object) {
    return contains<std::list<T>, T>(src, object);
}

/// проверяет, есть указанный объект в векторе
template <typename T>
inline bool contains(const std::vector<T>& src, const T& object) {
    return contains<std::vector<T>, T>(src, object);
}

/// проверяет, есть указанный объект в коллекции по предикату
template <typename ContainerType, typename ItemType, typename TypeMappe>
inline bool contains_if(const ContainerType& src, TypeMappe f) {
    return std::find_if(src.begin(), src.end(), f) != src.end();
}

/// проверяет, есть указанный объект в векторе по предикату
template <typename T, typename TypeMapper>
inline bool contains_if(const std::vector<T>& src, TypeMapper f) {
    return contains_if<std::vector<T>, T, TypeMapper>(src, f);
}

/// проверяет, есть указанный объект в сете
template <typename T>
inline bool contains(const std::set<T>& src, const T& object) {
    return src.find(object) != src.end();
}

/// проверяет, есть ли указанный элемент в map
template <typename TKey, typename TValue>
bool contains(const std::map<TKey, TValue>& srcMap, const TValue& value) {
    for (const auto& pair : srcMap) {
        if (pair.second == value)
            return true;
    }
    return false;
}

/// проверяет, есть ли указанный ключ в map
template <typename TKey, typename TValue>
inline bool containsKey(const std::map<TKey, TValue>& srcMap, const TKey& key) {
    return srcMap.find(key) != srcMap.end();
}

/// проверяет, есть ли указанный ключ в unordered_map
template <typename TKey, typename TValue>
inline bool containsKey(const std::unordered_map<TKey, TValue>& srcMap, const TKey& key) {
    return srcMap.find(key) != srcMap.end();
}

template <typename ContainerType, typename ItemType>
int indexOf(ContainerType& src, const ItemType& item) {
    auto it = std::find(src.begin(), src.end(), item);
    if (it != src.end()) {
        int index = std::distance(src.begin(), it);
        return index;
    }
    return -1;
}

/// определяет индекс итема в списке, либо -1, если не найден
template <typename T>
int indexOf(std::list<T>& src, const T& item) {
    return indexOf<std::list<T>, T>(src, item);
}

/// определяет индекс итема в векторе, либо -1, если не найден
template <typename T>
int indexOf(std::vector<T>& src, const T& item) {
    return indexOf<std::vector<T>, T>(src, item);
}


template <typename MapType, typename ItemType, typename ItemIdType>
inline ItemType getByKey(const MapType& container, const ItemIdType& id) {
    const auto& it = container.find(id);
    if (it != container.end()) {
        return it->second;
    }
    return ItemType();
}

template <typename ItemType, typename ItemIdType>
ItemType getByKey(const std::unordered_map<ItemIdType, ItemType>& container, const ItemIdType& id) {
    return getByKey<std::unordered_map<ItemIdType, ItemType>, ItemType, ItemIdType>(container, id);
}

template <typename ItemType, typename ItemIdType>
ItemType getByKey(const std::map<ItemIdType, ItemType>& container, const ItemIdType& id) {
    return getByKey<std::map<ItemIdType, ItemType>, ItemType, ItemIdType>(container, id);
}


template <typename ItemType, typename ItemIdType>
ItemType getByKey(const HashMap<ItemIdType, ItemType>& container, const ItemIdType& id) {
    const auto& it = container.Find(id);
    if (it != container.End()) {
        return it->second_;
    }
    return ItemType();
}

template <typename MapType, typename ItemType, typename ItemIdType>
inline const ItemType& getConstByKey(const MapType& container, const ItemIdType& id) {
    const auto& it = container.find(id);
    if (it != container.end()) {
        return it->second;
    }
    static const ItemType emptyItem;
    return emptyItem;
}

template <typename ItemType, typename ItemIdType>
const ItemType& getConstByKey(const std::unordered_map<ItemIdType, ItemType>& container, const ItemIdType& id) {
    return getConstByKey<std::unordered_map<ItemIdType, ItemType>, ItemType, ItemIdType>(container, id);
}

template <typename ItemType, typename ItemIdType>
const ItemType& getConstByKey(const std::map<ItemIdType, ItemType>& container, const ItemIdType& id) {
    return getConstByKey<std::map<ItemIdType, ItemType>, ItemType, ItemIdType>(container, id);
}

/// ищет элемент в контейнере по условию
template <typename ContainerType, typename ItemType, typename TypeMapper>
ItemType find(const ContainerType& container, TypeMapper f) {
    const auto& it = std::find_if(container.begin(), container.end(), f);
    if (it != container.end()) {
        return *it;
    }
    return ItemType();
}

template <typename T, typename TypeMapper>
inline T find(const std::vector<T>& src, TypeMapper f) {
    return find<std::vector<T>, T>(src, f);
}

/// --------------------------------------------------------------------
/// фильтрация контейнеров:
/// удаляются элементы, удовлетворяющие условию

template <typename T, typename TypeMapper>
void filterDirect(std::vector<T>& v, TypeMapper f) {
    auto it = std::remove_if(v.begin(), v.end(), f);
    v.erase(it, v.end());
}

template <typename T, typename TypeMapper>
void filterDirect(std::set<T>& v, TypeMapper f) {
    for (auto it = v.begin(); it != v.end();) {
        if (f(*it)) {
            it = v.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename T, typename TypeMapper>
void filterDirect(std::list<T>& v, TypeMapper f) {
    v.remove_if(f);
}

template <typename T1, typename T2, typename TypeMapper>
void filterDirect(std::unordered_map<T1, T2>& v, TypeMapper f) {
    for (auto it = v.begin(); it != v.end();) {
        if (f(it->second)) {
            it = it.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename T1, typename T2, typename TypeMapper>
T1 filter(const T1& v, TypeMapper f) {
    /// TODO: делать без копирования?
    T1 vv = v;
    filterDirect(vv, f);
    return vv;
}

template <typename T1, typename T2, typename T3, typename TypeMapper>
T1 filter(const T1& v, TypeMapper f) {
    /// TODO: делать без копирования?
    auto vv = v;
    filterDirect(vv, f);
    return vv;
}

template <typename T, typename TypeMapper>
inline std::list<T> filter(const std::list<T>& v, TypeMapper f) {
    return filter<std::list<T>, T>(v, f);
}

template <typename T, typename TypeMapper>
inline std::vector<T> filter(const std::vector<T>& v, TypeMapper f) {
    return filter<std::vector<T>, T>(v, f);
}

template <typename T1, typename T2, typename TypeMapper>
inline std::unordered_map<T1, T2> filterMap(const std::unordered_map<T1, T2>& v, TypeMapper f) {
    return filter<std::unordered_map<T1, T2>, T1, T2>(v, f);
}

/// ------------------------------------------------------------
/// removes - удаление конкретных итемов из контейнеров

/// удаляет указанные объекты из вектора
template <typename T>
inline void remove(std::list<T>& src, const std::vector<T>& removingItems) {
    filterDirect<T>(src, [&removingItems](const T& obj) {
        return contains(removingItems, obj);
    });
}

/// удаляет указанные объекты из вектора
template <typename T>
inline void remove(std::vector<T>& src, const std::vector<T>& removingItems) {
    filterDirect<T>(src, [&removingItems](const T& obj) {
        return contains(removingItems, obj);
    });
}

/// удаляет указанный объект из вектора
template <typename T>
inline void remove(std::vector<T>& src, const T& object) {
    src.erase(std::remove(src.begin(), src.end(), object));
}

/// удаляет указанный объект из листа
template <typename T>
inline void remove(std::list<T>& src, const T& object) {
    src.remove(object);
}

template <typename T>
inline void remove(std::set<T>& src, const T& object) {
    src.erase(object);
}

template <typename T>
inline void remove(std::unordered_set<T>& src, const T& object) {
    src.erase(object);
}


template <typename K, typename T>
inline void remove(std::unordered_map<K, T>& src, const T& object) {
    for(auto it = src.begin(); it!= src.end();){
        if (object == it->second) it = src.erase(it); else it++;
    }
}


template <typename K, typename T>
inline void remove(std::map<K, T>& src, const T& object) {
    for(auto it = src.begin(); it!= src.end();){
        if (object == it->second) it = src.erase(it); else it++;
    }
}


template <typename K, typename T>
inline void removeKey(std::unordered_map<K, T>& src, const K& key) {
    src.erase(key);
}

template <typename K, typename T>
inline void removeKey(std::map<K, T>& src, const K& key) {
    src.erase(key);
}

///
/// добавление итемов в коллекцию

template <typename ItemType>
inline void add(std::vector<ItemType>& container, const ItemType& item) {
    container.push_back(item);
}

template <typename ItemType>
inline void add(std::vector<ItemType>& container, ItemType&& item) {
    container.emplace_back(item);
}

template <typename ItemType>
inline void add(std::set<ItemType>& container, const ItemType& item) {
    container.insert(item);
}

template <typename ItemType>
inline void add(std::unordered_set<ItemType>& container, const ItemType& item) {
    container.insert(item);
}

template <typename ItemType>
inline void add(std::set<ItemType>& container, ItemType&& item) {
    container.emplace(item);
}

template <typename ItemType>
inline void add(std::unordered_set<ItemType>& container, ItemType&& item) {
    container.emplace(item);
}

template <typename ItemType>
inline void add(std::set<ItemType>& container, const std::set<ItemType>& container2) {
    container.insert(container2.begin(), container2.end());
}

template <typename ItemType, typename CollectionType2>
void add(std::vector<ItemType>& container, const CollectionType2& container2) {
    container.insert(container.end(), container2.begin(), container2.end());
}

template <typename ItemType, typename CollectionType2>
void add(std::set<ItemType>& container, const CollectionType2& container2) {
    container.insert(container2.begin(), container2.end());
}

template <typename ItemType, typename CollectionType2>
void add(std::unordered_set<ItemType>& container, const CollectionType2& container2) {
    container.insert(container2.begin(), container2.end());
}

template <typename ItemType>
void addUnique(std::vector<ItemType>& container, const ItemType& item) {
    if (!contains(container, item))
        container.push_back(item);
}

template <typename ItemType>
void addUnique(std::list<ItemType>& container, const ItemType& item) {
    if (!contains(container, item))
        container.push_back(item);
}

template <typename ItemType>
inline void addUnique(std::set<ItemType>& container, const ItemType& item) {
    container.insert(item);
}

/// --------------------------------------------------------------------
/// маппинг контейнеров - перегнать коллекцию одних элементов в другие

template <typename ContainerType1, typename ContainerType2, typename T1, typename T2, typename TypeMapper>
ContainerType2 map(const ContainerType1& v, TypeMapper f) {
    ContainerType2 vv;
    for (const T1& it : v)
        vv.emplace_back(f(it));
    return vv;
}

template <typename T1, typename T2, typename ContainerType2, typename TypeMapper>
inline ContainerType2 map(const std::initializer_list<T1>& v, TypeMapper f) {
    return map<std::vector<T1>, ContainerType2, T1, T2>(v, f);
}

template <typename T1, typename T2, typename TypeMapper>
inline std::vector<T2> map(const std::vector<T1>& v, TypeMapper f) {
    return map<std::vector<T1>, std::vector<T2>, T1, T2>(v, f);
}

template <typename T1, typename T2, typename TypeMapper>
inline Vector<T2> map(const Vector<T1>& v, TypeMapper f) {
    return map<Vector<T1>, Vector<T2>, T1, T2>(v, f);
}


template <typename T1, typename T2, typename TypeMapper>
inline std::vector<T2> map(const PODVector<T1>& v, TypeMapper f) {
    return map<PODVector<T1>, std::vector<T2>, T1, T2>(v, f);
}


template <typename T1, typename T2, typename TypeMapper>
inline std::list<T2> map(const std::list<T1>& v, TypeMapper f) {
    return map<std::list<T1>, std::list<T2>, T1, T2>(v, f);
}



#endif
