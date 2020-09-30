//
//  BitmapComposer.cpp
//  maxtouchTool
//
//  Created by Katekov Anton on 8/11/16.
//  Copyright © 2016 katekovanton. All rights reserved.
//

#include "BitmapComposer.hpp"
#include "BitmapTexture.h"
#include "ByteBuffer.h"



BitmapComposer::BitmapComposer(GSize2D size)
{
    _resultBitmap = new BitmapTexture();
    _resultBitmap->_info._chanels = 4;
    _resultBitmap->_info._textureSize = size;
    
    size_t bufferSize = (size_t)size.width * (size_t)size.height * _resultBitmap->_info._chanels;
    _resultBitmap->MakeBufferWithSize(bufferSize);
}

BitmapComposer::~BitmapComposer()
{
    delete _resultBitmap;
}

void BitmapComposer::setColor(Color color, int x, int y)
{
    Color *buffer = getColorBuffer();
    buffer[y * (int)_resultBitmap->_info._textureSize.width + x] = color;
}

GSize2D BitmapComposer::getSize()
{
    return _resultBitmap->_info._textureSize;
}

Color *BitmapComposer::getColorBuffer()
{
    return reinterpret_cast<Color*>(_resultBitmap->GetBuffer()->getPointer());;
}
