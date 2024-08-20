#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include <stdio.h>
#include <esp_task_wdt.h>

#define WIDTH 200
#define HEIGHT 200
#define DC_PIN GPIO_NUM_3
#define BUSY_PIN GPIO_NUM_4
#define CS_PIN GPIO_NUM_10
#define RESET_PIN GPIO_NUM_0

void send_command(spi_device_handle_t handle, uint8_t cmd) {
    printf("setting dc pin low\n");
    gpio_set_level(DC_PIN, 0); // Set D/C low for command mode
    spi_transaction_t trans = {
        .length = 8,
        .tx_buffer = &cmd
    };
    printf("sending command\n");
    esp_err_t ret = spi_device_transmit(handle, &trans);
    if (ret != ESP_OK) {
        printf("SPI transmission failed\n");
    }
    printf("done\n");
}

void send_data(spi_device_handle_t handle, uint8_t data) {
    printf("setting dc pin high\n");
    gpio_set_level(DC_PIN, 1); // Set D/C high for data mode
    spi_transaction_t trans = {
        .length = 8,
        .tx_buffer = &data
    };
    printf("sending data\n");
    esp_err_t ret = spi_device_transmit(handle, &trans);
    if (ret != ESP_OK) {
        printf("SPI transmission failed\n");
    }
    printf("done\n");
}

void app_main(void)
{   
    esp_task_wdt_deinit();
    printf("init pins\n");

    // init dc pin
    esp_rom_gpio_pad_select_gpio(DC_PIN);
    gpio_set_direction(DC_PIN, GPIO_MODE_OUTPUT);

    // init cs pin
    esp_rom_gpio_pad_select_gpio(CS_PIN);
    gpio_set_direction(CS_PIN, GPIO_MODE_OUTPUT);

    // init reset pin
    esp_rom_gpio_pad_select_gpio(RESET_PIN);
    gpio_set_direction(RESET_PIN, GPIO_MODE_OUTPUT);

    // init busy pin
    esp_rom_gpio_pad_select_gpio(BUSY_PIN);
    gpio_set_direction(BUSY_PIN, GPIO_MODE_INPUT);

    // SPI configuration
    spi_bus_config_t buscfg = {
        .miso_io_num = GPIO_NUM_2,   // MISO
        .mosi_io_num = GPIO_NUM_7,   // MOSI
        .sclk_io_num = GPIO_NUM_6,   // SCK
        .quadwp_io_num = -1,         // Unused
        .quadhd_io_num = -1,         // Unused
    };

    printf("configure spi\n");

    // Initialize the SPI bus
    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        printf("SPI bus initialization failed\n");
        return;
    }
    
    // Device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 8,
    };

    // Register device connected to the bus
    spi_device_handle_t handle;
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &handle);
    if (ret != ESP_OK) {
        printf("SPI bus addition failed\n");
        return;
    }


    printf("1\n");
    // hw reset - active low
    gpio_set_level(RESET_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(RESET_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

    // set cs low for transmission
    gpio_set_level(CS_PIN, 0);


    printf("2\n");
    // 2. Set Initial Config
    // sw reset
    send_command(handle, 0x12);
    vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms

    printf("3\n");
    // 3. Send Initialization Code
    send_command(handle, 0x01);     // set gate driver output
    send_data(handle, 0xc7);        // 200x200 res
    send_data(handle, 0x00);
    send_data(handle, 0x00); 
    send_command(handle, 0x11);     // def data entry sequence
    send_data(handle, 0x03);        // x increase, y increase : normal mode
    send_command(handle, 0x44);     // set ram x range
    send_data(handle, 0x00); 
    send_data(handle, (WIDTH - 1) / 8);
    send_command(handle, 0x45);     // set ram y range
    send_data(handle, 0x00); 
    send_data(handle, 0x00); 
    send_data(handle, HEIGHT - 1);
    send_data(handle, 0x00); 
    send_command(handle, 0x3C);     // set panel border
    send_data(handle, 0x05);        // copying watchy, 0x02 for dark

    printf("4\n");
    // 4. Load Waveform LUT
    send_command(handle, 0x18);     // sense temp ext
    send_data(handle, 0x48);
    send_command(handle, 0x22);     // display update ctrl2
    send_data(handle, 0xff);        // unsure but praying
    send_command(handle, 0x20);     // activate display update seq
    uint8_t busy = 1;
    while (busy) {                  // wait busy low
        printf("status: busy high\n");
        busy = gpio_get_level(BUSY_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    printf("status: busy low\n");

    printf("5\n");
    // 5. Write image and drive display
    send_command(handle, 0x4e);     // set x address counter
    send_data(handle, 0x00);
    send_command(handle, 0x4f);     // set y address counter
    send_data(handle, 0x00);
    send_command(handle, 0x24);     // write RAM
    for (uint16_t i = 0; i < 25; i++) {
        send_data(handle, 0xFF);
    }
    for (uint16_t i = 0; i < 25; i++) {
        send_data(handle, 0x00);
    }
    for (uint16_t i = 0; i < 25; i++) {
        send_data(handle, 0xFF);
    }
    for (uint16_t i = 0; i < 25; i++) {
        send_data(handle, 0x00);
    }
    for (uint16_t i = 0; i < 25; i++) {
        send_data(handle, 0xFF);
    }
    for (uint16_t i = 0; i < 12; i++) {
        send_data(handle, 0x00);
    }
    // send_command(handle, 0x0c);     // set softstart 
    // not messing with defaults
    send_command(handle, 0x22);     // display update ctrl2
    send_data(handle, 0xff);        // unsure but praying
    send_command(handle, 0x20);     // activate display update seq
    vTaskDelay(pdMS_TO_TICKS(1000));
    busy = 1;
    while (busy) {                  // wait busy low
        printf("status: busy high\n");
        busy = gpio_get_level(BUSY_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    printf("status: busy low\n");

    gpio_set_level(CS_PIN, 1); // set cs high

    printf("done\n");
}