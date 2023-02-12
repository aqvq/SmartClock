#ifndef SHANG_BLE_H
#define SHANG_BLE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <WiFi.h>

#include "conf.h"

void ble_init(void);
void communicateBLE(std::string rxData);
void BLE2WiFi(std::string WiFi_Data);

#endif