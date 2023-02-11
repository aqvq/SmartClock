#include <Drivers/Time.h>

void updateTime(struct tm *timeinfo)
{
    getLocalTime(timeinfo);
    Serial.println(timeinfo, "%F %T %A"); // 格式化输出:2021-10-24 23:00:44 Sunday
}
