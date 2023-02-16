// WiFi配网模块

#ifndef SHANG_NETWORK_H
#define SHANG_NETWORK_H

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>  //用于设备域名 MDNS.begin("esp32")
#include <esp_wifi.h> //用于esp_wifi_restore() 删除保存的wifi信息
#include "conf.h"

extern const int LED; // 设置LED引脚

//===========需要调用的函数===========
void network_init();               // 初始化网络
void network_routine();            // 实时检查网络状况
void checkConnect(bool reConnect); // 检测wifi是否已经连接
void eraseWiFi();                  // 删除保存的wifi信息
void LEDinit();                    // LED初始化
void checkDNS_HTTP();              // 检测客户端DNS&HTTP请求
void connectToWiFi(int timeOut_s); // 连接WiFi
void closeWiFi();                  // 关闭WiFi

//===========内部函数===========
void handleRoot();                    // 处理网站根目录的访问请求
void handleConfigWifi();              // 提交数据后的提示页面
void handleNotFound();                // 处理404情况的函数'handleNotFound'
void initSoftAP();                    // 进入AP模式
void initDNS();                       // 开启DNS服务器
void initWebServer();                 // 初始化WebServer
bool scanWiFi();                      // 扫描附近的WiFi，为了显示在配网界面
void wifiConfig();                    // 配置配网功能
void reset_device();                  // 重置设备
void blinkLED(int led, int n, int t); // LED闪烁函数        //用不上LED可删除

#endif