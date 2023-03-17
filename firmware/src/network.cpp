#include "network.h"

// const char* AP_PASS  = "";               //这里不设置设置AP热点密码
String scanNetworksID = "";     // 用于储存扫描到的WiFi ID
IPAddress apIP(192, 168, 4, 1); // 设置AP的IP地址
String wifi_ssid = "";          // 暂时存储wifi账号密码
String wifi_pass = "";          // 暂时存储wifi账号密码
bool resetPressed = false;
static unsigned long timePressed = 0;
DNSServer dnsServer;       // 创建dnsServer实例
WebServer server(webPort); // 开启web服务, 创建TCP SERVER,参数: 端口号,最大连接数
bool first_sync = false;
unsigned long sync_time;
extern RtcDS1302<ThreeWire> Rtc;

#define ROOT_HTML "<!DOCTYPE html><html><head><title>WIFI</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input {display: block;margin-top: 10px;}.input span {width: 100px;float: left;float: left;height: 36px;line-height: 36px;}.input input {height: 30px;width: 200px;}.btn {width: 120px;height: 35px;background-color: #000000;border: 0px;color: #ffffff;margin-top: 15px;margin-left: 100px;}form {display: flex;flex-direction: column;align-items: center;}</style><body><form method=\"POST\" action=\"configwifi\"><label class=\"input\"><span>WiFi名称</span><input type=\"text\" name=\"ssid\" placeholder=\"请输入WiFi名称\" required oninvalid=\"validate('WiFi名称不能为空')\"></label><label class=\"input\"><span>WiFi密码</span><input type=\"password\" name=\"pass\" placeholder=\"请输入WiFi密码\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"连接\"><p class=\"near\"><br><span> 附近的WiFi </span></p>"

/*
 * 重置设备
 */
void reset_device()
{
    Serial.println("\nErasing WiFi data.");
    eraseWiFi();   // 删除保存的wifi信息
    ESP.restart(); // 重启复位esp32
}

/*
 * 网络模块初始化
 */
void network_init()
{
#if ENABLE_BLINKER
    blinker_init(); // 云服务初始化
#endif
    // Serial.begin(115200);            // 波特率
    pinMode(resetPin, INPUT_PULLUP); // 按键上拉输入模式(默认高电平输入,按下时下拉接到低电平)
    LEDinit();                       // LED用于显示WiFi状态
    connect_wifi_init();             // 连接wifi，传入的是wifi连接等待时间15
}

/*
 * 实时处理网络状况
 */
void network_routine()
{
#if ENABLE_BLINKER
    blinker_routine();
#endif
    // 监听重置按键长按状态
    if (digitalRead(resetPin) == LOW)
    {
        if (resetPressed == false)
        {
            resetPressed = true;
            timePressed = millis();
            Serial.println(millis());
        }
        else
        {
            if ((millis() - timePressed) > 5000)
            {
                Serial.println(millis());
                // reset_device();
                connect_wifi_config(); // 配置wifi
            }
        }
    }
    else
    {
        resetPressed = false;
    }
    // 监听网络状态
    checkDNS_HTTP(); // 检测客户端DNS&HTTP请求，也就是检查配网页面那部分
    checkConnect();  // 检测网络连接状态，参数true表示如果断开重新连接
    if (!first_sync && WiFi.status() == WL_CONNECTED)
    {
        first_sync = true;
        config_ntp_time();
        sync_time = millis();
    }
    if (WiFi.status() != WL_CONNECTED || (millis() - sync_time >= 1000 * 60 * 60))
    {
        first_sync = false;
    }
}

void closeWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect(true); // 断开wifi网络
        WiFi.mode(WIFI_OFF);   // 关闭网络
    }
}

/*
 * 处理网站根目录的访问请求
 */
void handleRoot()
{
    if (server.hasArg("selectSSID"))
    {
        server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</form></body></html>"); // scanNetWprksID是扫描到的wifi
    }
    else
    {
        server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</form></body></html>");
    }
}

/*
 * 提交数据后的提示页面
 */
void handleConfigWifi() // 返回http状态
{
    if (server.hasArg("ssid")) // 判断是否有账号参数
    {
        Serial.print("Got ssid: ");
        wifi_ssid = server.arg("ssid"); // 获取html表单输入框name名为"ssid"的内容

        Serial.println(wifi_ssid);
    }
    else // 没有参数
    {
        Serial.println("Error, not found ssid");
        server.send(200, "text/html", "<meta charset='UTF-8'>请输入WiFi名称"); // 返回错误页面
        return;
    }
    // 密码与账号同理
    if (server.hasArg("pass"))
    {
        Serial.print("Got password:");
        wifi_pass = server.arg("pass"); // 获取html表单输入框name名为"pwd"的内容
        Serial.println(wifi_pass);
    }
    else
    {
        Serial.println("Error, not found password");
        server.send(200, "text/html", "<meta charset='UTF-8'>请输入密码");
        return;
    }
    server.send(200, "text/html", "<meta charset='UTF-8'>已取得WiFi信息,正在尝试连接..."); // 返回保存成功页面
    // delay(2000);
    WiFi.softAPdisconnect(true); // 参数设置为true，设备将直接关闭接入点模式，即关闭设备所建立的WiFi网络。
    server.close();              // 关闭web服务
    WiFi.softAPdisconnect();     // 在不输入参数的情况下调用该函数,将关闭接入点模式,并将当前配置的AP热点网络名和密码设置为空值.
    Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

    if (WiFi.status() != WL_CONNECTED) // wifi没有连接成功
    {
        Serial.println("Executing connectToWiFi()...");
        connect_wifi_init();
    }
    else
    {
        Serial.println("Connecting WiFi successfully..");
    }
}

/*
 * 处理404情况的函数'handleNotFound'
 */
void handleNotFound() // 当浏览器请求的网络资源无法在服务器找到时通过此自定义函数处理
{
    handleRoot(); // 访问不存在目录则返回配置页面
    //   server.send(404, "text/plain", "404: Not found");
}

/*
 * 进入AP模式
 */
void initSoftAP()
{
    WiFi.mode(WIFI_AP);                                         // 配置为AP模式
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // 设置AP热点IP和子网掩码
    if (WiFi.softAP(AP_SSID))                                   // 开启AP热点,如需要密码则添加第二个参数
    {
        // 打印相关信息
        Serial.println("ESP-32S SoftAP is right.");
        Serial.print("Soft-AP IP address = ");
        Serial.println(WiFi.softAPIP());                                            // 接入点ip
        Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str()); // 接入点mac
    }
    else // 开启AP热点失败
    {
        Serial.println("WiFiAP Failed");
        // delay(1000);
        Serial.println("Restarting now...");
        ESP.restart(); // 重启复位esp32
    }
}

/*
 * 开启DNS服务器
 */
void initDNS()
{
    if (dnsServer.start(DNS_PORT, "*", apIP)) // 判断将所有地址映射到esp32的ip上是否成功
    {
        Serial.println("Starting DNS server succeeded.");
    }
    else
    {
        Serial.println("Starting DNS server failed.");
    }
}

/*
 * 初始化WebServer
 */
void initWebServer()
{
    if (MDNS.begin("esp32")) // 给设备设定域名esp32,完整的域名是esp32.local
    {
        Serial.println("MDNS responder started!");
    }
    // 必须添加第二个参数HTTP_GET，以下面这种格式去写，否则无法强制门户
    server.on("/", HTTP_GET, handleRoot);                  //  当浏览器请求服务器根目录(网站首页)时调用自定义函数handleRoot处理，设置主页回调函数，必须添加第二个参数HTTP_GET，否则无法强制门户
    server.on("/configwifi", HTTP_POST, handleConfigWifi); //  当浏览器请求服务器/configwifi(表单字段)目录时调用自定义函数handleConfigWifi处理

    server.onNotFound(handleNotFound); // 当浏览器请求的网络资源无法在服务器找到时调用自定义函数handleNotFound处理

    server.begin(); // 启动TCP SERVER

    Serial.println("WebServer started!");
}

/*
 * 扫描附近的WiFi，为了显示在配网界面
 */
bool scanWiFi()
{
    Serial.println("Scan starting");
    Serial.println("--------->");
    // 扫描附近WiFi
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0)
    {
        Serial.println("No network found.");
        scanNetworksID = "No network found.";
        return false;
    }
    else
    {
        Serial.print(n);
        Serial.println("Networks found:");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            scanNetworksID += "<P>" + WiFi.SSID(i) + "</P>";
            // delay(10);
        }
        return true;
    }
}

/*
 * 连接WiFi
 */
void connect_wifi_init()
{
    WiFi.hostname(HOST_NAME); // 设置设备名
    Serial.println("Executing connectToWiFi()");
    WiFi.mode(WIFI_STA);       // 设置为STA模式并连接WIFI
    WiFi.setAutoConnect(true); // 设置自动连接

    if (wifi_ssid != "") // wifi_ssid不为空，意味着从网页读取到wifi
    {
        Serial.println("Connecting by web data.");
        WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str()); // c_str(),获取该字符串的指针
        wifi_ssid = "";
        wifi_pass = "";
    }
    else // 未从网页读取到wifi
    {
        Serial.println("Connecting by nvs data.");
        WiFi.begin(); // begin()不传入参数，默认连接上一次连接成功的wifi
    }
    if (WiFi.status() == WL_CONNECTED) // 如果连接成功
    {
        Serial.println("WIFI connect Success");
        Serial.printf("SSID:%s", WiFi.SSID().c_str());
        Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
        Serial.print("LocalIP:");
        Serial.print(WiFi.localIP());
        Serial.print(" ,GateIP:");
        Serial.println(WiFi.gatewayIP());
        Serial.print("WIFI status is:");
        Serial.print(WiFi.status());
        digitalWrite(LED, HIGH);
        if (wifi_ssid != "")
            server.stop(); // 停止开发板所建立的网络服务器。
        // WiFi在另一个线程中执行，不一定恰好在此处时已连接
        // configTime(8 * 3600, 0, ntp_server);
        // rtc_sync_time();
    }
}

/*
 * 配置WiFi
 */
void connect_wifi_config()
{
    Serial.println(""); // 主要目的是为了换行符
    Serial.println("WIFI auto connection failed, starting AP for web configuration now...");
    blinkLED(LED, 3, 200); // LED闪烁
    // digitalWrite(LED, LOW); // 关于LED，不需要可删除
    wifiConfig(); // 开始配网功能
}

/*
 * 连接WiFi
 */
void connectToWiFi(int timeOut_s)
{
    WiFi.hostname(HOST_NAME); // 设置设备名
    Serial.println("Executing connectToWiFi()");
    WiFi.mode(WIFI_STA);       // 设置为STA模式并连接WIFI
    WiFi.setAutoConnect(true); // 设置自动连接
    if (wifi_ssid != "")       // wifi_ssid不为空，意味着从网页读取到wifi
    {
        Serial.println("Connecting by web data.");
        WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str()); // c_str(),获取该字符串的指针
        wifi_ssid = "";
        wifi_pass = "";
    }
    else // 未从网页读取到wifi
    {
        Serial.println("Connecting by nvs data.");
        WiFi.begin(); // begin()不传入参数，默认连接上一次连接成功的wifi
    }

    int Connect_time = 0;                 // 用于连接计时，如果长时间连接不成功，复位设备
    while (WiFi.status() != WL_CONNECTED) // 等待WIFI连接成功
    {
        Serial.print("."); // 一共打印30个点点
        digitalWrite(LED, !digitalRead(LED));
        delay(500);
        Connect_time++;

        if (Connect_time > 2 * timeOut_s) // 长时间连接不上，重新进入配网页面
        {
            digitalWrite(LED, LOW);
            Serial.println(""); // 主要目的是为了换行符
            Serial.println("WIFI auto connection failed, starting AP for web configuration now...");
            wifiConfig(); // 开始配网功能
            return;       // 跳出 防止无限初始化
        }
    }

    if (WiFi.status() == WL_CONNECTED) // 如果连接成功
    {
        Serial.println("WIFI connect Success");
        Serial.printf("SSID:%s", WiFi.SSID().c_str());
        Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
        Serial.print("LocalIP:");
        Serial.print(WiFi.localIP());
        Serial.print(" ,GateIP:");
        Serial.println(WiFi.gatewayIP());
        Serial.print("WIFI status is:");
        Serial.print(WiFi.status());
        digitalWrite(LED, HIGH);
        server.stop(); // 停止开发板所建立的网络服务器。
        // configTime(8 * 3600, 0, ntp_server);
        // rtc_sync_time();
    }
}

/*
 * 配置配网功能
 */
void wifiConfig()
{
    initSoftAP();
    initDNS();
    initWebServer();
    scanWiFi();
}

/*
 * 删除保存的wifi信息，这里的删除是删除存储在flash的信息。删除后wifi读不到上次连接的记录，需重新配网
 */
void eraseWiFi()
{
    esp_wifi_restore(); // 删除保存的wifi信息
    Serial.println("Ready to connect a new device..");
    blinkLED(LED, 3, 200);  // LED闪烁5次         //关于LED，不需要可删除
    digitalWrite(LED, LOW); // 关于LED，不需要可删除
}

/*
 * 检查wifi是否已经连接
 */
// void checkConnect(bool reConnect)
void checkConnect()
{
    if (WiFi.status() != WL_CONNECTED) // wifi连接失败
    {
        if (digitalRead(LED) != LOW)
            digitalWrite(LED, LOW);
        // if (reConnect == true && WiFi.getMode() != WIFI_AP && WiFi.getMode() != WIFI_AP_STA)
        // {
        //     Serial.println("WIFI not connected.");
        //     Serial.print("WiFi Mode: ");
        //     Serial.println(WiFi.getMode());
        //     Serial.println("Connecting WiFi...");
        //     connectToWiFi(connectTimeOut_s); // 连接wifi函数
        // }
    }
    else if (digitalRead(LED) != HIGH)
        digitalWrite(LED, HIGH); // wifi连接成功
}

/*
 * LED闪烁函数        //用不上LED可删除
 */
void blinkLED(int led, int n, int t)
{
    for (int i = 0; i < 2 * n; i++)
    {
        digitalWrite(led, !digitalRead(led));
        delay(t);
    }
}

/*
 * LED初始化
 */
void LEDinit()
{
    pinMode(LED, OUTPUT);   // 配置LED口为输出口
    digitalWrite(LED, LOW); // 初始灯灭
}

/*
 * 检测客户端DNS&HTTP请求
 */
void checkDNS_HTTP()
{
    dnsServer.processNextRequest(); // 检查客户端DNS请求
    server.handleClient();          // 检查客户端(浏览器)http请求
}

void config_ntp_time()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        struct tm timeinfo;
        configTime(8 * 3600, 0, ntp_server);
        getLocalTime(&timeinfo);
        if (timeinfo.tm_year != 70)
        {
            RtcDateTime sync_time(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
            Rtc.SetDateTime(sync_time);
        }
    }
}