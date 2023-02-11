#include <Drivers/Network.h>

void initWiFi(void)
{
    Serial.print("\nConnecting WiFi...");
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void closeWiFi(void)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect(true); // 断开wifi网络
        WiFi.mode(WIFI_OFF);   // 关闭网络
    }
}
