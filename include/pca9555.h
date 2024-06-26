#pragma once

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#define MAX_DEVICES 3

#define REG_INPUT_0    0
#define REG_INPUT_1    1
#define REG_OUTPUT_0   2
#define REG_OUTPUT_1   3
#define REG_POLARITY_0 4
#define REG_POLARITY_1 5
#define REG_CONFIG_0   6
#define REG_CONFIG_1   7

#define PCA_OUTPUT   0
#define PCA_INPUT    1
#define PCA_NORMAL   0
#define PCA_INVERTED 0

typedef void (*intr_t)(uint8_t, bool);  // Interrupt handler type

typedef struct PCA9555 {
    // Pins
    int     i2c_bus;
    uint8_t i2c_addr;
    // Internal state
    uint8_t  reg_config[2];
    uint8_t  reg_polarity[2];
    uint8_t  reg_output[2];
    uint16_t previous_state;
    // Mutex
    SemaphoreHandle_t i2c_semaphore;
} PCA9555;

/**
 * Initialize internal structures for the PCA9555.
 * @note This should be called before using any other methods.
 * @return ESP_OK on success; any other value indicates an error
 */
extern esp_err_t pca9555_init(PCA9555* device);

/**
 * Configure the direction of a pin
 * @param pin the pin-number (0-15)
 * @param direction 1 is input, 0 is output
 * @return 0 on success; -1 on error
 */
extern esp_err_t pca9555_set_gpio_direction(PCA9555* device, int pin, bool direction);

/**
 * Retrieve the current direction of a pin
 * @param pin the pin-number (0-15)
 * @return 1 when input; 0 when output; -1 on error
 */
extern int pca9555_get_gpio_direction(PCA9555* device, int pin);

/**
 * Configure the polarity of a pin
 * @param pin the pin-number (0-15)
 * @param direction 0 is normal, 1 is inverted
 * @return 0 on success; -1 on error
 */
extern esp_err_t pca9555_set_gpio_polarity(PCA9555* device, int pin, bool polarity);

/**
 * Retrieve the current polarity of a pin
 * @param pin the pin-number (0-15)
 * @return 0 when normal; 1 when inverted; -1 on error
 */
extern int pca9555_get_gpio_polarity(PCA9555* device, int pin);

/**
 * Set the state of a GPIO pin
 * @param pin the pin-number
 * @param value 0 is low; 1 is high
 * @return 0 on success; -1 on error
 */
extern esp_err_t pca9555_set_gpio_value(PCA9555* device, int pin, bool value);

/**
 * Retrieve the current state of a GPIO pin
 * @param pin the pin-number
 * @return 0 when low; 1 when high; -1 on error
 */
extern int pca9555_get_gpio_value(PCA9555* device, int pin);

/**
 * Retrieve all pin levels of the PCA9555
 * @param device The device
 * @param output The sotrage destination
 * @return 0 on success; -1 on error
 */
extern int pca9555_get_gpio_values(PCA9555* device, uint16_t *output);

__END_DECLS
