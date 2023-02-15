// #pragma GCC diagnostic ignored "-Wwrite-strings"

// #define BLINKER_WIFI
// #define BLINKER_ESP_SMARTCONFIG

#include "conf.h"
// #include <Blinker.h>
#include "display.h"
#include "dht.h"
#include "network.h"
// #include "input.h"

extern float humidity;
extern float temperature;
// BlinkerNumber blinker_humidity("humidity");
// BlinkerNumber blinker_temperature("temperature");
void dataRead(const String &data);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifi_init();
  dht_init();
  buzzer_init();
  // 注释了关于Blinker相关的部分
  // 开机的时候比较卡，自己写的WiFi模块已经够用了
  // BLINKER_DEBUG.stream(Serial);
  // Blinker.begin(blinker_auth);
  // Blinker.attachData(dataRead);
  configTime(8 * 3600, 0, ntp_server);
  display_init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Blinker.run();
  display_routine();
  // blinker_humidity.print(humidity);
  // blinker_temperature.print(temperature);
}

// void dataRead(const String &data)
// {
//   BLINKER_LOG("Blinker readString: ", data);
//   Blinker.vibrate();
//   uint32_t BlinkerTime = millis();
//   Blinker.print("millis", BlinkerTime);
// }
