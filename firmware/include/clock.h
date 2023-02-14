#ifndef SHANG_CLOCK_H
#define SHANG_CLOCK_H
#include <SPIFFS.h>
#include <bitset>
#include "conf.h"

void clock_init();
void clock_read_all();
void clock_save_all();
void clock_set(int i, int j);
void clock_reset(int i, int j);
bool clock_get(int i, int j);

#endif