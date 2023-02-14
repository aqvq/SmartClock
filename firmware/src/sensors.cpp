#include "sensors.h"

#define DHTPIN 17
#define DHTTYPE DHT11
#define BUZPIN 26
DHT dht(DHTPIN, DHTTYPE);

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

void buzzer_init()
{
    pinMode(BUZPIN, OUTPUT);
    digitalWrite(BUZPIN, HIGH);
}

void buzzer_start()
{
    digitalWrite(BUZPIN, LOW);
}

void buzzer_stop()
{
    digitalWrite(BUZPIN, HIGH);
}

void buzzer_ring(uint16_t time)
{
    buzzer_start();
    delay(time);
    buzzer_stop();
}
