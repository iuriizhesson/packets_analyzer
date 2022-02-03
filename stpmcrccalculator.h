#ifndef STPMCRCCALCULATOR_H
#define STPMCRCCALCULATOR_H

#include <cstdint>

class StpmCrcCalculator
{
  public:
    void    operator()(uint8_t byte);
    uint8_t getResult() const;

  private:
    uint8_t result = 0;
};

#endif // STPMCRCCALCULATOR_H
