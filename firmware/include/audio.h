#ifndef AUDIO_SHANG
#define AUDIO_SHANG

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "clock.h"

void audio_init();
void audio_routine();
void printDetail(uint8_t type, int value);

#endif