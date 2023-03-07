// 使用旋转编码器作为输入设备

#include "input.h"

static int64_t encoder_diff, encoder_position;
static lv_indev_state_t encoder_state;
lv_indev_t *indev_encoder;
lv_indev_drv_t indev_drv;
lv_group_t *group;
int aState, aLastState;
ESP32Encoder encoder;

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
    encoder.attachHalfQuad(ENCODER_DT, ENCODER_CLK);
    encoder.setCount(0);
    pinMode(ENCODER_SW, INPUT_PULLUP);
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    encoder_diff = encoder.getCount() - encoder_position;
    encoder_position = encoder.getCount();
    data->enc_diff = encoder_diff;

    if (digitalRead(ENCODER_SW) == LOW)
    {
        if (digitalRead(ENCODER_SW) == LOW)
        {
            encoder_state = LV_INDEV_STATE_PR;
        }
    }
    data->state = encoder_state;
    // encoder_diff = 0;
    encoder_state = LV_INDEV_STATE_REL;
    // Serial.printf("\nnew position: %d\n", encoder_position);
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
