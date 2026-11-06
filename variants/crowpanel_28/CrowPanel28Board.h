// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <Arduino.h>
#include <helpers/ESP32Board.h>

class CrowPanel28Board : public ESP32Board {
public:
  void begin();
  uint16_t getBattMilliVolts() override;
  const char* getManufacturerName() const override { return "Elecrow CrowPanel Advance 2.8"; }
  int externalPowerPresent();
private:
  bool     _ina_ok = false;
  uint16_t _batt_mv = 0;     // cached reading, 0 = none yet
  uint32_t _batt_at = 0;     // millis() of that reading
  int8_t _ext = -1; uint32_t _ext_at = 0; //external power cached read and millis of the read
};
