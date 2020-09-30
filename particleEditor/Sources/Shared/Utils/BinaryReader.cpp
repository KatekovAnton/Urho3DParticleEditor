#include "BinaryReader.h"
#include <memory>
#include <cstring>

#include <string>
#include <iostream>
#include "StringUtils.h"

#ifdef ANDROID
#include "android/asset_manager.h"
#include <android/log.h>

AAssetManager* BinaryReader::assetManager = 0;

void fread(void* buffer, size_t size, size_t num, AAsset* asset) {
	AAsset_read(asset, buffer, size * num);
}

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "native-activity", __VA_ARGS__))

#endif

using namespace std;



BinaryReader::BinaryReader(char *buffer, long length):
_position(0),
_filesize(length),
_file(NULL),
_shouldDeallocBuffer(false)
{
    _internalBuffer = buffer;
    SetPosition(0);
}


BinaryReader::BinaryReader(char *buffer, long length, bool shouldDeallocBuffer)
:_position(0),
_filesize(length),
_file(NULL),
_shouldDeallocBuffer(shouldDeallocBuffer)
{
    _internalBuffer = buffer;
    SetPosition(0);
}

long BinaryReader::GetPosition()
{
    if (_file)
    {
#ifdef ANDROID
    	off_t asset_l = AAsset_getLength(_file);
		off_t asset_r = AAsset_getRemainingLength(_file);
	    _position = asset_l - asset_r;
#else
        _position = ftell(_file);
#endif
    }
    return _position;
}

void BinaryReader::SetPosition(long position)
{
    if (position <= _filesize)
    {
        _position = position;
    }
    if (_file)
    {
#ifdef ANDROID
    	AAsset_seek(_file, _position, SEEK_SET);
#else
        fseek(_file, _position, SEEK_SET);
#endif
    }
    else
    {
        _currentBuffer = _internalBuffer;
        _currentBuffer += _position;
     //   _currentBuffer = *&_internalBuffer + _position; //reinterpret_cast<char *>(_internalBuffer)> + _position;
    }
}


int32_t BinaryReader::ReadInt()
{
    if (_file)
    {
        int retVal;
        fread(&retVal, sizeof(int), 1, _file);
        return retVal;
    }
    int retVal = *(int*)_currentBuffer;
    _currentBuffer += sizeof(int);
    _position += sizeof(int);
    return retVal;
}

uint32_t BinaryReader::ReadUInt()
{
    if (_file)
    {
        uint32_t retVal;
        fread(&retVal, sizeof(uint32_t), 1, _file);
        return retVal;
    }
    uint32_t retVal = *(uint32_t*)_currentBuffer;
    _currentBuffer += sizeof(uint32_t);
    _position += sizeof(uint32_t);
    return retVal;
}

uint16_t BinaryReader::ReadUInt16()
{
    if (_file)
    {
        uint16_t retVal;
        fread(&retVal, sizeof(uint16_t), 1, _file);
        return retVal;
    }
    uint16_t retVal = *(uint16_t*)_currentBuffer;
    _currentBuffer += sizeof(uint16_t);
    _position += sizeof(uint16_t);
    return retVal;
}

int16_t BinaryReader::ReadInt16()
{
    if (_file)
    {
        int16_t retVal;
        fread(&retVal, sizeof(int16_t), 1, _file);
        return retVal;
    }
    int16_t retVal = *(int16_t*)_currentBuffer;
    _currentBuffer += sizeof(int16_t);
    _position += sizeof(int16_t);
    return retVal;
}

float BinaryReader::ReadSingle()
{
    if (_file)
    {
        float retVal;
        fread(&retVal, sizeof(float), 1, _file);
        return retVal;
    }
    float retVal;// = *reinterpret_cast<float*>(_currentBuffer);
    memcpy(&retVal, _currentBuffer, 4);
    _currentBuffer += 4;
    _position += 4;
    return retVal;
}

unsigned char BinaryReader::ReadUChar()
{
    if (_file)
    {
        unsigned char retVal;
        fread(&retVal, sizeof(unsigned char), 1, _file);
        return retVal;
    }
    unsigned char retVal = *_currentBuffer;
    _currentBuffer += 1;
    _position += 1;
    return retVal;
}

char BinaryReader::ReadChar()
{
    if (_file)
    {
        char retVal;
        fread(&retVal, sizeof(char), 1, _file);
        return retVal;
    }
    char retVal = *_currentBuffer;
    _currentBuffer += 1;
    _position += 1;
    return retVal;
}

void BinaryReader::ReadBuffer(long length, char *buf)
{
    if (_file)
    {
        fread(buf, sizeof(char), length, _file);
        return;
    }
    memcpy(buf, _currentBuffer, length);
    _currentBuffer += length;
    _position += length;
    return;
}

string BinaryReader::ReadBadString()
{
    int length = ReadInt();
    char *data = (char*) malloc (sizeof(char)*length+1);
    ReadBuffer(length+1, data);
    string str = string(data);
    free(data);
    return str;
}

string BinaryReader::ReadString()
{
    int length = ReadInt();
    char *data = (char*) malloc (sizeof(char)*length+1);
    ReadBuffer(length+1, data);
    if (data[length] != '\0')
    {
        ReadChar();
        char *dataold = data;
        data = (char*) malloc (sizeof(char)*length+2);
        memcpy(data, dataold, length+1);
        data[length+1]='\0';
        free(dataold);
    }
    string str = string(data);
    free(data);
    return str;
}

BinaryReader::~BinaryReader()
{
    if (_file)
    {
#ifdef ANDROID
    	AAsset_close(_file);
#else
        fclose(_file);
#endif
        return;
    }
    if (_currentBuffer && _shouldDeallocBuffer)
        free(_currentBuffer);
}

std::string BinaryReader::ReadFullAsString()
{
    SetPosition(0);
    long length = GetLength();
    char* data = (char*)malloc(length+2);
    memset(data, 0, length+2);
    ReadBuffer(length, data);
    data[length] = '\0';
    data[length+1] = '\0';
    std::string result = string(data);
    free(data);
	//removeBadCharacters(result);
    return result;
}

#pragma mark - IBinaryReader

void BinaryReader::ReaderSetPosition(long position)
{
    SetPosition(position);
}

long BinaryReader::ReaderGetPosition()
{
    return GetPosition();
}

long BinaryReader::ReaderGetSize()
{
    return GetLength();
}

int BinaryReader::ReaderReadInt()
{
    return ReadInt();
}

unsigned int BinaryReader::ReaderReadUInt()
{
    return ReadUInt();
}

void BinaryReader::ReaderReadBuffer(long length, char *buf)
{
    ReadBuffer(length, buf);
}

float BinaryReader::ReaderReadFloat()
{
    return ReadSingle();
}
