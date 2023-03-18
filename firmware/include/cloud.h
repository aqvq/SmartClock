#ifndef SHANG_CLOUD_H
#define SHANG_CLOUD_H

#include "conf.h"

void dataRead(const String &data);
void heartbeat();
void blinker_init();
void blinker_routine();

#endif