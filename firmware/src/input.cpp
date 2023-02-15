// 使用旋转编码器作为输入设备

#include "input.h"

static ESP32Encoder encoder;
static int32_t encoder_diff;
static lv_indev_state_t encoder_state;
lv_indev_t *indev_encoder;
lv_indev_drv_t indev_drv;
lv_group_t *group;

void input_init(void)
{
    encoder_init();
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

    group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(indev_encoder, group);
}

/*Initialize your keypad*/
static void encoder_init(void)
{
    /*Your code comes here*/
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_C, INPUT_PULLUP);
    encoder.attachHalfQuad(ENCODER_A, ENCODER_B);
    encoder.setCount(0);
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static long lastCount = 0;
    long newCount = encoder.getCount() / 2;
    // Serial.printf("\nencoder_count: %ld\n", newCount);
    data->enc_diff = newCount - lastCount;
    if (digitalRead(ENCODER_C) == LOW)
    {
        delay(10);
        if (digitalRead(ENCODER_C) == LOW)
        {
            encoder_state = LV_INDEV_STATE_PR;
        }
    }
    data->state = encoder_state;
    lastCount = newCount;
    encoder_state = LV_INDEV_STATE_REL;
    // Serial.printf("\nenc_diff: %d\n", data->enc_diff);
    // Serial.printf("\nstate: %d\n", data->state);
}

/*Call this function in an interrupt to process encoder events (turn, press)*/
static void encoder_handler(void)
{
    /*Your code comes here*/
    encoder_state = LV_INDEV_STATE_REL;
    encoder_diff += 0;
}
