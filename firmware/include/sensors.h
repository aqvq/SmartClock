#ifndef SHANG_DHT_H
#define SHANG_DHT_H

#include <DHT.h>

void dht_init();
void dht_update(float *humidity, float *temperature);

#endif