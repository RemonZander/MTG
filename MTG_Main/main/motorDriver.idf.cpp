#include "motorDriver.hpp"

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "esp_log.h"

typedef struct {
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

rmt_stepper_curve_encoder_t *accel_curve = NULL;
rmt_stepper_curve_encoder_t *decel_curve = NULL;

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
    rmt_channel_handle_t motor_chan = NULL;
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
    // generate accelaration curve
    if (accel_curve != NULL)
    {
        free(accel_curve);
    }
    accel_curve = rmt_alloc_encoder_mem(sizeof(rmt_stepper_curve_encoder_t) + config->sample_points * sizeof(rmt_symbol_word_t));
    float curve_step = (maxSpeed - jurk) / (config->sample_points - 1);
    accel_curve->sample_points = config->sample_points;
    accel_curve->flags.is_accel_curve = true;
    for (uint32_t i = 0; i < config->sample_points; i++)
    {
        smooth_freq = convert_to_smooth_freq(jurk, maxSpeed, jurk + curve_step * i);
        symbol_duration = config->resolution / smooth_freq / 2;
        accel_curve->curve_table[i].level0 = 0;
        accel_curve->curve_table[i].duration0 = symbol_duration;
        accel_curve->curve_table[i].level1 = 1;
        accel_curve->curve_table[i].duration1 = symbol_duration;
    }

    // generate decalaration curve
    if (decel_curve != NULL)
    {
        free(decel_curve);
    }
    decel_curve = rmt_alloc_encoder_mem(sizeof(rmt_stepper_curve_encoder_t) + config->sample_points * sizeof(rmt_symbol_word_t));
    float curve_step = (jurk - maxSpeed) / (config->sample_points - 1);
    decel_curve->flags.is_accel_curve = false;
    decel_curve->sample_points = config->sample_points;
    for (uint32_t i = 0; i < config->sample_points; i++)
    {
        smooth_freq = convert_to_smooth_freq(maxSpeed, jurk, maxSpeed + curve_step * i);
        symbol_duration = config->resolution / smooth_freq / 2;
        decel_curve->curve_table[config->sample_points - i - 1].level0 = 0;
        decel_curve->curve_table[config->sample_points - i - 1].duration0 = symbol_duration;
        decel_curve->curve_table[config->sample_points - i - 1].level1 = 1;
        decel_curve->curve_table[config->sample_points - i - 1].duration1 = symbol_duration;
    }
}

void MotorDriver::SetStepsPerMM(int32_t a, int32_t b)
{
    stepsPerMMA = a;
    stepsPerMMB = b;
}

void MotorDriver::move(float x, float y, uint32_t speed)
{
    int32_t stepsA = -y + x;
    int32_t stepsB = -y - x;
	stepperMotorA->move(stepsA * stepsPerMMA);
	stepperMotorB->move(stepsB * stepsPerMMB);

    bool motorAFinished = false, motorBFinished = false;

    while (!motorAFinished || !motorBFinished)
    {
        stepperMotorA->run();
        stepperMotorB->run();
        motorAFinished = stepperMotorA->distanceToGo() == 0;
        motorBFinished = stepperMotorB->distanceToGo() == 0;
    }
}

void MotorDriver::home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY)
{
	stepperMotorA->move(-maxMove);
	stepperMotorB->move(maxMove);

    bool motorAFinished = false, motorBFinished = false;

    while (!motorAFinished && !motorBFinished && (digitalRead(endStopXPin) == 1))
    {
        stepperMotorA->run();
        stepperMotorB->run();
        motorAFinished = stepperMotorA->distanceToGo() == 0;
        motorBFinished = stepperMotorB->distanceToGo() == 0;
    }
    stepperMotorA->stop();
    stepperMotorB->stop();

	stepperMotorA->move(maxMove);
	stepperMotorB->move(maxMove);

    motorAFinished = false;
    motorBFinished = false;

    while (!motorAFinished && !motorBFinished && (digitalRead(endStopYPin) == 0))
    {
        stepperMotorA->run();
        stepperMotorB->run();
        motorAFinished = stepperMotorA->distanceToGo() == 0;
        motorBFinished = stepperMotorB->distanceToGo() == 0;
    }
    stepperMotorA->stop();
    stepperMotorB->stop();

	float deltaA = -offsetY + offsetX;
	float deltaB = -offsetY - offsetX;
	stepperMotorA->move((long) deltaA * 10);
	stepperMotorB->move((long) deltaB * 10);

    motorAFinished = false;
    motorBFinished = false;

    while (!motorAFinished || !motorBFinished)
    {
        stepperMotorA->run();
        stepperMotorB->run();
        motorAFinished = stepperMotorA->distanceToGo() == 0;
        motorBFinished = stepperMotorB->distanceToGo() == 0;
    }
    stepperMotorA->stop();
    stepperMotorB->stop();
}


/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */


///////////////////////////////Change the following configurations according to your board//////////////////////////////
#define STEP_MOTOR_GPIO_EN       0
#define STEP_MOTOR_GPIO_DIR      2
#define STEP_MOTOR_GPIO_STEP     4
#define STEP_MOTOR_ENABLE_LEVEL  0 // DRV8825 is enabled on low level
#define STEP_MOTOR_SPIN_DIR_CLOCKWISE 0
#define STEP_MOTOR_SPIN_DIR_COUNTERCLOCKWISE !STEP_MOTOR_SPIN_DIR_CLOCKWISE

#define STEP_MOTOR_RESOLUTION_HZ 1000000 // 1MHz resolution

static const char *TAG = "example";

void app_main(void)
{
    ESP_LOGI(TAG, "Create motor encoders");
    stepper_motor_curve_encoder_config_t accel_encoder_config = {
        .resolution = STEP_MOTOR_RESOLUTION_HZ,
        .sample_points = 500,
        .start_freq_hz = 500,
        .end_freq_hz = 1500,
    };
    rmt_encoder_handle_t accel_motor_encoder = NULL;
    ESP_ERROR_CHECK(rmt_new_stepper_motor_curve_encoder(&accel_encoder_config, &accel_motor_encoder));

    stepper_motor_uniform_encoder_config_t uniform_encoder_config = {
        .resolution = STEP_MOTOR_RESOLUTION_HZ,
    };
    rmt_encoder_handle_t uniform_motor_encoder = NULL;
    ESP_ERROR_CHECK(rmt_new_stepper_motor_uniform_encoder(&uniform_encoder_config, &uniform_motor_encoder));

    stepper_motor_curve_encoder_config_t decel_encoder_config = {
        .resolution = STEP_MOTOR_RESOLUTION_HZ,
        .sample_points = 500,
        .start_freq_hz = 1500,
        .end_freq_hz = 500,
    };
    rmt_encoder_handle_t decel_motor_encoder = NULL;
    ESP_ERROR_CHECK(rmt_new_stepper_motor_curve_encoder(&decel_encoder_config, &decel_motor_encoder));

    ESP_LOGI(TAG, "Enable RMT channel");
    ESP_ERROR_CHECK(rmt_enable(motor_chan));

    ESP_LOGI(TAG, "Spin motor for 6000 steps: 500 accel + 5000 uniform + 500 decel");
    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };

    const static uint32_t accel_samples = 500;
    const static uint32_t uniform_speed_hz = 1500;
    const static uint32_t decel_samples = 500;

    while (1) {
        // acceleration phase
        tx_config.loop_count = 0;
        ESP_ERROR_CHECK(rmt_transmit(motor_chan, accel_motor_encoder, &accel_samples, sizeof(accel_samples), &tx_config));

        // uniform phase
        tx_config.loop_count = 5000;
        ESP_ERROR_CHECK(rmt_transmit(motor_chan, uniform_motor_encoder, &uniform_speed_hz, sizeof(uniform_speed_hz), &tx_config));

        // deceleration phase
        tx_config.loop_count = 0;
        ESP_ERROR_CHECK(rmt_transmit(motor_chan, decel_motor_encoder, &decel_samples, sizeof(decel_samples), &tx_config));
        // wait all transactions finished
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(motor_chan, -1));

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
