// 闹钟数据存储与读取模块
// 使用位图进行数据保存，每一个位为一个闹钟，置1代表启用，置0代表禁用
// 一共有24*60个位，使用bitset进行存储

#ifndef SHANG_CLOCK_H
#define SHANG_CLOCK_H
#include <SPIFFS.h>
#include <bitset>
#include "conf.h"

// 初始化文件系统
void clock_init();
// 从文件中读取所有数据
void clock_read_all();
// 将所有数据保存到文件中
void clock_save_all();
// 置位某一时刻的闹钟
void clock_set(int i, int j);
// 复位某一时刻的闹钟
void clock_reset(int i, int j);
// 获取某一时刻的闹钟的状态
bool clock_get(int i, int j);

#endif