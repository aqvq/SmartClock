
#define BLINKER_WIFI
#define BLINKER_WITHOUT_SSL
#include <Blinker.h>
#include "cloud.h"

BlinkerNumber blinker_data_temperature("temperature");
BlinkerNumber blinker_data_humidity("humidity");
uint32_t read_time = 0;
extern float humidity;
extern float temperature;

void dataRead(const String &data)
{
    BLINKER_LOG("Blinker readString: ", data);
    Blinker.vibrate();
    uint32_t BlinkerTime = millis();
    Blinker.print("millis", BlinkerTime);
}

void heartbeat()
{
    blinker_data_temperature.print(temperature);
    blinker_data_humidity.print(humidity);
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
}