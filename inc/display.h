#ifndef DISPLAY_H
#define DISPLAY_H

#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <stdint.h>

#define SPI_CLOCK_SPEED 1000000
#define SPI_MODE 0
#define SPI_QUEUE_SIZE 8

#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

#define RESET_PIN GPIO_NUM_6
#define BUSY_PIN  GPIO_NUM_7
#define DC_PIN    GPIO_NUM_4
#define CS_PIN    GPIO_NUM_0
#define MOSI_PIN  GPIO_NUM_10
#define SCLK_PIN  GPIO_NUM_15

#ifdef DEV_BUILD
#undef SCLK_PIN
#define SCLK_PIN GPIO_NUM_18
#endif // DEV_BUILD

#define RESET_DELAY_MS 10
#define AWAIT_BUSY_DELAY_MS 250

#define BITS_PER_BYTE 8

typedef struct {
    uint8_t x;              // pixel x position
    uint8_t y;              // pixel y position
    uint8_t w;              // pixel width
    uint8_t h;              // pixel height
    const uint8_t *p_data;  // image data, one bit per pixel
    const uint16_t size;
} image_t;

esp_err_t display_initialize(spi_device_handle_t *p_spi_handle);
void display_clear(spi_device_handle_t spi_handle);

#endif // DISPLAY_H
