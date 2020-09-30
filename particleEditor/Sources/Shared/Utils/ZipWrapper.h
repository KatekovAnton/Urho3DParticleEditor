//
//  ZipWrapper.h
//  MAX
//
//  Created by Anton Katekov on 25.04.14.
//  Copyright (c) 2014 AntonKatekov. All rights reserved.
//

#ifndef ZipWrapper__
#define ZipWrapper__

#include <iostream>

class ByteBuffer;

void zip_compress(char *bytes, size_t length, ByteBuffer *destination);
void zip_decompress(char *bytes, size_t length, ByteBuffer *destination);

#endif /* defined(ZipWrapper__) */
