#include "CrowPanel28Board.h"
#include <Wire.h>
#include <Adafruit_INA219.h>

static Adafruit_INA219 s_ina(0x40);   //INA219 battery readings (optional)

void CrowPanel28Board::begin() {
  digitalWrite(PIN_SD_CS, HIGH);
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(45, LOW);          // expansion slot to LoRa, not the microphone
  pinMode(45, OUTPUT);
  ESP32Board::begin();            // opens Wire on GPIO 15/16
  _ina_ok = s_ina.begin(&Wire);
  Serial.printf("[crowpanel-28] INA219 %s\n", _ina_ok ? "found at 0x40" : "not found");
}

uint16_t CrowPanel28Board::getBattMilliVolts() {
  if (!_ina_ok) return 0;         // UI shows "no battery", telemetry 0 V
  const uint32_t now = millis();
  if (_batt_mv && now - _batt_at < 2000) return _batt_mv;
  _batt_mv = (uint16_t)(s_ina.getBusVoltage_V() * 1000.0f + s_ina.getShuntVoltage_mV());
  _batt_at = now;
  return _batt_mv;
}

int CrowPanel28Board::externalPowerPresent() {   // 1 = on USB, 0 = on battery, -1 = unknown
  if (!_ina_ok) return -1;
  const uint32_t now = millis();
  if (_ext_at && now - _ext_at < 2000) return _ext;
  // if under 5mA it should be enough but to be tested
  _ext = s_ina.getCurrent_mA() < 5.0f ? 1 : 0;
  _ext_at = now;
  return _ext;
}