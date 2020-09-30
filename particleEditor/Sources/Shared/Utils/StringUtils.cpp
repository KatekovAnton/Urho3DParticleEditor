//
//  StringUtils.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "StringUtils.h"
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <locale>
#include <cassert>
#include <algorithm>
#include <cctype>



using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    string::size_type pos = 0;
    while (true)
    {
        string::size_type startPos = s.find_first_of(delim, pos);
        if (startPos == std::string::npos)
        {
            if (pos != s.length())
                elems.push_back(s.substr(pos, s.length() - pos));
            
            break;
        }
        elems.push_back(s.substr(pos, startPos - pos));
        pos = startPos + 1;
    }
    
    //    while(std::getline(ss, item, delim))
    //    {
    //        elems.push_back(item);
    //    }
    return elems;
}

vector<string> splitTest ( string input , string split_id ) {
    vector<string> result;
    int i = 0;
    bool add;
    string temp;
    stringstream ss;
    size_t found;
    string real;
    int r = 0;
    while ( i != input.length() ) {
        add = false;
        ss << input.at(i);
        temp = ss.str();
        found = temp.find(split_id);
        if ( found != string::npos ) {
            add = true;
            real.append ( temp , 0 , found );
        } else if ( r > 0 &&  ( i+1 ) == input.length() ) {
            add = true;
            real.append ( temp , 0 , found );
        }
        if ( add ) {
            result.push_back(real);
            ss.str(string());
            ss.clear();
            temp.clear();
            real.clear();
            r = 0;
        }
        i++;
        r++;
    }
    return result;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<std::string> split(const std::string &s, string delim)
{
    std::vector<std::string> elems= splitTest(s, delim);
    return elems;
}

std::vector<std::string> getElements(string value)
{
    size_t index = 0;
    while (true) {
        index = value.find("  ", 0);
        if (index == string::npos)
            break;
        value.replace(index, 2, " ");
    }
    vector<string> elements = split(value, ' ');
    return elements;
}

const string commentSign = "//";

vector<string> removeCommentsAndEmptyStrings(vector<string> lines)
{
    vector<string> result;
    
    for (int i = 0; i < lines.size(); i++)
    {
        string str = lines[i];
        if (str.length() == 0) {
            continue;
        }
        string::size_type startPos = str.find(commentSign.c_str(), 0);
        if (startPos != std::string::npos)
        {
            if (startPos == 0)
                continue;
            
            string result1 = str.substr(0, startPos);
            result.push_back(result1);
        }
        else
            result.push_back(str);
    }
    
    return result;
}


string intToString(int value)
{
    char buffer [10];
    int n = 0;
    n = sprintf (buffer, "%d", value);
    return string(buffer);
}

string longToString(long value)
{
    char buffer [64];
    int n = 0;
    n = sprintf (buffer, "%ld", value);
    return string(buffer);
}

string floatToString3(float value)
{
    char buffer [64];
    int n = 0;
    n = sprintf (buffer, "%.03f", value);
    return string(buffer);
}

string floatToString2(float value)
{
    char buffer [64];
    int n = 0;
    n = sprintf (buffer, "%.02f", value);
    return string(buffer);
}

string floatToString1(float value)
{
    char buffer [64];
    int n = 0;
    n = sprintf (buffer, "%.01f", value);
    return string(buffer);
}

string floatToString(float value)
{
  
    std::stringstream ss;
    ss<<value;
    std::string str;
    ss>>str;
    return str;
}

float stringToFloat(string value)
{
    return atof(value.c_str());
}

void removeBadCharacters(std::string &param)
{
	param.erase (std::remove(param.begin(), param.end(), '\r'), param.end());

	//remove(param.begin(), param.end(), '\r');
}

string toLower(string value)
{
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value;
}


string toUpper(string value)
{
    std::transform(value.begin(), value.end(), value.begin(), ::toupper);
    return value;
}

String repeat(char c, size_t num) {
    String s; s.Resize(num);
    memset((void*)s.CString(), c, num);
    return s;
}

String repeat(String s, size_t num) {
    size_t sz = s.Length();
    String r; r.Resize(num * sz);
    char* dst = (char*)r.CString();
    void* src = (void*)s.CString();
    for (size_t i = 0; i < num; i++){
        memcpy((void*)dst, src, sz);
        dst += sz;
    }
    return s;
}


void stringify(const JSONValue& v, String& result, bool pretty, String& pad, const String& padd){
    switch(v.GetValueType()) {
        case JSON_NULL: result += "null"; break;
        case JSON_BOOL: result += to<String>(v.GetBool()); break;
        case JSON_NUMBER: result += to<String>(v.GetDouble()); break;
        case JSON_STRING: result += "\"" + v.GetString() + "\""; break;
        case JSON_ARRAY: {
            if (pretty) {
                result += '[';
                auto sz = pad.Length();
                pad += padd;
                const JSONArray& a = v.GetArray();
                for (unsigned i = 0, e = a.Size(); i < e; i++) {
                    if (i) result += ",\n";
                    result += pad;
                    stringify(v[i], result, pretty, pad, padd);
                }
                result += ']'; 
                pad.Resize(sz);
            } else {
                result += '[';
                const JSONArray& a = v.GetArray();
                for (unsigned i = 0, e = a.Size(); i < e; i++) {
                    if (i) result += ",";
                    stringify(v[i], result, pretty, pad, padd);
                }
                result += ']'; 
            }
            break;
        }
        
        case JSON_OBJECT: {
            bool zpt = false;
            if (pretty) {
                result += "{";
                auto sz = pad.Length();
                pad += padd;
                
                for (ConstJSONObjectIterator i = v.Begin(); i != v.End(); ++i) {
                    result += zpt ? ",\n" : "\n";
                    result += pad + "\"" + i->first_ + "\": "; 
                    stringify(i->second_, result, pretty, pad, padd);
                    zpt = true;
                }
                result += zpt ? "\n}" : "}";
                pad.Resize(sz);
            } else {
                
                result += '{';
                for (ConstJSONObjectIterator i = v.Begin(); i != v.End(); ++i) {
                    if (zpt) result += ",";
                    result += "\"" + i->first_ + "\":"; 
                    stringify(i->second_, result, pretty, pad, padd);
                    zpt = true;
                }
                result += '}';
                
            }
            break;
        }
    }
}

String stringify(const JSONValue& v, bool pretty){
    String result; String pad;
    stringify(v, result, pretty, pad, "\t");
    return result;
}
    


/*
 base64.cpp and base64.h
 
 Copyright (C) 2004-2008 René Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 
 */

#include <iostream>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}

void pushBackIfNotExists(std::vector<std::string> &vector, std::string &value)
{
    if (std::find(vector.begin(), vector.end(), value) == vector.end()) {
        vector.push_back(value);
    }
}

void pushBackIfNotExists(std::vector<std::string> &vector, const char *value)
{
    if (std::find(vector.begin(), vector.end(), value) == vector.end()) {
        vector.push_back(value);
    }
}

string formattedTimeAgoString(unsigned long long timestamp)
{
    time_t timeNow;
    time(&timeNow);
    time_t timeParam = timestamp;
    
    double difference = timeNow - timeParam;
    difference = difference;
    if (difference < 60*60)
    {
        float time = difference/60;
        time += 0.5;
        int minute = time;
        string result = intToString(minute);
        if (minute == 1) {
            return result + " minute ago";
        }
        return result + " minutes ago";
    }
    if (difference < 60*60*18)
    {
        float time = difference/3600;
        time += 0.5;
        int hour = time;
        string result = intToString(time);
        if (hour == 1) {
            return result + " hour ago";
        }
        return result + " hours ago";
    }
    if (difference < 60*60*24*30) {
        float time = difference/(60*60*24);
        time += 0.5;
        
        int day = time;
        if (day == 0) {
            return "about a day ago";
        }
        string result = intToString(day);
        if (day == 1) {
            return result + " day ago";
        }
        return result + " days ago";
    }
    
    char mbstr[256];
    strftime(mbstr, sizeof(mbstr), "%b %d, %Y", localtime(&timeParam));
    
    return string(mbstr);
}



void StrUtils::ToStdWString(std::string const &src, std::wstring &dst) {
    dst = ToStdWString(src);
}

#ifdef __APPLE__
    using cvt_t = std::codecvt_utf8<wchar_t>;
#elif __GNUC__
    using cvt_t = std::codecvt_utf8<wchar_t>;
#elif _MSC_VER
    using cvt_t = std::codecvt_utf8_utf16<wchar_t>;
#else
#error "Your compiler is not supported"
#endif

    
std::wstring StrUtils::ToStdWString(std::string const &src) {
    return std::wstring_convert<cvt_t>().from_bytes(src);
}

void StrUtils::ToStdString(std::wstring const &src, std::string &dst) {
    dst = ToStdString(src);
}

std::string StrUtils::ToStdString(std::wstring const &src) {
    return std::wstring_convert<cvt_t>().to_bytes(src);
}

std::string StrUtils::ToLowerASCII(std::string const &str) {
    auto result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) {
            assert(c <= 127);  // assumed characters are ASCII
            return std::tolower(c);
        });
    return result;
}

bool StrUtils::endsWith(std::string const &str, std::string const &suffix)
{
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool StrUtils::startsWith(std::string const &str, std::string const &prefix)
{
    if (str.length() < prefix.length()) {
        return false;
    }
    return str.compare(0, prefix.size(), prefix) == 0;
}

wstring StrUtils::stringByReplacing(const std::wstring &source, const std::wstring &whatToReplace, const std::wstring &toReplaceWith)
{
    wstring result = source;
    
    size_t start_pos = 0;
    while((start_pos = source.find(whatToReplace, start_pos)) != std::string::npos) {
        result.replace(start_pos, whatToReplace.length(), toReplaceWith);
        start_pos += toReplaceWith.length();
    }
    return result;
    
//    std::replace(result.begin(), result.end(), whatToReplace, toReplaceWith);
//    return result;
}

void StrUtils::splitString(const ::std::string& str, char delimiter, ::std::vector< ::std::string>* dest) 
{
  ::std::vector< ::std::string> parsed;
  ::std::string::size_type pos = 0;
  while (true) {
    const ::std::string::size_type colon = str.find(delimiter, pos);
    if (colon == ::std::string::npos) {
      parsed.push_back(str.substr(pos));
      break;
    } else {
      parsed.push_back(str.substr(pos, colon - pos));
      pos = colon + 1;
    }
  }
  dest->swap(parsed);
}
