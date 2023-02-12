// #include "ble.h"

// static BLEServer *pServer;   // 创建服务器
// static BLEService *pService; // 创建服务
// static BLECharacteristic *pCharacteristic;
// static bool deviceConnected = false;
// static bool oldDeviceConnected = false;
// // Server回调函数声明
// class MyServerCallbacks : public BLEServerCallbacks
// {
//     void onConnect(BLEServer *pServer)
//     {
//         Serial.println("connected~");
//         // pCharacteristic->setValue("SHANG");
//         // pCharacteristic->notify();
//     };

//     void onDisconnect(BLEServer *pServer)
//     {
//         Serial.println("disconnected~");
//         // 在有设备接入后Advertising广播会被停止，所以要在设备断开连接时重新开启广播
//         // 不然的话只有重启ESP32后才能重新搜索到
//         pServer->startAdvertising(); // 该行效果同 BLEDevice::startAdvertising();
//     }
// };

// class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
// {
//     void onRead(BLECharacteristic *pCharacteristic)
//     { // 客户端读取事件回调函数
//         Serial.println("reading...");
//     }

//     void onWrite(BLECharacteristic *pCharacteristic)
//     { // 客户端写入事件回调函数
//         Serial.println("writing...");
//         std::string rxValue = pCharacteristic->getValue(); // 接收信息

//         if (rxValue.length() > 0)
//         { // 向串口输出收到的值
//             Serial.print("Rx: ");
//             for (int i = 0; i < rxValue.length(); i++)
//                 Serial.print(rxValue[i]);

//             Serial.println();
//             communicateBLE(rxValue); //	下面开始处理信息
//         }
//     }
// };

// void ble_init(void)
// {
//     BLEDevice::init(ble_name);
//     pServer = BLEDevice::createServer();
//     pService = pServer->createService(service_uuid);                     // 创建服务
//     BLECharacteristic *pCharacteristic = pService->createCharacteristic( // 创建特征
//         characteristic_uuid,
//         BLECharacteristic::PROPERTY_READ |
//             BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
//     pCharacteristic->setValue("SHANG");
//     pServer->setCallbacks(new MyServerCallbacks()); // 绑定回调函数
//     pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
//     pService->start(); // 启动服务
//     BLEDevice::startAdvertising();
// }

// void ble_routine()
// {
//     // deviceConnected	已连接
//     if (deviceConnected)
//     {
//         pCharacteristic->setValue(&txValue, 1); // 设置要发送的值为1
//         pCharacteristic->notify();              // 广播
//         txValue++;                                // 指针地址自加1
//         delay(2000);                              // 如果有太多包要发送，蓝牙会堵塞
//     }

//     // disconnecting  断连
//     if (!deviceConnected && oldDeviceConnected)
//     {
//         delay(500);                  // 留时间给蓝牙缓冲
//         pServer->startAdvertising(); // 重新广播
//         Serial.println("Began to Broadcast");
//         oldDeviceConnected = deviceConnected;
//     }

//     // connecting  正在连接
//     if (deviceConnected && !oldDeviceConnected)
//     {
//         // do stuff here on connecting
//         oldDeviceConnected = deviceConnected;
//     }
// }

// // ***********************************************************************
// //	@ 函数名称： communicateBLE
// //	@ 函数功能： BLE通讯的内容
// //	@ 函数输入： 无
// //	@ 函数输出： 无
// //	@ 注意事项： 无
// // ***********************************************************************
// void communicateBLE(std::string rxData)
// {
//     uint8_t Length = rxData.length();

//     if ('W' == rxData[0] && '*' == rxData[Length - 1])
//     {
//         Serial.printf("Ready for Wi-Fi\r\n");
//         BLE2WiFi(rxData);
//     }
// }

// // ***********************************************************************
// //	@ 函数名称： communicateBLE
// //	@ 函数功能： BLE通讯的内容
// //	@ 函数输入： 无
// //	@ 函数输出： 无
// //	@ 注意事项： 无
// // ***********************************************************************
// void BLE2WiFi(std::string WiFi_Data)
// {
//     uint8_t Length = WiFi_Data.length();
//     uint8_t Interval = 0, ConnectCnt = 0;

//     // 首先找到间隔'#'，确定SSID和PSWD的长度
//     for (uint8_t i = 0; i < Length; i++)
//     {
//         if ('#' == WiFi_Data[i])
//         {
//             Interval = i;
//         }
//     }

//     // 首先获取SSID
//     std::string Temp = "";
//     for (uint8_t i = 0; i < Interval - 1; i++)
//     {
//         Temp += WiFi_Data[i + 1];
//     }
//     char *SSID = new char[Interval - 1];
//     strcpy(SSID, Temp.c_str());
//     Serial.printf("SSID:%s\r\n", SSID);

//     // 然后才是PSWD
//     Temp = "";
//     for (uint8_t i = 0; i < Length - Interval - 2; i++)
//     {
//         Temp += WiFi_Data[i + 1 + Interval];
//     }
//     char *PSWD = new char[Length - Interval - 2];
//     strcpy(PSWD, Temp.c_str());
//     Serial.printf("PSWD:%s\r\n", PSWD);

//     // 准备连接Wi-Fi
//     Serial.printf("Connectingto WiFi.");
//     WiFi.begin(SSID, PSWD);
//     ConnectCnt = 0;
//     while (WL_CONNECTED != WiFi.status())
//     {
//         delay(1000);
//         Serial.printf(".");
//         if ((++ConnectCnt > 10) || (WL_CONNECTED == WiFi.status()))
//         {
//             break;
//         }
//     }

//     // 判断是否连接成功
//     if (WL_CONNECTED == WiFi.status())
//     {
//         Serial.printf("Connected to the WiFi network\r\n");
//         Serial.printf("IP address: ");
//         Serial.println(WiFi.localIP());
//     }
//     else
//     {
//         Serial.print("\r\nFile to connected WiFi\r\n");
//     }
// }
