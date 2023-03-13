#include "clock.h"

std::bitset<max_clock_num> clock_bitmap;
StoredConfig stored_config;

void clock_init()
{
    Serial.println("Opening SPIFFS");
    while (!SPIFFS.begin(true)) // 首次进行文件系统初始化，后续保持不变
    {
        Serial.println("...");
    }
    Serial.println("SPIFFS OK!");
    config_read_all();
}

void clock_read_all()
{
    if (SPIFFS.exists(data_file))
    {
        File file = SPIFFS.open(data_file, FILE_READ);
        if (!file)
        {
            Serial.println("There was an error opening the file for reading");
            return;
        }
        clock_bitmap = std::bitset<max_clock_num>(std::string(file.readString().c_str()));
        // Serial.println(clock_bitmap.to_string().c_str());
        file.close();
    }
}

void clock_save_all()
{
    File file = SPIFFS.open(data_file, FILE_WRITE);
    if (!file)
    {
        Serial.println("There was an error opening the file for writing");
        return;
    }
    file.print(clock_bitmap.to_string().c_str());
    // Serial.println(file.readString());
    file.close();
}

void clock_set(int i, int j)
{
    clock_bitmap.set(i * 60 + j);
}

void clock_reset(int i, int j)
{
    clock_bitmap.reset(i * 60 + j);
}

bool clock_get(int i, int j)
{
    return clock_bitmap[i * 60 + j];
}

void config_read_all()
{
    if (SPIFFS.exists(config_file))
    {
        File file = SPIFFS.open(config_file, FILE_READ);
        if (!file)
        {
            Serial.println("There was an error opening the file for reading");
            return;
        }
        stored_config.bright = file.parseInt();
        stored_config.audio = file.parseInt();
        file.close();
    }
    else
    {
        stored_config.bright = 100;
        stored_config.audio = 100;
    }
}

void config_save_all()
{
    File file = SPIFFS.open(config_file, FILE_WRITE);
    if (!file)
    {
        Serial.println("There was an error opening the file for writing");
        return;
    }
    file.printf("%d,%d", stored_config.bright, stored_config.audio);
    // Serial.println(file.readString());
    file.close();
}