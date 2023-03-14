// 语音模块，实现语音播报和闹铃等音效
// 使用DFPlayer Mini模块

#ifndef AUDIO_SHANG
#define AUDIO_SHANG

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "clock.h"

// 音频初始化
void audio_init();
// 音频执行例程
void audio_routine();
// 打印调试信息
void printDetail(uint8_t type, int value);

#endif