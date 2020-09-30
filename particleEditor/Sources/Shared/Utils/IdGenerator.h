#ifndef IdGenerator_h
#define IdGenerator_h

#include <iostream>

class IdGenerator {
    
    unsigned int        _idPointer = 0;
    
public:
    
    unsigned int GenerateId();
    
    void Reset();
    void SetId(unsigned int);
    unsigned int GetCurrentId();
    
};

#endif
