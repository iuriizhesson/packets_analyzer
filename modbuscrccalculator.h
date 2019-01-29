#ifndef MODBUSCRCCALCULATOR_H
#define MODBUSCRCCALCULATOR_H

#include <cstdint>

class ModbusCrcCalculator
{
  public:
    void operator()(const uint8_t byte);
    uint16_t getResult() const;

  private:
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
};

#endif // MODBUSCRCCALCULATOR_H
