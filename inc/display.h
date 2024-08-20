#ifndef DISPLAY_H
#define DISPLAY_H

#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <stdint.h>

#define WIDTH 200
#define HEIGHT 200

#define RESET_PIN GPIO_NUM_0
#define BUSY_PIN  GPIO_NUM_4
#define DC_PIN    GPIO_NUM_3
#define CS_PIN    GPIO_NUM_10
#define MISO_PIN  GPIO_NUM_2
#define MOSI_PIN  GPIO_NUM_7
#define SCLK_PIN  GPIO_NUM_6

#define SPI_CLOCK_SPEED 1000000
#define SPI_QUEUE_SIZE 8

esp_err_t InitDisplay();
void WriteCommand(spi_device_handle_t handle, uint8_t command);
void WriteData(spi_device_handle_t handle, uint8_t *data, uint8_t length);

#endif // DISPLAY_H
