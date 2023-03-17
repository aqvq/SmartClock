#ifndef SHANG_CLOUD_H
#define SHANG_CLOUD_H

#include "conf.h"
#include "sensors.h"
void dataRead(const String &data);
void heartbeat();
void blinker_init();
void blinker_routine();

#endif