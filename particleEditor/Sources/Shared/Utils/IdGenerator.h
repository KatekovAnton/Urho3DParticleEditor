//
//  IdGenerator.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/1/12.
//
//

#ifndef __TerminalExtraction__IdGenerator__
#define __TerminalExtraction__IdGenerator__

#include <iostream>

class IdGenerator {
    
    unsigned int        _idPointer = 0;
    
public:
    
    unsigned int GenerateId();
    
    void Reset();
    void SetId(unsigned int);
    unsigned int GetCurrentId();
    
};

#endif /* defined(__TerminalExtraction__IdGenerator__) */
