#include "display.h"
#include <driver/gpio.h>

esp_err_t InitDisplay() {
    printf("Initializing pins\n");
    // init dc pin
    esp_rom_gpio_pad_select_gpio(DC_PIN);
    gpio_set_direction(DC_PIN, GPIO_MODE_OUTPUT);

    // init reset pin
    esp_rom_gpio_pad_select_gpio(RESET_PIN);
    gpio_set_direction(RESET_PIN, GPIO_MODE_OUTPUT);

    // init busy pin
    esp_rom_gpio_pad_select_gpio(BUSY_PIN);
    gpio_set_direction(BUSY_PIN, GPIO_MODE_INPUT);

    // spi config
    spi_bus_config_t buscfg = {
        .miso_io_num = MISO_PIN,   // MISO
        .mosi_io_num = MOSI_PIN,   // MOSI
        .sclk_io_num = SCLK_PIN,   // SCK
        .quadwp_io_num = -1,       // Unused
        .quadhd_io_num = -1,       // Unused
    };

    // initialize the SPI bus
    esp_err_t err = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        printf("SPI bus initialization failed\n");
        return err;
    }
    
    // Device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_CLOCK_SPEED,
        .mode = 0,
        .spics_io_num = CS_PIN,
        .queue_size = SPI_QUEUE_SIZE,
    };

    // Register device connected to the bus
    spi_device_handle_t handle;
    err = spi_bus_add_device(SPI2_HOST, &devcfg, &handle);
    if (err != ESP_OK) {
        printf("SPI bus addition failed\n");
        return err;
    }

    printf("running\n");
    // 1.
    // hw reset (pin active low) 
    gpio_set_level(RESET_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(RESET_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    // 2. Set Initial Config
    // sw reset
    WriteCommand(handle, 0x12);
    vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms

    // 3. Send Initialization Code
    WriteCommand(handle, 0x01);                             // set gate driver output
    WriteData(handle, (uint8_t[]){0xc7, 0x00, 0x00}, 3);    // 200x200 res
    WriteCommand(handle, 0x11);                             // def data entry sequence
    WriteData(handle, (uint8_t[]){0x03}, 1);                // x increase, y increase : normal mode
    WriteCommand(handle, 0x44);                             // set ram x range
    WriteData(handle, (uint8_t[]){0x00, (WIDTH - 1) / 8}, 2);
    WriteCommand(handle, 0x45);                             // set ram y range
    WriteData(handle, (uint8_t[]){0x00, 0x00, (HEIGHT - 1), 0x00}, 4);
    WriteCommand(handle, 0x3C);                             // set panel border
    WriteData(handle, (uint8_t[]){0x05}, 1);                // 0x02 for dark

    // 4. Load Waveform LUT
    WriteCommand(handle, 0x18);                             // sense temp ext
    WriteData(handle, (uint8_t[]){0x48}, 1);
    WriteCommand(handle, 0x22);                             // display update ctrl2
    WriteData(handle, (uint8_t[]){0xff}, 1);
    WriteCommand(handle, 0x20);                             // activate display update seq
    uint8_t busy = 1;
    while (busy) {                                          // wait busy low
        busy = gpio_get_level(BUSY_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // 5. Write image and drive display
    WriteCommand(handle, 0x4e);                             // set x address counter
    WriteData(handle, (uint8_t[]){0x00}, 1);
    WriteCommand(handle, 0x4f);                             // set y address counter
    WriteData(handle, (uint8_t[]){0x00}, 1);
    WriteCommand(handle, 0x24);                             // write RAM
    // generate small test pattern
    uint8_t ram[200];
    for (uint16_t i = 0; i < 200; i++) {
        ram[i] = 0xaa;
    }
    WriteData(handle, ram, 200);
    // WriteCommand(handle, 0x0c);                          // set softstart - not messing with this one
    WriteCommand(handle, 0x22);                             // display update ctrl2
    WriteData(handle, (uint8_t[]){0xff}, 1);
    WriteCommand(handle, 0x20);                             // activate display update seq
    busy = 1;
    while (busy) { 
        busy = gpio_get_level(BUSY_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    return ESP_OK;
}

void WriteCommand(spi_device_handle_t handle, uint8_t command) {
    // Set D/C~ low for command mode
    gpio_set_level(DC_PIN, 0);
    // setup transaction
    spi_transaction_t transaction = {
        .length = 8,
        .tx_buffer = &command
    };
    esp_err_t err = spi_device_transmit(handle, &transaction);
    if (err != ESP_OK) {
        printf("SPI transmission failed\n");
    }
}

void WriteData(spi_device_handle_t handle, uint8_t *data, uint8_t length) {
    // Set D/C~ high for data mode
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t transaction = {
        .length = 8 * length,
        .tx_buffer = data
    };
    esp_err_t err = spi_device_transmit(handle, &transaction);
    if (err != ESP_OK) {
        printf("SPI transmission failed\n");
    }
}
