// #include "network.h"

// void wifi_init()
// {
//   Serial.println("\nConnecting WiFi...");
//   // WiFi.begin(wifi_ssid, wifi_password);
//   if (!wifi_auto_config())
//   {
//     wifi_smart_config();
//   }
//   esp_wifi_restore(); // 删除保存的wifi信息
//   Serial.println("\nWiFi connected!");
// }

// void wifi_close()
// {
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     WiFi.disconnect(true); // 断开wifi网络
//     WiFi.mode(WIFI_OFF);   // 关闭网络
//   }
// }

// void wifi_smart_config()
// {
//   WiFi.mode(WIFI_STA);
//   Serial.println("\r\nWait for Smartconfig...");
//   WiFi.beginSmartConfig(SC_TYPE_ESPTOUCH_V2);
//   while (1)
//   {
//     Serial.print(".");
//     delay(500); // wait for a second
//     if (WiFi.smartConfigDone())
//     {
//       Serial.println("SmartConfig Success");
//       Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
//       Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
//       break;
//     }
//   }
// }

// bool wifi_auto_config()
// {
//   WiFi.begin();
//   // 如果觉得时间太长可改
//   for (int i = 0; i < 3; i++)
//   {
//     int wstatus = WiFi.status();
//     if (wstatus == WL_CONNECTED)
//     {
//       Serial.println("WIFI SmartConfig Success");
//       Serial.printf("SSID:%s", WiFi.SSID().c_str());
//       Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
//       Serial.print("LocalIP:");
//       Serial.print(WiFi.localIP());
//       Serial.print(" ,GateIP:");
//       Serial.println(WiFi.gatewayIP());
//       return true;
//     }
//     else
//     {
//       Serial.print("WIFI AutoConfig Waiting......");
//       Serial.println(wstatus);
//       delay(500);
//     }
//   }
//   Serial.println("WIFI AutoConfig Faild!");
//   return false;
// }
