#include "motorDriver.hpp"

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"

#include "logger.h"

#define CURVE_SAPLES_COUND 500
#define STEP_MOTOR_RESOLUTION_HZ 1000000
// #define STEP_MOTOR_RESOLUTION_HZ 100000

// #if MOTOR_JURK >= MOTOR_MAX_SPEED
// #error "MOTOR_JURK sould be lower than MOTOR_MAX_SPEED"
// #endif

#define MOTOR_SPEED_DIFF ((float)MOTOR_MAX_SPEED - (float)MOTOR_JURK)
#define MOTOR_SPEED_DIFF_STEPS ((float)MOTOR_SPEED_DIFF * (float)STEPS_PER_MM)
#define MOTOR_MAX_SPEED_STEPS ((float)MOTOR_MAX_SPEED * (float)STEPS_PER_MM)
#define MOTOR_JURK_STEPS ((float)MOTOR_JURK * (float)STEPS_PER_MM)
#define MOTOR_ACCELARATION_STEPS (MOTOR_ACCELARATION * STEPS_PER_MM)
#define MOTOR_HOME_SPEED_STEPS (MOTOR_HOME_SPEED * STEPS_PER_MM)
#define MOTOR_HOME_ACCELARATION_STEPS (MOTOR_HOME_ACCELARATION * STEPS_PER_MM)

#define ACCELARATION_TIME ((float)MOTOR_SPEED_DIFF / (float)MOTOR_ACCELARATION)
#define ACCELARATION_STEP_COUNT (((float)MOTOR_JURK + (float)MOTOR_SPEED_DIFF/2.0) * (float)ACCELARATION_TIME * (float)STEPS_PER_MM)

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_handle_t encoder;
    uint32_t resolution;
} rmt_stepper_uniform_encoder_t;

typedef enum {
        STEP_ENC_FAILED_RMP_ENC = -1,
        STEP_ENC_READY = 0,
        STEP_ENC_RUNNING = 1
} stepper_encoder_state_t;

static float convert_to_smooth_freq(float freq1, float freq2, float freqx)
{
    float normalize_x = ((float)(freqx - freq1)) / (freq2 - freq1);
    // third-order "smoothstep" function: https://en.wikipedia.org/wiki/Smoothstep
    float smooth_x = normalize_x * normalize_x * (3 - 2 * normalize_x);
    return smooth_x * (freq2 - freq1) + freq1;
}

esp_err_t reset_mnt(rmt_encoder_t *encoder)
{
    return rmt_encoder_reset(encoder);
}

class stepper_encoder {
public:

    stepper_encoder(bool is_accel)
    {
        int ret;
        rmt_copy_encoder_config_t encoder_config_acal = {};
        ret = rmt_new_copy_encoder(&encoder_config_acal, &this->_encoder);
        if (ret != ESP_OK)
        {
            LOG_C("failed to create copy encoder for acceleration curve");
            this->_state = STEP_ENC_FAILED_RMP_ENC;
            return;
        }
        this->_flags.is_accel_curve = is_accel;
        this->_base.del = &reset_mnt;//&this->del;
        this->_base.encode = &this->_encode;
        this->_base.reset = &reset_mnt;//&this->reset;

        // pre calculate all pulse times for the acceleration
        float curve_time = 0.0; // keep track of current time in accaleration (each step takes diffrent time, thus different step size in speed)
        for (uint32_t i = 0; i < ACCELARATION_STEP_COUNT; i++)
        {
            // calculate next speed (liniair)
            const float curve_freq = (
                (is_accel) ? (float)MOTOR_JURK_STEPS      + curve_time * (float)MOTOR_ACCELARATION_STEPS
                           : (float)MOTOR_MAX_SPEED_STEPS - curve_time * (float)MOTOR_ACCELARATION_STEPS
            );
            // convert liniair speed in a smooth curve
            const float smooth_freq = convert_to_smooth_freq(MOTOR_JURK_STEPS, MOTOR_MAX_SPEED_STEPS, curve_freq);
            // calculate pulse time
            const uint16_t symbol_duration = STEP_MOTOR_RESOLUTION_HZ / smooth_freq / 2;
            uint32_t I = (is_accel) ? i : MOTOR_ACCELARATION_STEPS - i - 1;
            this->_curve_table[I].level0 = 0;
            this->_curve_table[I].duration0 = symbol_duration;
            this->_curve_table[I].level1 = 1;
            this->_curve_table[I].duration1 = symbol_duration;
            curve_time += 1 / smooth_freq; // update the current time
        }

        this->_state = STEP_ENC_READY;
    }

    ~stepper_encoder()
    {
        this->del();
    }

    void del()
    {
        rmt_del_encoder(this->_encoder);
    }

    void reset(rmt_encoder_t *encoder)
    {
        rmt_encoder_reset(this->_encoder);
    }

    stepper_encoder_state_t getState()
    {
        return this->_state;
    }

    int transmit(rmt_channel_handle_t channel, uint32_t steps)
    {
        if (channel == NULL)
        {
            LOG_E("motordriver: transmit: channel is NULL");
            return -1;
        }
        if (steps == 0)
        {
            LOG_E("motordriver: transmit: steps is 0");
            return -2;
        }
        rmt_transmit_config_t tx_config = {
            .loop_count = 0,
            .flags = {
                .eot_level = 0,
                .queue_nonblocking = 0
            }
        };
        this->_steps = steps;
        return rmt_transmit(channel, &this->_base, this, sizeof(stepper_encoder), &tx_config);
    }

private:
    static size_t _encode(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
    {
        // stepper_encoder *motor_encoder = __containerof(encoder, stepper_encoder, base);
        rmt_encode_state_t session_state = RMT_ENCODING_RESET;
        stepper_encoder *stepper = (stepper_encoder *)primary_data;
        uint32_t step_count = stepper->_steps;
        if (step_count == 0)
        {
            LOG_E("stepper_encoder: a move of zero? what are you doing?");
            return 0;
        }
        else if (step_count > ACCELARATION_STEP_COUNT)
        {
            LOG_W("stepper_encoder: step_count is bigger than acceleration curve. You might miss %lu steps", step_count - (uint32_t)ACCELARATION_STEP_COUNT);
            step_count = ACCELARATION_STEP_COUNT;
        }

        size_t symbol_count = 0;
        if (stepper->_flags.is_accel_curve == 1)
        {
            symbol_count = stepper->_encoder->encode(
                stepper->_encoder,
                channel,
                &stepper->_curve_table[0],
                step_count * sizeof(rmt_symbol_word_t),
                &session_state
            );
        }
        else
        {
            symbol_count = stepper->_encoder->encode(
                stepper->_encoder,
                channel,
                &stepper->_curve_table[(uint32_t)ACCELARATION_STEP_COUNT - step_count],
                step_count * sizeof(rmt_symbol_word_t),
                &session_state
            );
        }

        if (symbol_count != step_count)
        {
            LOG_E("stepper_encoder: symbol_count != step_count (%u != %lu)", symbol_count, step_count);
        }

        *ret_state = session_state;
        return symbol_count;
    }

private:
    stepper_encoder_state_t _state;
    rmt_encoder_t _base;
    uint32_t _steps;
    rmt_encoder_handle_t _encoder;
    struct {
        uint32_t is_accel_curve: 1;
    } _flags;
    rmt_symbol_word_t _curve_table[(uint32_t)ACCELARATION_STEP_COUNT];
};

rmt_channel_handle_t motor_channel_a = NULL;
rmt_channel_handle_t motor_channel_b = NULL;
stepper_encoder *accel_curve = NULL;
rmt_stepper_uniform_encoder_t *constant_curve = NULL;
stepper_encoder *decel_curve = NULL;

static size_t rmt_encode_stepper_motor_uniform(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_stepper_uniform_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_uniform_encoder_t, base);
    rmt_encoder_handle_t copy_encoder = motor_encoder->encoder;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    uint32_t target_freq_hz = *(uint32_t *)primary_data;
    uint16_t symbol_duration = STEP_MOTOR_RESOLUTION_HZ / target_freq_hz / 2;
    rmt_symbol_word_t freq_sample = {
        .duration0 = symbol_duration,
        .level0 = 0,
        .duration1 = symbol_duration,
        .level1 = 1
    };
    size_t encoded_symbols = copy_encoder->encode(copy_encoder, channel, &freq_sample, sizeof(freq_sample), &session_state);
    if (encoded_symbols != 1)
    {
        LOG_E("uniform: symbol_count != 1 (%u != %u)", encoded_symbols, 1);
    }
    *ret_state = session_state;
    return encoded_symbols;
}

static esp_err_t rmt_del_stepper_motor_uniform_encoder(rmt_encoder_t *encoder)
{
    rmt_stepper_uniform_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_uniform_encoder_t, base);
    rmt_del_encoder(motor_encoder->encoder);
    free(motor_encoder);
    return ESP_OK;
}

static esp_err_t rmt_reset_stepper_motor_uniform(rmt_encoder_t *encoder)
{
    rmt_stepper_uniform_encoder_t *motor_encoder = __containerof(encoder, rmt_stepper_uniform_encoder_t, base);
    rmt_encoder_reset(motor_encoder->encoder);
    return ESP_OK;
}

MotorDriver::MotorDriver()
{}

int MotorDriver::init()
{
    int ret;

    // === init GPIO

    LOG_D("Initialize GPIO");
    gpio_config_t dir_gpio_config = {
        .pin_bit_mask = 1ULL << MOTOR_A_DIR_PIN | 1ULL << MOTOR_B_DIR_PIN,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret = gpio_config(&dir_gpio_config);
    if (ret != 0)
    {
        LOG_C("faild to set dir pins as output");
        return -1;
    }
    gpio_config_t end_stop_gpio_config = {
        .pin_bit_mask = 1ULL << LIMIT_X_PIN | 1ULL << LIMIT_Y_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ret = gpio_config(&end_stop_gpio_config);
    if (ret != 0)
    {
        LOG_C("faild to set end stop pins as input");
        return -2;
    }

    LOG_D("Set spin direction");
    gpio_set_level(MOTOR_A_DIR_PIN, STEPER_DIR_CW);
    gpio_set_level(MOTOR_B_DIR_PIN, STEPER_DIR_CW);

    // === init RMT channels

    LOG_D("Create RMT TX channels");
    rmt_tx_channel_config_t tx_chan_config_a = {
        .gpio_num = MOTOR_A_STEP_PIN,
        .clk_src = RMT_CLK_SRC_DEFAULT, // select clock source
        .resolution_hz = STEP_MOTOR_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .trans_queue_depth = 10, // set the number of transactions that can be pending in the background
        .flags = {
            .invert_out = 0,   /*!< Whether to invert the RMT channel signal before output to GPIO pad */
            .with_dma = 1,     /*!< If set, the driver will allocate an RMT channel with DMA capability */
            .io_loop_back = 0, /*!< The signal output from the GPIO will be fed to the input path as well */
            .io_od_mode = 0    /*!< Configure the GPIO as open-drain mode */
        }
    };
    ret = rmt_new_tx_channel(&tx_chan_config_a, &motor_channel_a);
    if (ret != 0)
    {
        LOG_C("faild to create rmt channel for motor a");
        return -3;
    }
    rmt_tx_channel_config_t tx_chan_config_b = {
        .gpio_num = MOTOR_B_STEP_PIN,
        .clk_src = RMT_CLK_SRC_DEFAULT, // select clock source
        .resolution_hz = STEP_MOTOR_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .trans_queue_depth = 10, // set the number of transactions that can be pending in the background
        .flags = {
            .invert_out = 0,   /*!< Whether to invert the RMT channel signal before output to GPIO pad */
            .with_dma = 0,     /*!< If set, the driver will allocate an RMT channel with DMA capability */
            .io_loop_back = 0, /*!< The signal output from the GPIO will be fed to the input path as well */
            .io_od_mode = 0    /*!< Configure the GPIO as open-drain mode */
        }
    };
    ret = rmt_new_tx_channel(&tx_chan_config_b, &motor_channel_b);
    if (ret != 0)
    {
        LOG_C("faild to create rmt channel for motor b");
        return -4;
    }

    // === init motor encoders

    LOG_D("create motor encoder curves");

    LOG_D("ACCELARATION_STEP_COUNT: %lu", (uint32_t)ACCELARATION_STEP_COUNT);
    LOG_D("MOTOR_MAX_SPEED: %f", (float)MOTOR_MAX_SPEED);
    LOG_D("MOTOR_JURK: %f", (float)MOTOR_JURK);
    LOG_D("MOTOR_SPEED_DIFF: %f", (float)MOTOR_SPEED_DIFF);
    LOG_D("ACCELARATION_TIME: %f", (float)ACCELARATION_TIME);
    LOG_D("STEPS_PER_MM: %f", (float)STEPS_PER_MM);
    LOG_D("MOTOR_ACCELARATION: %f", (float)MOTOR_ACCELARATION);

    // generate accelaration curve
    if (accel_curve != NULL)
    {
        LOG_W("accel_curve already initilized. removeing old curve");
        delete(accel_curve);
    }
    accel_curve = new stepper_encoder(true);

    // generate constant curve
    if (constant_curve != NULL)
    {
        LOG_W("constant_curve already exsisted. removeing old curve");
        free(constant_curve);
    }
    constant_curve = (rmt_stepper_uniform_encoder_t*)rmt_alloc_encoder_mem(sizeof(rmt_stepper_uniform_encoder_t));
    if (constant_curve == NULL)
    {
        delete(accel_curve);
        LOG_C("faild to allocate memory for constant curve");
        return -5;
    }
    rmt_copy_encoder_config_t encoder_config_const = {};
    ret = rmt_new_copy_encoder(&encoder_config_const, &constant_curve->encoder);
    if (ret != ESP_OK)
    {
        delete(accel_curve);
        LOG_C("failed to create copy encoder for constant curve");
        return -6;
    }
    constant_curve->resolution = STEP_MOTOR_RESOLUTION_HZ;
    constant_curve->base.del = &rmt_del_stepper_motor_uniform_encoder;
    constant_curve->base.encode = &rmt_encode_stepper_motor_uniform;
    constant_curve->base.reset = &rmt_reset_stepper_motor_uniform;

    // generate decalaration curve
    if (decel_curve != NULL)
    {
        LOG_W("decel_curve already initilized. removeing old curve");
        delete(decel_curve);
    }
    decel_curve = new stepper_encoder(true);

    // === enable rmt channels

    ret = rmt_enable(motor_channel_a);
    if (ret != 0)
    {
        delete(accel_curve);
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
        delete(decel_curve);
        LOG_C("failed to enable rmt channel a");
        return -7;
    }

    ret = rmt_enable(motor_channel_b);
    if (ret != 0)
    {
        delete(accel_curve);
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
        delete(decel_curve);
        LOG_C("failed to enable rmt channel b");
        return -8;
    }
    return 0;
}

MotorDriver::~MotorDriver()
{
    if (accel_curve != NULL)
    {
        delete(accel_curve);
    }
    if (constant_curve != NULL)
    {
        rmt_del_stepper_motor_uniform_encoder(&constant_curve->base);
    }
    if (decel_curve != NULL)
    {
        delete(decel_curve);
    }
}

void MotorDriver::move(float x, float y, uint32_t speed)
{
    LOG_D("move: mm (%f, %f), speed=%lu", x, y, speed);
    int32_t stepsA = (-y + x) * STEPS_PER_MM;
    int32_t stepsB = (-y - x) * STEPS_PER_MM;

    if (stepsA > 0)
    {
        gpio_set_level(MOTOR_A_DIR_PIN, STEPER_DIR_CW);
    }
    else
    {
        stepsA = -stepsA;
        gpio_set_level(MOTOR_A_DIR_PIN, STEPER_DIR_CCW);
    }
    
    if (stepsB > 0)
    {
        gpio_set_level(MOTOR_B_DIR_PIN, STEPER_DIR_CW);
    }
    else
    {
        stepsB = -stepsB;
        gpio_set_level(MOTOR_B_DIR_PIN, STEPER_DIR_CCW);
    }
    LOG_D("move: steps A=%li, B=%li", stepsA, stepsB);

    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
        .flags = {
            .eot_level = 0,
            .queue_nonblocking = 0
        }
    };
    uint32_t curve_samples;
    int ret;

    // stepper A
    if (stepsA < (uint32_t)ACCELARATION_STEP_COUNT * 2)
    {
        LOG_D("move a: less steps than accel + decel. do parcel accel and decel");

        // acceleraton
        curve_samples = stepsA / 2;
        ret = accel_curve->transmit(motor_channel_a, curve_samples);
        if (ret != 0)
        {
            LOG_E("move a: faild to accelerate motor");
        }
        // decelaraton
        curve_samples += stepsA & 0x1; // add one if odd
        ret = decel_curve->transmit(motor_channel_a, curve_samples);
        if (ret != 0)
        {
            LOG_E("move a: faild to decelerate motor");
        }
    }
    else
    {
        LOG_D("move a: full curve");

        // acceleraton
        ret = accel_curve->transmit(motor_channel_a, (uint32_t)ACCELARATION_STEP_COUNT);
        if (ret != 0)
        {
            LOG_E("move a: faild to accelerate motor");
        }
        // LOG_D("move a: full curve 2");
        // constant speed
        tx_config.loop_count = stepsA - (uint32_t)ACCELARATION_STEP_COUNT*2;
        ret = rmt_transmit(motor_channel_a, &constant_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        if (ret != 0)
        {
            LOG_E("move a: faild to move motor constant");
        }
        // LOG_D("move a: full curve 3");
        // decelaraton
        tx_config.loop_count = 0;
        ret = decel_curve->transmit(motor_channel_a, (uint32_t)ACCELARATION_STEP_COUNT);
        if (ret != 0)
        {
            LOG_E("move a: faild to decelerate motor");
        }
    }

    tx_config.loop_count = 0;
    // stepper B
    if (stepsB < (uint32_t)ACCELARATION_STEP_COUNT * 2)
    {
        // acceleraton
        curve_samples = stepsA / 2;
        ret = accel_curve->transmit(motor_channel_b, curve_samples);
        if (ret != 0)
        {
            LOG_E("move b: faild to accelerate motor");
        }
        // decelaraton
        curve_samples += stepsA & 0x1; // add one if odd
        ret = decel_curve->transmit(motor_channel_b, curve_samples);
        if (ret != 0)
        {
            LOG_E("move b: faild to decelerate motor");
        }
    }
    else
    {
        // acceleraton
        ret = accel_curve->transmit(motor_channel_b, (uint32_t)ACCELARATION_STEP_COUNT);
        if (ret != 0)
        {
            LOG_E("move b: faild to accelerate motor");
        }
        // constant speed
        tx_config.loop_count = stepsB - (uint32_t)ACCELARATION_STEP_COUNT*2;
        ret = rmt_transmit(motor_channel_b, &constant_curve->base, &curve_samples, sizeof(curve_samples), &tx_config);
        if (ret != 0)
        {
            LOG_E("move b: faild to move motor constant");
        }
        // decelaraton
        ret = decel_curve->transmit(motor_channel_b, (uint32_t)ACCELARATION_STEP_COUNT);
        if (ret != 0)
        {
            LOG_E("move b: faild to decelerate motor");
        }
    }

    rmt_tx_wait_all_done(motor_channel_a, 10000);
    rmt_tx_wait_all_done(motor_channel_b, 10000);
}

void MotorDriver::home(int32_t maxMove, uint32_t speed, float offsetX, float offsetY)
{
    rmt_transmit_config_t tx_config = {
        .loop_count = maxMove,
        .flags = {
            .eot_level = 0,
            .queue_nonblocking = 0
        }
    };

    gpio_set_level(stepperAPins.dir, STEPER_DIR_CCW);
    gpio_set_level(stepperBPins.dir, STEPER_DIR_CW);

    rmt_transmit(motor_channel_a, &constant_curve->base, &speed, sizeof(uint32_t), &tx_config);

    //TODO: check if motors are finished
    while (true && (gpio_get_level(endStopXPin) == 1))
    {
        // do nothing?
    }
    //TODO: stop motors

    gpio_set_level(stepperAPins.dir, STEPER_DIR_CW);
    gpio_set_level(stepperBPins.dir, STEPER_DIR_CW);

    rmt_transmit(motor_channel_a, &constant_curve->base, &speed, sizeof(uint32_t), &tx_config);

    while (true && (gpio_get_level(endStopYPin) == 0))
    {
        // do nothing?
    }
    //TODO: stop motors

    // move to center of (0,0)
    move(offsetX, offsetY, jurk);
}
