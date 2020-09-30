//
//  BinaryPack.cpp
//  MAX
//
//  Created by Katekov Anton on 3/18/16.
//  Copyright © 2016 AntonKatekov. All rights reserved.
//

#include "BinaryPack.hpp"
#include <Shared/Utils/Math/Geometry.h>
#include "ByteBuffer.h"
#include <Shared/Utils/BinaryInterfaces.hpp>


#include <memory.h>



static bool _logPacks = true;

using namespace std;

__BinaryPackItemHeader::__BinaryPackItemHeader()
:_flags(0)
,_size(0)
,_offset(0)
{
	memset(_name, 0, sizeof(_name));
}



BinaryPack::BinaryPack()
:_numberOfItems(0)
,_items(10)
,_filename("")
{
    
}

BinaryPack::BinaryPack(const std::string &filename)
:_numberOfItems(0)
,_items(10)
,_filename(filename)
{

}

BinaryPack::~BinaryPack()
{
    ClearItems();
}

void BinaryPack::ClearItems()
{
    _items.clear();
    _numberOfItems = 0;
    _itemsHash.clear();
}

void BinaryPack::RefreshHash()
{
    _itemsHash.clear();
    for (int i = 0; i < _numberOfItems; i++)
    {
        BinaryPackItemHeader *item = _items.GetArrayPointer() + i;
        _itemsHash.insert(pair<string, BinaryPackItemHeader*>(item->_name, item));
    }
}

void BinaryPack::AddItem(const BinaryPackItemHeader &item)
{
#if defined DEBUG
    if (Item(item._name) != NULL) {
        throw "Item with same name already esists!";
    }
#endif
    
    _items.addObject(item);
    _numberOfItems++;
    
#if defined DEBUG
//    string s = string("==== AddItem ==== ") + string(item._name) + string(" : ") + to_string(_numberOfItems);
//    ULog(s.c_str());
#endif
    
    BinaryPackItemHeader *internalItem = _items.GetArrayPointer() + _numberOfItems;
    _itemsHash.insert(pair<string, BinaryPackItemHeader*>(item._name, internalItem));
}

BinaryPackItemHeader *BinaryPack::Item(const std::string &name)
{
    if (_itemsHash.count(name) == 0) {
        return NULL;
    }
    return _itemsHash[name];
}

BinaryPackItemHeader *BinaryPack::ItemAtIndex(unsigned int index)
{
    return _items.GetArrayPointer() + index;
}



BinaryPackReader::BinaryPackReader(BinaryPack *binaryPack, std::shared_ptr<IBinaryReader> reader)
:_binaryPack(binaryPack)
,_reader(reader)
{
    Reload();
}

void BinaryPackReader::Reload()
{
    _reader->ReaderSetPosition(0);
    _binaryPack->ClearItems();
    
    if (!_reader) {
        return;
    }
    
    unsigned int position = 0;
    while (position + BINARY_PACK_HEADER_SIZE <= _reader->ReaderGetSize())
    {
        BinaryPackItemHeader header;
        _reader->ReaderReadBuffer(BINARY_PACK_HEADER_NAME_SIZE, header._name);
        header._flags = _reader->ReaderReadUInt();
        header._size = _reader->ReaderReadUInt();
        
        if (header._size == 0) {
            break;
        }
        
        header._offset = _reader->ReaderGetPosition();
        
#if defined DEBUG
//        if (_logPacks) {
//            ULog("BinaryPackReader: Found item %s, size %lu", header._name, (unsigned long)header._size);
//        }
#endif
        
        position = position + BINARY_PACK_HEADER_SIZE + header._size;
        _reader->ReaderSetPosition(position);
        
        _binaryPack->AddItem(header);
    }
    
    _binaryPack->RefreshHash();
}

BinaryPackItemHeader *BinaryPackReader::Item(const std::string &name)
{
    return _binaryPack->Item(name);
}

void BinaryPackReader::ReadItemContent(BinaryPackItemHeader *item, ByteBuffer *buffer)
{
    _reader->ReaderSetPosition(item->_offset);
    if (buffer->getFullSize() < item->_size) {
        buffer->increaseBufferBy(item->_size - buffer->getFullSize());
    }
    _reader->ReaderReadBuffer(item->_size, reinterpret_cast<char*>(buffer->getPointer()));
    buffer->dataAppended(item->_size);
}



BinaryPackWriter::BinaryPackWriter(BinaryPack *binaryPack, std::shared_ptr<IBinaryWriter> writer)
:_binaryPack(binaryPack)
,_writer(writer)
{}

void BinaryPackWriter::RemoveLastItems(unsigned int countToRemove)
{
    unsigned int count = ____min(countToRemove, _binaryPack->_numberOfItems);
    if (count == 0) {
        return;
    }
    
    unsigned int offset = 0;
    if (count != _binaryPack->_numberOfItems) {
        BinaryPackItemHeader lastHeaderToStay = _binaryPack->_items.objectAtIndex(_binaryPack->_numberOfItems - count - 1);
        offset = lastHeaderToStay._offset + lastHeaderToStay._size;
    }
    unsigned int size = _writer->WriterGetSize() - offset;
    _writer->WriterSetPosition(offset);
    _writer->WriterWriteTile(0, size);
    _writer->WriterFlush();
    
    unsigned oldNumberOfItems = _binaryPack->_numberOfItems;
    BinaryPackItemHeader emptyHeader;
    for (int i = oldNumberOfItems - count; i < oldNumberOfItems; i++) {
        _binaryPack->_itemsHash.erase(_binaryPack->_items.objectAtIndex(i)._name);
        _binaryPack->_items.replaceObjectAtIndex(i, emptyHeader);
    }
    _binaryPack->_numberOfItems = oldNumberOfItems - count;
    _binaryPack->_items.unsafeObjectsAdded(-1 * (int)count);

    
    _writer->WriterFlush();
    _writer->WriterSetPosition(offset);
}

void BinaryPackWriter::Write(const std::string &name, unsigned int flags, ByteBuffer &body)
{
    Write(name, flags, reinterpret_cast<const char *>(body.getPointer()), static_cast<unsigned int>(body.getDataSize()));
}

void BinaryPackWriter::Write(const std::string &name, unsigned int flags, const char *body, unsigned int length)
{
    unsigned int lastBlockEnding = 0;
    
    if (_binaryPack->_items.GetCount() != 0) {
        BinaryPackItemHeader header = _binaryPack->_items.objectAtIndex(_binaryPack->_items.GetCount() - 1);
        lastBlockEnding = header._offset + header._size;
    }
    
    _writer->WriterSetPosition(lastBlockEnding);
    
    BinaryPackItemHeader header = BinaryPackWriter::Write(_writer.get(), name, flags, body, length);
    
    _binaryPack->AddItem(header);
}

BinaryPackItemHeader BinaryPackWriter::Write(IBinaryWriter *writer, const std::string &name, unsigned int flags, const char *body, unsigned int length)
{
    BinaryPackItemHeader itemHeader;
    strcpy(itemHeader._name, name.c_str());
    itemHeader._flags = flags;
    itemHeader._size = length;
    itemHeader._offset = writer->WriterGetPosition() + BINARY_PACK_HEADER_SIZE;
    
    writer->WriterWriteBuffer(reinterpret_cast<const char *>(&itemHeader._name), sizeof(itemHeader._name));
    writer->WriterWriteUInt(itemHeader._flags);
    writer->WriterWriteUInt(itemHeader._size);
    writer->WriterWriteBuffer(reinterpret_cast<const char *>(body), length);
    writer->WriterFlush();
    
    return itemHeader;
}

