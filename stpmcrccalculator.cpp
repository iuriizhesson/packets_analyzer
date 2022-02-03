#include "stpmcrccalculator.h"

void StpmCrcCalculator::operator()(uint8_t byte)
{
    for (unsigned index = 0; index < 8; ++index)
    {
        uint8_t tmp = byte ^ result;
        result <<= 1;
        if (tmp & 0x80)
        {
            result ^= 0x07;
        }
        byte <<= 1;
    }
}

uint8_t StpmCrcCalculator::getResult() const
{
    return result;
}
