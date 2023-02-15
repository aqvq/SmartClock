# 智能家庭时钟——单片机课程设计

## 题目要求
1. 在液晶显示屏上显示“*年*月*日 实时时间”（DS1302）;
2. 通过矩阵键盘设置时间；
3. 整点语音报时；
4. 可设多个闹钟；
5. 可以做定时器；
6. 通过温湿度传感器采集温、湿度，通过液晶屏显示温、湿度；

## 技术要点
- ESP32
- Arduino
- LVGL

## 项目概括

本项目硬件部分主要使用：

- ESP32主控芯片
- TFT LCD显示屏幕
- DHT11温湿度传感器
- 蜂鸣器
- 旋转编码器

本项目固件部分主要使用：

- Arduino为主要框架
- TFT-eSPI为底层显示驱动
- LVGL为图形化库

本项目实现了题目要求的诸如：

- 实时显示时间、日期、温湿度数据信息
- 实现整点报时
- 实现计时与闹钟功能等

此外，我还增加了如下额外功能：

- 将键盘输入改为旋转编码器输入，提升了用户体验，易于上手

- 还实现了WiFi联网功能，能实时同步网络时间，以此解决单片机本地晶振计时不准的问题，省去了用户校时的麻烦
- 本项目使用LVGL绘制UI，使界面更加现代化，更具有美感，增强了用户交互体验

## 文件结构

> 更详细的项目逻辑解释在源代码的注释中

- /firmware 固件程序源代码
- /hardware 硬件电路相关资料
  - /include 程序头文件
  - /lib 程序所需的库
  - /src 程序源代码
    - main.cpp 主程序
    - network.cpp WiFi模块
    - ui.cpp 交互界面模块
    - display.cpp 显示驱动模块
    - input.cpp 输入设备驱动模块
    - sensors.cpp 传感器模块
    - clock.cpp 闹钟数据处理模块
    - /fonts 汉字字库
- /docs 技术文档

## 参考资料

- [超详细ESP32驱动TFT屏幕教程，有手就行_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1Q24y1d7f6/?vd_source=653c32e1b9068b6dc916bedc5c6c2c7d)
- [ESP32与微信小程序_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1tv411w74d/?p=10&spm_id_from=333.880.my_history.page.click&vd_source=653c32e1b9068b6dc916bedc5c6c2c7d)
- [Homepea7/ESP32_Code: 一些ESP32的代码 (github.com)](https://github.com/Homepea7/ESP32_Code)
- [Online font converter - TTF or WOFF fonts to C array | LVGL](https://lvgl.io/tools/fontconverter)
- [欢迎阅读百问网LVGL中文开发手册！ — 百问网LVGL中文教程文档 文档 (100ask.net)](http://lvgl.100ask.net/8.2/index.html)
- [peng-zhihui/HoloCubic: 带网络功能的伪全息透明显示桌面站 (github.com)](https://github.com/peng-zhihui/HoloCubic)
- [基于Arduino框架下的ESP32移植LVGL教程_Yogurt-994的博客-CSDN博客_esp32 lvgl](https://blog.csdn.net/qq_52619462/article/details/128294156)
- [Online Font Converter Convert TTF and WOFF fonts to C array.](https://lvgl.io/tools/fontconverter)
- [lvgl使用旋转编码器做为外部输入设备](https://blog.csdn.net/wojueburenshu/article/details/123033471)
- [ESP32的FLASH、NVS、SPIFFS、OTA等存储分布以及启动过程](https://blog.csdn.net/tiandiren111/article/details/123144089)
- [《C++ Primer》笔记 第17章 标准库特殊设施 ](https://www.cnblogs.com/juzaizai/p/15082749.html)
- [玩转 ESP32 + Arduino (二十一) SPIFFS文件系统](https://blog.csdn.net/finedayforu/article/details/108661563)

