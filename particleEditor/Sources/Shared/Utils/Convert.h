#ifndef PROJECTN_CONVERT_H
#define PROJECTN_CONVERT_H
#pragma once


#include <Defines.h>
#include <Shared/Utils/ForEach.h>
#include <Urho3D/Resource/JSONValue.h>

inline bool is_digit(char c){ return c>='0'&&c<='9'; }
inline bool is_int(char c){ return is_digit(c) || c=='-' || c=='+'; }
inline bool is_float(char c){ return is_int(c) || c=='.'; }
inline bool is_hexdigit(char c){ return (c>='0'&&c<='9')||(c>='A'&&c<='F'); }
inline bool is_letter(char c){ return (c>='a'&&c<='z') || (c>='A'&&c<='Z'); }

const JSONValue& at(const JSONArray& v, unsigned index);
const JSONValue& at(const JSONValue& v, unsigned index);

// SFINAE test
// https://gist.github.com/fenbf/d2cd670704b82e2ce7fd
#define HAS_METHOD_HELPER(method) \
template <typename T> \
class Has_##method \
{ \
private: \
    typedef char YesType[1]; \
    typedef char NoType[2]; \
    template <typename C> static YesType& test( decltype(&C :: method) ) ; \
    template <typename C> static NoType& test(...); \
public: \
    enum { value = sizeof(test<T>(0)) == sizeof(YesType) }; \
};

HAS_METHOD_HELPER(ToString)
HAS_METHOD_HELPER(toJson)
HAS_METHOD_HELPER(fromJson)


/*
template <typename T1, typename T2, typename std::enable_if<
             !std::is_trivially_destructible<T2>{} &&
             (std::is_class<T2>{} || std::is_union<T2>{}),
            int>::type = 0>
T1 to(const T2& v);
*/

template <typename T1, typename T2>
// если типы разные и не строка/json (для них отдельная песня)
typename std::enable_if_t<!std::is_same<T1, String>::value && !std::is_same<T1, JSONValue>::value && !std::is_same<T2, JSONValue>::value && !std::is_same<T1, T2>::value && !std::is_enum<T1>::value, T1> 
to(const T2& v);

template <typename T1, typename T2>
// если типы разные и не строка/json (для них отдельная песня)
typename std::enable_if_t<std::is_same<T2, JSONValue>::value && std::is_trivial<T1>::value && !std::is_enum<T1>::value, T1> 
to(const T2& v);

template <typename T1, typename T2>
inline typename std::enable_if_t<std::is_same<T1, T2>::value, T1> 
to(const T2& v) { return v; }


template <typename T1, typename T2>
typename std::enable_if_t<!std::is_same<T1, String>::value, T1> // для строк отдельная песня
to(const T2* v);



// специализации для преобразования чего-то с методом ToString в строку

template <typename T1, typename T2>
typename std::enable_if_t<std::is_same<T1, String>::value && !std::is_same<T2, String>::value && !std::is_enum<T2>::value && !Has_ToString<T2>::value, String>
to(const T2& v);

template <typename T1, typename T2>
typename std::enable_if_t<std::is_same<T1, std::string>::value && !std::is_same<T1, std::string>::value && !std::is_enum<T2>::value && !Has_ToString<T2>::value, std::string>
to(const T2& v);

template < typename T1, typename T2 >
typename std::enable_if_t<std::is_same<T1, String>::value && Has_ToString<T2>::value, T1>
inline to(const T2& t) {
   return t.ToString();
}

template < typename T1, typename T2 >
typename std::enable_if_t<std::is_same<T1, std::string>::value && Has_ToString<T2>::value, T1>
inline to(const T2& t) {
   return t.ToString();
}


template < typename T1, typename T2 >
typename std::enable_if_t<std::is_same<T1, JSONValue>::value && Has_toJson<T2>::value, T1>
inline to(const T2& t) {
   return t.toJson();
}

template <typename T1, typename T2>
typename std::enable_if_t<std::is_same<T1, JSONValue>::value && !Has_toJson<T2>::value && !std::is_same<T1, T2>::value && !(std::is_trivial<T2>::value || std::is_same<T2, String>::value) && !std::is_enum<T2>::value, T1> 
to(const T2& v);

template <typename T1, typename T2>
typename std::enable_if_t<std::is_same<T2, JSONValue>::value && !Has_fromJson<T1>::value && !std::is_same<T1, T2>::value && !(std::is_trivial<T1>::value || std::is_same<T1, String>::value) && !std::is_enum<T1>::value, T1> 
to(const T2& v);

template <typename T1, typename T2>
typename std::enable_if_t<std::is_same<T1, JSONValue>::value && (std::is_trivial<T2>::value || std::is_same<T2, String>::value) && !std::is_enum<T2>::value, T1> 
to(const T2& v);


template < typename T1, typename T2 >
typename std::enable_if_t<std::is_same<T2, JSONValue>::value && Has_fromJson<T1>::value, T1>
inline to(const T2& t) {
   T1 s;
   s.fromJson(t);
   return s;
}





// enum to string converts

template<typename T>
struct enums
{
    static const std::unordered_map<T, const char*> data;
    static const std::vector<T> all;
};


/// макрос ENUM_CONVERT_HELPER - позволяет переводить более легко енумы в строку и обратно

#define _ENUM_ECHO(EnumType, x) EnumType::x,
#define _ENUM_ECHO_M_STR(EnumType, x) { EnumType::x, #x },
#define ENUM_CONVERT_HELPER( EnumType, ... ) \
template<> const std::unordered_map<EnumType, const char*> enums<EnumType>::data = { FOREACH3( _ENUM_ECHO_M_STR, EnumType, __VA_ARGS__ ) }; \
template<> const std::vector<EnumType> enums<EnumType>::all = { FOREACH3( _ENUM_ECHO, EnumType, __VA_ARGS__ ) }; 
 

template<typename T1, typename EnumType>
typename std::enable_if<std::is_same<T1, String>::value && std::is_enum<EnumType>::value, T1>::type
to(const EnumType& e){
    const char* c = getByKey( enums<EnumType>::data, e );
    return c ? c : "";
}


template<typename EnumType, typename T1>
typename std::enable_if<std::is_same<T1, String>::value && std::is_enum<EnumType>::value, EnumType>::type
to(const T1& e){
    for (auto& it : enums<EnumType>::data ){
        if (e == it.second) return it.first;
    }
    return (EnumType)0;
}

// known warning -Wundefined-var-template

template<typename EnumType, typename T1>
typename std::enable_if<std::is_same<T1, JSONValue>::value && std::is_enum<EnumType>::value, EnumType>::type
to(const T1& v){
    return v.IsString() ? to<EnumType>(v.GetString()) : v.IsNumber() ? (EnumType)v.GetInt() : EnumType();
}

template<typename T1, typename EnumType>
typename std::enable_if<std::is_same<T1, JSONValue>::value && std::is_enum<EnumType>::value, T1>::type
to(const EnumType& e){
    return (int)e;
}


template<typename EnumType, typename T1>
typename std::enable_if<std::is_same<T1, Variant>::value && std::is_enum<EnumType>::value, EnumType>::type
to(const T1& v){
    switch (v.GetType()){
        case VAR_STRING: return to<EnumType>(v.GetString());
        case VAR_INT64: 
        case VAR_INT: return (EnumType)v.GetInt();
        default: return EnumType();
    }
}

template<typename T1, typename EnumType>
typename std::enable_if<std::is_same<T1, Variant>::value && std::is_enum<EnumType>::value, T1>::type
to(const EnumType& e){
    return (int)e;
}
 

// хрень, которая сама разбирает по типам аргументов как ей чего конвертить
template < typename T1, typename T2 >
inline void assign(const T1& v, T2& v2){
    v2 = to<T2>(v);
}

template < typename T1, typename T2 >
inline typename std::enable_if_t<std::is_same<T1, JSONValue>::value && Has_fromJson<T2>::value, void>
assign(const T1& v, T2* & v2){
    if (!v2) v2 = new T2(); v2->fromJson(v);
}
 
#endif
