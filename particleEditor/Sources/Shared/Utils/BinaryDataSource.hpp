//
//  BinaryDataSource.h
//  MAX
//
//  Created by Katekov Anton on 2/9/17.
//  Copyright © 2017 AntonKatekov. All rights reserved.
//

#ifndef BinaryDataSource_h
#define BinaryDataSource_h

#include <string>



class ByteBuffer;



class BinaryDataReceiver {
    
public:
    virtual void BinaryDataReceiver_AddData(const std::string &name, ByteBuffer *buffer) = 0;
    
};



class BinaryDataSource {
    
public:
    virtual void BinaryDataSource_GetData(const std::string &name, ByteBuffer *buffer) = 0;
    
};



class BinaryDataContainer : public BinaryDataReceiver, public BinaryDataSource {
    
public:
    virtual void BinaryDataReceiver_AddData(const std::string &name, ByteBuffer *buffer) = 0;
    virtual void BinaryDataSource_GetData(const std::string &name, ByteBuffer *buffer) = 0;
    
};



class BinaryDataProvider {
    
public:
    virtual void BinaryDataProvider_FillReceiver(BinaryDataReceiver *receiver) = 0;
    
};

#endif /* BinaryDataSource_h */
