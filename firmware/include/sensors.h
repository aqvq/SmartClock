#ifndef SHANG_DHT_H
#define SHANG_DHT_H

#include <DHT.h>

void dht_init();
void dht_update(float *humidity, float *temperature);
void buzzer_init();
void buzzer_start();
void buzzer_stop();
void buzzer_ring(uint16_t time);

#endif