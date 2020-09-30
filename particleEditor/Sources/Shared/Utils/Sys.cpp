//
//  Sys.c
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/15/12.
//
//

#include <stdio.h>
#include "stdarg.h"
#ifdef TARGET_OS_IPHONE
#include <strings.h>
#endif
#include "Sys.h"
//#include <Sys/Display.h>

#ifdef ANDROID

#include <android/log.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "native-activity", __VA_ARGS__))
#endif

static bool _ULogSetEnabled = true;
void ULogSetEnabled(bool enabled)
{
    _ULogSetEnabled = enabled;
}


void ULogR(const char * pszFormat, ...)
{
    char szBuf[16*1024];
    
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, 16*1024, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
}

void ULog(const char * pszFormat, ...)
{
    if (!_ULogSetEnabled) {
        return;
    }
    
    
    char szBuf[16*1024];
    
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, 16*1024, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
    
    
#ifdef ANDROID
    LOGD("%s", szBuf);
#endif
}

void UDLog(const char * pszFormat, ...)
{
	char szBuf[16 * 1024];

	va_list ap;
	int result = 0;
	va_start(ap, pszFormat);
	result += vsnprintf(szBuf, 16 * 1024 - 1, pszFormat, ap);
	va_end(ap);
	szBuf[result] = '\n';
	szBuf[result + 1] = 0;

//	Display::WriteLog(szBuf);
}






#include <stdio.h>
#include <stdlib.h>



    unsigned char _alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    int __base64Decode(const unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len );
    
    int __base64Decode(const unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len )
    {
        static char inalphabet[256], decoder[256];
        int i, bits, c = 0, char_count, errors = 0;
        unsigned int input_idx = 0;
        unsigned int output_idx = 0;
        
        for (i = (sizeof _alphabet) - 1; i >= 0 ; i--) {
            inalphabet[_alphabet[i]] = 1;
            decoder[_alphabet[i]] = i;
        }
        
        char_count = 0;
        bits = 0;
        for( input_idx=0; input_idx < input_len ; input_idx++ ) {
            c = input[ input_idx ];
            if (c == '=')
                break;
            if (c > 255 || ! inalphabet[c])
                continue;
            bits += decoder[c];
            char_count++;
            if (char_count == 4) {
                output[ output_idx++ ] = (bits >> 16);
                output[ output_idx++ ] = ((bits >> 8) & 0xff);
                output[ output_idx++ ] = ( bits & 0xff);
                bits = 0;
                char_count = 0;
            } else {
                bits <<= 6;
            }
        }
        
        if( c == '=' ) {
            switch (char_count) {
                case 1:
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
//                    std::fprintf(stderr, "base64Decode: encoding incomplete: at least 2 bits missing");
//#endif
                    errors++;
                    break;
                case 2:
                    output[ output_idx++ ] = ( bits >> 10 );
                    break;
                case 3:
                    output[ output_idx++ ] = ( bits >> 16 );
                    output[ output_idx++ ] = (( bits >> 8 ) & 0xff);
                    break;
            }
        }
        else if ( input_idx < input_len )
        {
            if (char_count) {
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
//                std::fprintf(stderr, "base64 encoding incomplete: at least %d bits truncated",
//                             ((4 - char_count) * 6));
//#endif
                errors++;
            }
        }
        
        *output_len = output_idx;
        return errors;
    }
    
    int decodeFromBase64(const unsigned char *in, unsigned int inLength, unsigned char **out)
    {
        unsigned int outLength = 0;
        
        //should be enough to store 6-bit buffers in 8-bit buffers
        *out = new unsigned char[(size_t)(inLength * 3.0f / 4.0f + 1)];
        if( *out ) {
            int ret = __base64Decode(in, inLength, *out, &outLength);
            
            if (ret > 0 )
            {
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
//                printf("Base64Utils: error decoding");
//#endif
                delete [] *out;
                *out = NULL;
                outLength = 0;
            }
        }
        return outLength;
    

}
