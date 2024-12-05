#include "motorDriver.hpp"

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define CURVE_SAPLES_COUND 500

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

rmt_channel_handle_t motor_chan = NULL;
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

static float convert_to_smooth_freq(uint32_t freq1, uint32_t freq2, uint32_t freqx)
{
    float normalize_x = ((float)(freqx - freq1)) / (freq2 - freq1);
    // third-order "smoothstep" function: https://en.wikipedia.org/wiki/Smoothstep
    float smooth_x = normalize_x * normalize_x * (3 - 2 * normalize_x);
    return smooth_x * (freq2 - freq1) + freq1;
}

MotorDriver::MotorDriver(motorPins_t pinsMotorA, motorPins_t pinsMotorB, uint32_t endStopXPin, uint32_t endStopYPin)
    : endStopXPin(endStopXPin), endStopYPin(endStopYPin)
{
    ESP_LOGI(TAG, "Initialize GPIO");
    gpio_config_t dir_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = 1ULL << pinsMotorA.dir | 1ULL << pinsMotorB.dir,
    };
    ESP_ERROR_CHECK(gpio_config(&en_dir_gpio_config));
    gpio_config_t end_stop_gpio_config = {
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = 1ULL << endStopXPin | 1ULL << endStopYPin,
    };
    ESP_ERROR_CHECK(gpio_config(&en_dir_gpio_config));

    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select clock source
        .gpio_num = pinsMotorA.step,
        .mem_block_symbols = 64,
        .resolution_hz = STEP_MOTOR_RESOLUTION_HZ,
        .trans_queue_depth = 10, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &motor_chan));

    ESP_LOGI(TAG, "Set spin direction");
    gpio_set_level(pinsMotorA.dir, STEP_MOTOR_SPIN_DIR_CLOCKWISE);
    gpio_set_level(pinsMotorB.dir, STEP_MOTOR_SPIN_DIR_CLOCKWISE);
}

MotorDriver::~MotorDriver()
{
    if (accel_curve != NULL)
    {
        free(accel_curve);
    }
    if (decel_curve != NULL)
    {
        free(decel_curve);
    }
}

void MotorDriver::SetSpeeds(uint32_t maxSpeed, uint32_t acceleration, uint32_t jurk)
{
    //TODO: speeds are in mm/sec need to be converted to steps/sec, but this can be diffrent per motor
    // generate accelaration curve
    if (accel_curve != NULL)
    {
        free(accel_curve);
    }
    accel_curve = rmt_alloc_encoder_mem(sizeof(rmt_stepper_curve_encoder_t) + CURVE_SAPLES_COUND * sizeof(rmt_symbol_word_t));
    float curve_step = (maxSpeed - jurk) / (CURVE_SAPLES_COUND - 1);
    accel_curve->sample_points = CURVE_SAPLES_COUND;
    accel_curve->flags.is_accel_curve = true;
    for (uint32_t i = 0; i < CURVE_SAPLES_COUND; i++)
    {
        smooth_freq = convert_to_smooth_freq(jurk, maxSpeed, jurk + curve_step * i);
        symbol_duration = config->resolution / smooth_freq / 2;
        accel_curve->curve_table[i].level0 = 0;
        accel_curve->curve_table[i].duration0 = symbol_duration;
        accel_curve->curve_table[i].level1 = 1;
        accel_curve->curve_table[i].duration1 = symbol_duration;
    }

    // generate constsnt curve
    if (constant_curve != NULL)
    {
        free(constant_curve);
    }
    constant_curve = rmt_alloc_encoder_mem(sizeof(rmt_stepper_uniform_encoder_t));
    constant_curve->resolution = CURVE_SAPLES_COUND;

    // generate decalaration curve
    if (decel_curve != NULL)
    {
        free(decel_curve);
    }
    decel_curve = rmt_alloc_encoder_mem(sizeof(rmt_stepper_curve_encoder_t) + CURVE_SAPLES_COUND * sizeof(rmt_symbol_word_t));
    float curve_step = (jurk - maxSpeed) / (CURVE_SAPLES_COUND - 1);
    decel_curve->flags.is_accel_curve = false;
    decel_curve->sample_points = CURVE_SAPLES_COUND;
    for (uint32_t i = 0; i < CURVE_SAPLES_COUND; i++)
    {
        smooth_freq = convert_to_smooth_freq(maxSpeed, jurk, maxSpeed + curve_step * i);
        symbol_duration = config->resolution / smooth_freq / 2;
        decel_curve->curve_table[CURVE_SAPLES_COUND - i - 1].level0 = 0;
        decel_curve->curve_table[CURVE_SAPLES_COUND - i - 1].duration0 = symbol_duration;
        decel_curve->curve_table[CURVE_SAPLES_COUND - i - 1].level1 = 1;
        decel_curve->curve_table[CURVE_SAPLES_COUND - i - 1].duration1 = symbol_duration;
    }
}

void MotorDriver::SetStepsPerMM(int32_t a, int32_t b)
{
    stepsPerMMA = a;
    stepsPerMMB = b;
}

void MotorDriver::move(float x, float y, uint32_t speed)
{
    int32_t stepsA = (-y + x) * stepsPerMMA;
    int32_t stepsB = (-y - x) * stepsPerMMB;

    gpio_set_level(pinsMotorA.dir, (stepsA) ? STEPER_DIR_CW : STEPER_DIR_CCW);
    gpio_set_level(pinsMotorB.dir, (stepsB) ? STEPER_DIR_CW : STEPER_DIR_CCW);

    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };
    uint32_t curve_samples;

    // stepper A
    if (stepsA < CURVE_SAPLES_COUND * 2)
    {
        // acceleraton
        curve_samples = stepsA / 2;
        rmt_transmit(motor_chan, &accel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        curve_samples += stepsA & 0x1; // add one if odd
        rmt_transmit(motor_chan, &decel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
    }
    else
    {
        // acceleraton
        curve_samples = CURVE_SAPLES_COUND;
        rmt_transmit(motor_chan, &accel_curve->base, &CURVE_SAPLES_COUND, sizeof(CURVE_SAPLES_COUND), &tx_config);
        // constant speed
        tx_config.loop_count = stepsA - CURVE_SAPLES_COUND*2;
        rmt_transmit(motor_chan, &constant_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        tx_config.loop_count = 0;
        rmt_transmit(motor_chan, &decel_curve->base, &CURVE_SAPLES_COUND, sizeof(CURVE_SAPLES_COUND), &tx_config);
    }

    // stepper B
    if (stepsB < CURVE_SAPLES_COUND * 2)
    {
        // acceleraton
        curve_samples = stepsB / 2;
        rmt_transmit(motor_chan, &accel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        curve_samples += stepsB & 0x1; // add one if odd
        rmt_transmit(motor_chan, &decel_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
    }
    else
    {
        // acceleraton
        curve_samples = CURVE_SAPLES_COUND;
        rmt_transmit(motor_chan, &accel_curve->base, &CURVE_SAPLES_COUND, sizeof(CURVE_SAPLES_COUND), &tx_config);
        // constant speed
        tx_config.loop_count = stepsB - CURVE_SAPLES_COUND*2;
        rmt_transmit(motor_chan, &constant_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        // decelaraton
        tx_config.loop_count = 0;
        rmt_transmit(motor_chan, &decel_curve->base, &CURVE_SAPLES_COUND, sizeof(CURVE_SAPLES_COUND), &tx_config);
    }
}

void MotorDriver::home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY)
{
    rmt_transmit_config_t tx_config = {
        .loop_count = maxMove,
    };

    gpio_set_level(pinsMotorA.dir, STEPER_DIR_CCW);
    gpio_set_level(pinsMotorB.dir, STEPER_DIR_CW);

    rmt_transmit(motor_chan, &constant_curve->base, &speed, sizeof(uint32_t), &tx_config);

    //TODO: check if motors are finished
    while (true && (digitalRead(endStopXPin) == 1))
    {
        // do nothing?
    }
    //TODO: stop motors

    gpio_set_level(pinsMotorA.dir, STEPER_DIR_CW);
    gpio_set_level(pinsMotorB.dir, STEPER_DIR_CW);

    rmt_transmit(motor_chan, &constant_curve->base, &speed, sizeof(uint32_t), &tx_config);


    while (true && (digitalRead(endStopYPin) == 0))
    {
        // do nothing?
    }
    //TODO: stop motors

    // move to center of (0,0)
    move(offsetX, offsetY, jurk);
}
