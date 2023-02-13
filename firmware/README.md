# 智能家庭时钟

## 介绍

本项目是智能家庭时钟的固件程序部分，使用Arduino编写。

## 实现功能

- 实时联网校时
- 实现温湿度检测
- 实现数据实时上传手机
- 实现SmartConfig一键配网
- 实现中文显示
- 实现分页显示
- 实现编码器控制

## 安装过程

1. 在VSCODE中安装`PlatformIO IDE`插件
2. `PlatformIO`会自动安装环境依赖，请等待安装结束
3. 参考[配置要求](#配置要求)部分进行项目环境的配置
4. 编译烧录固件

## 配置要求
1. 配置`TFT_eSPI/User_Setup.h`中的参数，`include/TFT_Setup.h`为已有的模板可供参考
2. 配置`lvgl/lv_conf.h`中的参数，`include/lv_conf.h`为已有的模板可供参考
3. 配置`DHT`，将`DHT_U.cpp`和`DHT_U.h`删除

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