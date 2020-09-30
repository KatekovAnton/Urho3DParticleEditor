//
//  Sys.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/15/12.
//
//

#ifndef TerminalExtraction_Sys_h
#define TerminalExtraction_Sys_h

#ifdef TARGET_OS_IPHONE

#endif

#if defined DEBUG
#define UDLOG(s, ...) UDLog((s), __VA_ARGS__)
#else

#if defined PROJECTN_TARGET_OS_WIN
#define UDLOG(s, ...) UDLog((s), __VA_ARGS__)
#else
#define UDLOG(s, ...)
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

    void ULogSetEnabled(bool enabled);
    
    void ULogR(const char * pszFormat, ...);
    void ULog(const char * pszFormat, ...);
    void UDLog(const char * pszFormat, ...);

    int decodeFromBase64(const unsigned char *in, unsigned int inLength, unsigned char **out);

    
#ifdef __cplusplus
}
#endif

#endif




