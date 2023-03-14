#include "sensors.h"

extern unsigned long initialized_time;
DHT dht(DHTPIN, DHTTYPE);

// ================================
// DHT11温湿度传感器模块部分
// ================================
void dht_init()
{
    Serial.println(F("DHTxx test!"));
    dht.begin();
}

void dht_update(float *humidity, float *temperature)
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    *humidity = h;
    *temperature = t;
}
