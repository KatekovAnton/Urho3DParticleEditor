#include "IdGenerator.h"



unsigned int IdGenerator::GenerateId() {
    return _idPointer++;
}

void IdGenerator::Reset() {
    _idPointer = 0;
}

void IdGenerator::SetId(unsigned int point) {
    _idPointer = point;
}

unsigned int IdGenerator::GetCurrentId() {
    return _idPointer;
}
