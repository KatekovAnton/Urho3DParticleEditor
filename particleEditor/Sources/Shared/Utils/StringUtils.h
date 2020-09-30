//
//  StringUtils.h
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <Defines.h>

std::vector<std::string> splitTest ( std::string input , std::string split_id );

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

std::vector<std::string> split(const std::string &s, const std::string &delim);

std::vector<std::string> getElements(const std::string &value);

std::vector<std::string> removeCommentsAndEmptyStrings(const std::vector<std::string> &lines);

std::string intToString(int value);
std::string longToString(long value);

std::string floatToString3(float value);
std::string floatToString2(float value);
std::string floatToString1(float value);
std::string floatToString(float value);
float stringToFloat(const std::string &value);

void removeBadCharacters(const std::string &param);

std::string toLower(std::string value);
std::string toUpper(std::string value);

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

void pushBackIfNotExists(std::vector<std::string> &vector, std::string &value);
void pushBackIfNotExists(std::vector<std::string> &vector, const char *value);

std::string formattedTimeAgoString(unsigned long long time);

String repeat(char c, size_t num);
String repeat(String s, size_t num);
  
String stringify(const JSONValue& v, bool pretty = false );

    
//-- удали меня --
//
//я буду такие комменты оставлять иногда, требующие внимания, ок?
//зачем тут структура StrUtils? давай в общую область видимости.
//


struct StrUtils {

    static void ToStdWString(std::string const &src, std::wstring &dst);
    static std::wstring ToStdWString(std::string const &src);
    static void ToStdString(std::wstring const &src, std::string &dst);
    static std::string ToStdString(std::wstring const &src);

    static std::string ToLowerASCII(std::string const &str);
    
    static bool endsWith(std::string const &str, std::string const &suffix);
    static bool startsWith(std::string const &str, std::string const &prefix);
    
    static std::wstring stringByReplacing(const std::wstring &source, const std::wstring &whatToReplace, const std::wstring &toReplaceWith);
    static void splitString(const ::std::string& str, char delimiter, ::std::vector< ::std::string>* dest);

    StrUtils() = delete;
    StrUtils(StrUtils const &) = delete;
    StrUtils & operator=(StrUtils const &) = delete;
    StrUtils(StrUtils &&) = delete;
    StrUtils & operator=(StrUtils &&) = delete;
    ~StrUtils() = default;
};

#endif /* defined(STRING_UTILS_H) */
