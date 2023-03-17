
#define BLINKER_WIFI
#include <Blinker.h>
#include "cloud.h"

extern DHT dht;
BlinkerNumber blinker_data_temperature("temperature");
BlinkerNumber blinker_data_humidity("humidity");
uint32_t read_time = 0;

void dataRead(const String &data)
{
    BLINKER_LOG("Blinker readString: ", data);
    Blinker.vibrate();
    uint32_t BlinkerTime = millis();
    Blinker.print("millis", BlinkerTime);
}

void heartbeat()
{
    blinker_data_temperature.print(dht.readTemperature());
    blinker_data_humidity.print(dht.readHumidity());
}

void blinker_init()
{
    BLINKER_DEBUG.stream(Serial);
    Blinker.begin(blinker_auth, "", "");
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
}

void blinker_routine()
{
    Blinker.run();
    if (read_time == 0 || (millis() - read_time) >= 2000)
    {
        read_time = millis();
        BLINKER_LOG("Temperature: ", dht.readTemperature(), " *C");
        BLINKER_LOG("Humidity: ", dht.readHumidity(), " %");
    }
}