#include "motorDriver.hpp"

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"

#include "logger.h"

#define CURVE_SAPLES_COUND 500
#define STEP_MOTOR_RESOLUTION_HZ 1000000

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_handle_t copy_encoder;
    uint32_t sample_points;
    struct {
        uint32_t is_accel_curve: 1;
    } flags;
    rmt_symbol_word_t curve_table[];
} rmt_stepper_curve_encoder_t;

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_handle_t copy_encoder;
    uint32_t resolution;
} rmt_stepper_uniform_encoder_t;

rmt_channel_handle_t rmt_channel_motor_a = NULL;
rmt_channel_handle_t rmt_channel_motor_b = NULL;
rmt_stepper_curve_encoder_t *accel_curve = NULL;
rmt_stepper_uniform_encoder_t *constant_curve = NULL;
rmt_stepper_curve_encoder_t *decel_curve = NULL;

static size_t rmt_encode_stepper_motor_curve(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_stepper_curve_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_curve_encoder_t, base);
    rmt_encoder_handle_t copy_encoder = motor_encoder->copy_encoder;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    uint32_t sample_points = *(uint32_t *)primary_data;
    size_t encoded_symbols = 0;
    if (motor_encoder->flags.is_accel_curve) {
        encoded_symbols = copy_encoder->encode(copy_encoder, channel, &motor_encoder->curve_table[0],
                                               sample_points * sizeof(rmt_symbol_word_t), &session_state);
    } else {
        encoded_symbols = copy_encoder->encode(copy_encoder, channel, &motor_encoder->curve_table[0] + motor_encoder->sample_points - sample_points,
                                               sample_points * sizeof(rmt_symbol_word_t), &session_state);
    }
    *ret_state = session_state;
    return encoded_symbols;
}

static esp_err_t rmt_del_stepper_motor_curve_encoder(rmt_encoder_t *encoder)
{
    rmt_stepper_curve_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_curve_encoder_t, base);
    rmt_del_encoder(motor_encoder->copy_encoder);
    free(motor_encoder);
    motor_encoder = NULL;
    return ESP_OK;
}

static esp_err_t rmt_reset_stepper_motor_curve_encoder(rmt_encoder_t *encoder)
{
    rmt_stepper_curve_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_curve_encoder_t, base);
    rmt_encoder_reset(motor_encoder->copy_encoder);
    return ESP_OK;
}

static size_t rmt_encode_stepper_motor_uniform(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_stepper_uniform_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_uniform_encoder_t, base);
    rmt_encoder_handle_t copy_encoder = motor_encoder->copy_encoder;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    uint32_t target_freq_hz = *(uint32_t *)primary_data;
    uint16_t symbol_duration = STEP_MOTOR_RESOLUTION_HZ / target_freq_hz / 2;
    rmt_symbol_word_t freq_sample = {
        .duration0 = symbol_duration,
        .level0 = 0,
        .duration1 = symbol_duration,
        .level1 = 1,
    };
    size_t encoded_symbols = copy_encoder->encode(copy_encoder, channel, &freq_sample, sizeof(freq_sample), &session_state);
    *ret_state = session_state;
    return encoded_symbols;
}

static esp_err_t rmt_del_stepper_motor_uniform_encoder(rmt_encoder_t *encoder)
{
    rmt_stepper_uniform_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_uniform_encoder_t, base);
    rmt_del_encoder(motor_encoder->copy_encoder);
    free(motor_encoder);
    return ESP_OK;
}

static esp_err_t rmt_reset_stepper_motor_uniform(rmt_encoder_t *encoder)
{
    rmt_stepper_uniform_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_uniform_encoder_t, base);
    rmt_encoder_reset(motor_encoder->copy_encoder);
    return ESP_OK;
}

static float convert_to_smooth_freq(uint32_t freq1, uint32_t freq2, uint32_t freqx)
{
    float normalize_x = ((float)(freqx - freq1)) / (freq2 - freq1);
    // third-order "smoothstep" function: https://en.wikipedia.org/wiki/Smoothstep
    float smooth_x = normalize_x * normalize_x * (3 - 2 * normalize_x);
    return smooth_x * (freq2 - freq1) + freq1;
}

MotorDriver::MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, gpio_num_t endStopXPin, gpio_num_t endStopYPin)
    : endStopXPin(endStopXPin), endStopYPin(endStopYPin), stepperAPins(pinsMotorA), stepperBPins(pinsMotorB)
{
    int ret;
    LOG_D("Initialize GPIO");
    gpio_config_t dir_gpio_config = {
        .pin_bit_mask = 1ULL << pinsMotorA.dir | 1ULL << pinsMotorB.dir,
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret = gpio_config(&dir_gpio_config);
    if (ret != 0)
    {
        LOG_C("faild to set dir pins as output");
        return;
    }
    gpio_config_t end_stop_gpio_config = {
        .pin_bit_mask = 1ULL << endStopXPin | 1ULL << endStopYPin,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret = gpio_config(&end_stop_gpio_config);
    if (ret != 0)
    {
        LOG_C("faild to set end stop pins as input");
        return;
    }

    LOG_D("Create RMT TX channels");
    rmt_tx_channel_config_t tx_chan_config_a = {
        .gpio_num = pinsMotorA.step,
        .clk_src = RMT_CLK_SRC_DEFAULT, // select clock source
        .resolution_hz = STEP_MOTOR_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .trans_queue_depth = 10, // set the number of transactions that can be pending in the background
    };
    ret = rmt_new_tx_channel(&tx_chan_config_a, &rmt_channel_motor_a);
    if (ret != 0)
    {
        LOG_C("faild to create rmt channel for motor a");
        return;
    }
    rmt_tx_channel_config_t tx_chan_config_b = {
        .gpio_num = pinsMotorB.step,
        .clk_src = RMT_CLK_SRC_DEFAULT, // select clock source
        .resolution_hz = STEP_MOTOR_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .trans_queue_depth = 10, // set the number of transactions that can be pending in the background
    };
    ret = rmt_new_tx_channel(&tx_chan_config_b, &rmt_channel_motor_b);
    if (ret != 0)
    {
        LOG_C("faild to create rmt channel for motor b");
        return;
    }

    LOG_D("Set spin direction");
    gpio_set_level(pinsMotorA.dir, STEPER_DIR_CW);
    gpio_set_level(pinsMotorB.dir, STEPER_DIR_CW);
}

MotorDriver::~MotorDriver()
{
    if (accel_curve != NULL)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
    }
    if (constant_curve != NULL)
    {
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
    }
    if (decel_curve != NULL)
    {
        rmt_del_stepper_motor_curve_encoder(&decel_curve->base);
    }
}

int MotorDriver::SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk)
{
    LOG_I("set speeds: maxSpeed=%lu mm/s, acceleration=%lu mm/s2, jurk=%lu mm/s", maxSpeed, acceleration, jurk);
    if (jurk >= maxSpeed)
    {
        LOG_E("jurk must be lower than maxSpeed. Continueing with 0 mm/s for jurk");
        jurk = 0;
    }

    maxSpeed *= stepsPerMMA;
    acceleration *= stepsPerMMA;
    this->acceleration_steps = (jurk + (maxSpeed - jurk)/2) * (maxSpeed - jurk) / acceleration;
    LOG_D("acceleration_steps: %lu", this->acceleration_steps);
    jurk *= stepsPerMMA;

    int ret;

    // generate accelaration curve
    if (accel_curve != NULL)
    {
        LOG_W("accel_curve already exsisted. removeing old curve");
        rmt_del_encoder(accel_curve->copy_encoder);
        free(accel_curve);
        accel_curve = NULL;
    }
    accel_curve = (rmt_stepper_curve_encoder_t*)rmt_alloc_encoder_mem(sizeof(rmt_stepper_curve_encoder_t) + this->acceleration_steps * sizeof(rmt_symbol_word_t));
    if (accel_curve == NULL)
    {
        LOG_C("faild to allocate memory for acceleration curve");
        return -1;
    }
    rmt_copy_encoder_config_t copy_encoder_config_accl = {};
    ret = rmt_new_copy_encoder(&copy_encoder_config_accl, &accel_curve->copy_encoder);
    if (ret != ESP_OK)
    {
        free(accel_curve);
        accel_curve = NULL;
        LOG_C("failed to create copy encoder for acceleration curve");
        return -2;
    }
    float curve_step = (maxSpeed - jurk) / (this->acceleration_steps - 1);
    accel_curve->sample_points = this->acceleration_steps;
    accel_curve->flags.is_accel_curve = true;
    accel_curve->base.del = &rmt_del_stepper_motor_curve_encoder;
    accel_curve->base.encode = &rmt_encode_stepper_motor_curve;
    accel_curve->base.reset = &rmt_reset_stepper_motor_curve_encoder;
    
    for (uint32_t i = 0; i < this->acceleration_steps; i++)
    {
        float smooth_freq = convert_to_smooth_freq(jurk, maxSpeed, jurk + curve_step * i);
        uint16_t symbol_duration = STEP_MOTOR_RESOLUTION_HZ / smooth_freq / 2;
        accel_curve->curve_table[i].level0 = 0;
        accel_curve->curve_table[i].duration0 = symbol_duration;
        accel_curve->curve_table[i].level1 = 1;
        accel_curve->curve_table[i].duration1 = symbol_duration;
    }

    // generate constant curve
    if (constant_curve != NULL)
    {
        LOG_W("constant_curve already exsisted. removeing old curve");
        free(constant_curve);
    }
    constant_curve = (rmt_stepper_uniform_encoder_t*)rmt_alloc_encoder_mem(sizeof(rmt_stepper_uniform_encoder_t));
    if (constant_curve == NULL)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
        LOG_C("faild to allocate memory for constant curve");
        return -1;
    }
    rmt_copy_encoder_config_t copy_encoder_config_const = {};
    ret = rmt_new_copy_encoder(&copy_encoder_config_const, &constant_curve->copy_encoder);
    if (ret != ESP_OK)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
        free(accel_curve);
        accel_curve = NULL;
        LOG_C("failed to create copy encoder for constant curve");
        return -2;
    }
    constant_curve->resolution = STEP_MOTOR_RESOLUTION_HZ;
    constant_curve->base.del = &rmt_del_stepper_motor_uniform_encoder;
    constant_curve->base.encode = &rmt_encode_stepper_motor_uniform;
    constant_curve->base.reset = &rmt_reset_stepper_motor_uniform;

    // generate decalaration curve
    if (decel_curve != NULL)
    {
        LOG_W("decel_curve already exsisted. removeing old curve");
        rmt_del_encoder(decel_curve->copy_encoder);
        free(decel_curve);
        decel_curve = NULL;
    }
    decel_curve = (rmt_stepper_curve_encoder_t*)rmt_alloc_encoder_mem(sizeof(rmt_stepper_curve_encoder_t) + this->acceleration_steps * sizeof(rmt_symbol_word_t));
    if (decel_curve == NULL)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
        LOG_C("faild to allocate memory for deceleration curve");
        return -1;
    }
    rmt_copy_encoder_config_t copy_encoder_config_decel = {};
    ret = rmt_new_copy_encoder(&copy_encoder_config_decel, &decel_curve->copy_encoder);
    if (ret != ESP_OK)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
        free(decel_curve);
        decel_curve = NULL;
        LOG_C("failed to create copy encoder for deceleration curve");
        return -2;
    }
    curve_step = (maxSpeed - jurk) / (this->acceleration_steps - 1);
    decel_curve->sample_points = this->acceleration_steps;
    decel_curve->flags.is_accel_curve = false;
    decel_curve->base.del = &rmt_del_stepper_motor_curve_encoder;
    decel_curve->base.encode = &rmt_encode_stepper_motor_curve;
    decel_curve->base.reset = &rmt_reset_stepper_motor_curve_encoder;
    for (uint32_t i = 0; i < this->acceleration_steps; i++)
    {
        float smooth_freq = convert_to_smooth_freq(jurk, maxSpeed, jurk + curve_step * i);
        uint16_t symbol_duration = STEP_MOTOR_RESOLUTION_HZ / smooth_freq / 2;
        decel_curve->curve_table[this->acceleration_steps - i - 1].level0 = 0;
        decel_curve->curve_table[this->acceleration_steps - i - 1].duration0 = symbol_duration;
        decel_curve->curve_table[this->acceleration_steps - i - 1].level1 = 1;
        decel_curve->curve_table[this->acceleration_steps - i - 1].duration1 = symbol_duration;
    }

    ret = rmt_enable(rmt_channel_motor_a);
    if (ret != 0)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
        rmt_del_stepper_motor_curve_encoder(&decel_curve->base);
        LOG_C("failed to enable rmt channel a");
        return -3;
    }

    ret = rmt_enable(rmt_channel_motor_b);
    if (ret != 0)
    {
        rmt_del_stepper_motor_curve_encoder(&accel_curve->base);
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
        rmt_del_stepper_motor_curve_encoder(&decel_curve->base);
        LOG_C("failed to enable rmt channel b");
        return -3;
    }
    return 0;
}

void MotorDriver::SetStepsPerMM(int32_t a, int32_t b)
{
    stepsPerMMA = a;
    stepsPerMMB = b;
    LOG_D("steps per mm: A=%li, B=%li", stepsPerMMA, stepsPerMMB);
}

void MotorDriver::move(float x, float y, uint32_t speed)
{
    LOG_D("move: mm (%f, %f), speed=%lu", x, y, speed);
    int32_t stepsA = (-y + x) * stepsPerMMA;
    int32_t stepsB = (-y - x) * stepsPerMMB;

    if (stepsA > 0)
    {
        gpio_set_level(stepperAPins.dir, STEPER_DIR_CW);
    }
    else
    {
        stepsA = -stepsA;
        gpio_set_level(stepperAPins.dir, STEPER_DIR_CCW);
    }
    
    if (stepsB > 0)
    {
        gpio_set_level(stepperBPins.dir, STEPER_DIR_CW);
    }
    else
    {
        stepsB = -stepsB;
        gpio_set_level(stepperBPins.dir, STEPER_DIR_CCW);
    }
    LOG_D("move: steps A=%li, B=%li", stepsA, stepsB);

    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };
    uint32_t curve_samples;

    // stepper A
    if (stepsA < this->acceleration_steps * 2)
    {
        LOG_D("move: less steps than accel + decel. do parcel accel and decel");

        // acceleraton
        curve_samples = stepsA / 2;
        rmt_transmit(rmt_channel_motor_a, &accel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        curve_samples += stepsA & 0x1; // add one if odd
        rmt_transmit(rmt_channel_motor_a, &decel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
    }
    else
    {
        LOG_D("move: full curve");

        // acceleraton
        curve_samples = this->acceleration_steps;
        tx_config.loop_count = 0;
        rmt_transmit(rmt_channel_motor_a, &accel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // constant speed
        tx_config.loop_count = stepsA - this->acceleration_steps*2;
        rmt_transmit(rmt_channel_motor_a, &constant_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        tx_config.loop_count = 0;
        rmt_transmit(rmt_channel_motor_a, &decel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
    }

    tx_config.loop_count = 0;
    // stepper B
    if (stepsB < this->acceleration_steps * 2)
    {
        // acceleraton
        curve_samples = stepsB / 2;
        rmt_transmit(rmt_channel_motor_b, &accel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        curve_samples += stepsB & 0x1; // add one if odd
        rmt_transmit(rmt_channel_motor_b, &decel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
    }
    else
    {
        // acceleraton
        curve_samples = this->acceleration_steps;
        rmt_transmit(rmt_channel_motor_b, &accel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // constant speed
        tx_config.loop_count = stepsB - this->acceleration_steps*2;
        rmt_transmit(rmt_channel_motor_b, &constant_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        tx_config.loop_count = 0;
        rmt_transmit(rmt_channel_motor_b, &decel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
    }
}

void MotorDriver::home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY)
{
    rmt_transmit_config_t tx_config = {
        .loop_count = maxMove,
    };

    gpio_set_level(stepperAPins.dir, STEPER_DIR_CCW);
    gpio_set_level(stepperBPins.dir, STEPER_DIR_CW);

    rmt_transmit(rmt_channel_motor_a, &constant_curve->base, &speed, sizeof(uint32_t), &tx_config);

    //TODO: check if motors are finished
    while (true && (gpio_get_level(endStopXPin) == 1))
    {
        // do nothing?
    }
    //TODO: stop motors

    gpio_set_level(stepperAPins.dir, STEPER_DIR_CW);
    gpio_set_level(stepperBPins.dir, STEPER_DIR_CW);

    rmt_transmit(rmt_channel_motor_a, &constant_curve->base, &speed, sizeof(uint32_t), &tx_config);


    while (true && (gpio_get_level(endStopYPin) == 0))
    {
        // do nothing?
    }
    //TODO: stop motors

    // move to center of (0,0)
    move(offsetX, offsetY, jurk);
}
