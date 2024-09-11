#include "../inc/display.h"
#include "../inc/font.h"
#include <driver/spi_common.h>
#include <esp_task_wdt.h>
#include <stdio.h>
#include <esp_log.h>

void app_main(void)
{
    spi_device_handle_t spi_handle;
    esp_err_t err = display_initialize(&spi_handle);
    if (err != ESP_OK)
    {
        printf("Initialization error: %04x\n", err);
    }

    display_write_to_ram(spi_handle, characters_6x8['H'], 10, 10, 4);
    display_write_to_ram(spi_handle, characters_6x8['E'], 50, 10, 4);
    display_write_to_ram(spi_handle, characters_6x8['L'], 90, 10, 4);
    display_write_to_ram(spi_handle, characters_6x8['L'], 130, 10, 4);
    display_write_to_ram(spi_handle, characters_6x8['O'], 170, 10, 4);

    display_write_to_ram(spi_handle, characters_6x8['W'], 10, 50, 4);
    display_write_to_ram(spi_handle, characters_6x8['O'], 50, 50, 4);
    display_write_to_ram(spi_handle, characters_6x8['R'], 90, 50, 4);
    display_write_to_ram(spi_handle, characters_6x8['L'], 130, 50, 4);
    display_write_to_ram(spi_handle, characters_6x8['D'], 170, 50, 4);

    display_update(spi_handle);
}
