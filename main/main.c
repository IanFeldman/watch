#include "../inc/display.h"
#include <stdio.h>
#include <esp_task_wdt.h>
#include <driver/spi_common.h>

void app_main(void)
{
    spi_device_handle_t spi_handle;
    esp_err_t err = display_initialize(&spi_handle);
    if (err != ESP_OK)
    {
        printf("Initialization error: %04x\n", err);
    }
}
