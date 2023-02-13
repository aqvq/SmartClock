#pragma GCC diagnostic ignored "-Wwrite-strings"

#define BLINKER_WIFI
#define BLINKER_ESP_SMARTCONFIG

#include "conf.h"
#include <Blinker.h>
#include "display.h"
#include "dht.h"
// #include "input.h"

extern float humidity;
extern float temperature;
BlinkerNumber blinker_humidity("humidity");
BlinkerNumber blinker_temperature("temperature");
void dataRead(const String &data);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht_init();
  BLINKER_DEBUG.stream(Serial);
  Blinker.begin(blinker_auth);
  Blinker.attachData(dataRead);
  configTime(8 * 3600, 0, ntp_server);
  display_init();
  // input_init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blinker.run();
  display_routine();
  blinker_humidity.print(humidity);
  blinker_temperature.print(temperature);
}

void dataRead(const String &data)
{
  BLINKER_LOG("Blinker readString: ", data);
  Blinker.vibrate();
  uint32_t BlinkerTime = millis();
  Blinker.print("millis", BlinkerTime);
}
