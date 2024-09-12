#include "display.h"
#include "font.h"
#include "util.h"
#include <driver/gpio.h>
#include <string.h>

// static functions
static esp_err_t display_initialize_gpio(void);
static esp_err_t display_initialize_spi(spi_device_handle_t *p_spi_handle);
static void display_reset(spi_device_handle_t spi_handle);
static void display_configure_internals(spi_device_handle_t spi_handle);
static void display_set_update_sequence(spi_device_handle_t spi_handle);
static void display_write_command(spi_device_handle_t spi_handle, uint8_t command);
static void display_write_data(spi_device_handle_t spi_handle, uint8_t *p_data, uint8_t length);

/*!
 * @brief Initialize D/C#, reset, and busy gpio pins.
 * @return Esp error code.
 */
static esp_err_t display_initialize_gpio(void)
{
    esp_err_t err = ESP_OK;

    // init dc pin
    esp_rom_gpio_pad_select_gpio(DC_PIN);
    err = gpio_set_direction(DC_PIN, GPIO_MODE_OUTPUT);
    if (err != ESP_OK)
    {
        printf("D/C# pin initialization failed.\n");
        return err;
    }

    // init reset pin
    esp_rom_gpio_pad_select_gpio(RESET_PIN);
    err = gpio_set_direction(RESET_PIN, GPIO_MODE_OUTPUT);
    if (err != ESP_OK)
    {
        printf("Reset pin initialization failed.\n");
        return err;
    }

    // init busy pin
    esp_rom_gpio_pad_select_gpio(BUSY_PIN);
    err = gpio_set_direction(BUSY_PIN, GPIO_MODE_INPUT);
    if (err != ESP_OK)
    {
        printf("Busy pin initialization failed.\n");
        return err;
    }

    return err;
}

/*!
 * @brief Initialize spi bus for communicating with display.
 * @param[out] p_spi_handle A pointer to a spi handle structure.
 * @return Esp error code.
 */
static esp_err_t display_initialize_spi(spi_device_handle_t *p_spi_handle)
{
    // initialize bus configuration
    spi_bus_config_t bus_cfg = {
        .miso_io_num = -1,         // MISO
        .mosi_io_num = MOSI_PIN,   // MOSI
        .sclk_io_num = SCLK_PIN,   // SCK
        .quadwp_io_num = -1,       // Unused
        .quadhd_io_num = -1,       // Unused
    };

    // initialize the spi bus
    esp_err_t err = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK)
    {
        printf("SPI bus initialization failed.\n");
        return err;
    }
    
    // create device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_CLOCK_SPEED,
        .mode = SPI_MODE,
        .spics_io_num = CS_PIN,
        .queue_size = SPI_QUEUE_SIZE,
    };

    // register device connected to the bus
    err = spi_bus_add_device(SPI2_HOST, &devcfg, p_spi_handle);
    if (err != ESP_OK)
    {
        printf("SPI bus addition failed.\n");
        return err;
    }

    return ESP_OK;
}

/*!
 * @brief Perform hardware and software reset.
 * @param[in] spi_handle A spi device handle structure.
 * @todo Check for errors
 */
static void display_reset(spi_device_handle_t spi_handle)
{
    // hardware reset
    gpio_set_level(RESET_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(RESET_DELAY_MS));
    gpio_set_level(RESET_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(RESET_DELAY_MS));

    // sw reset
    display_write_command(spi_handle, 0x12);
    vTaskDelay(pdMS_TO_TICKS(RESET_DELAY_MS));
}

/*!
 * @brief Set gate driver, ram settings, and panel border.
 * @param[in] spi_handle A spi device handle structure.
 */
static void display_configure_internals(spi_device_handle_t spi_handle)
{
    // set gate driver output
    display_write_command(spi_handle, 0x01);
    uint8_t gate_driver_out[] = {0xc7, 0x00, 0x00};
    display_write_data(spi_handle, gate_driver_out, 3);

    // set ram settings
    display_write_command(spi_handle, 0x11);
    uint8_t data_entry_mode[] = {0x03}; // x increase, y increase : normal mode
    display_write_data(spi_handle, data_entry_mode, 1);

    // set ram x range
    display_write_command(spi_handle, 0x44);
    uint8_t ram_x_range[] = {0x00, (DISPLAY_WIDTH - 1) / BITS_PER_BYTE};
    display_write_data(spi_handle, ram_x_range, 2);

    // set ram y range
    display_write_command(spi_handle, 0x45);
    uint8_t ram_y_range[] = {0x00, 0x00, (DISPLAY_HEIGHT - 1), 0x00};
    display_write_data(spi_handle, ram_y_range, 4);

    // set panel border
    display_write_command(spi_handle, 0x3C);
    uint8_t panel_border[] = {0x05};
    display_write_data(spi_handle, panel_border, 1);
}

/*!
 * @brief Configure update sequence and update screen.
 * @param[in] spi_handle  A spi device handle structure.
 * @todo Set softstart
 */
static void display_set_update_sequence(spi_device_handle_t spi_handle)
{
    // set display update sequence
    display_write_command(spi_handle, 0x22);
    uint8_t update_sequence[] = {0xFF};
    display_write_data(spi_handle, update_sequence, sizeof(update_sequence));
    display_update(spi_handle);
}

/*!
 * @brief Write data to display ram.
 * @param[in] spi_handle A spi device handle structure.
 * @param[in] image An image structure. p_data must be >= width.
 * @param[in] x The x location of the top left corner of the image.
 * @param[in] y The y location of the top left corner of the image.
 * @todo Raise error
 */
void display_write_to_ram(spi_device_handle_t spi_handle, image_t image, uint8_t x, uint8_t y, uint8_t scale)
{
    // check if image exists
    if (image.p_data == NULL) {
        printf("Image data pointer is NULL.\n");
        return;
    }

    // check if image goes out of bounds
    if ((x + image.w * scale > DISPLAY_WIDTH) ||
        (y + image.h * scale > DISPLAY_HEIGHT))
    {
        printf("Image size exceeds display bounds.\n");
        return;
    }

    // calculate sizes
    uint8_t row_size_bytes = util_ceiling_divide(image.w, BITS_PER_BYTE);
    uint8_t row_size_bytes_scaled = util_ceiling_divide(image.w * scale, BITS_PER_BYTE);

    // write to ram one row at a time
    for (uint8_t i = 0; i < image.h; i++)
    {
        uint16_t row_byte_idx = (image.w * i) / BITS_PER_BYTE;
        uint8_t row_bit_idx = (image.w * i) - (BITS_PER_BYTE * row_byte_idx);

        // create scaled row
        uint8_t p_row_scaled[util_ceiling_divide(DISPLAY_WIDTH, BITS_PER_BYTE)];
        memset(p_row_scaled, 0xFF, row_size_bytes_scaled);
        util_bitwise_scale(p_row_scaled, image.p_data + row_byte_idx, row_bit_idx, image.w, scale);

        // write multiple rows for scaling
        for (uint8_t h = 0; h < scale; h++)
        {
            // set x address counter
            display_write_command(spi_handle, 0x4E);
            uint8_t x_address[] = {x / BITS_PER_BYTE};
            display_write_data(spi_handle, x_address, sizeof(x_address));

            // set y address counter
            display_write_command(spi_handle, 0x4F);
            uint8_t y_address[] = {y + i * scale + h};
            display_write_data(spi_handle, y_address, sizeof(y_address));

            // write row to ram
            display_write_command(spi_handle, 0x24);
            display_write_data(spi_handle, p_row_scaled, row_size_bytes_scaled);
        }
    }
}

/*!
 * @brief Run display update cycle.
 * @param[in] spi_handle A spi device handle structure.
 * @todo Ensure await busy loop will not get stuck.
 */
void display_update(spi_device_handle_t spi_handle)
{
    // activate display update sequence
    display_write_command(spi_handle, 0x20);
    // wait busy low
    uint8_t busy = 1;
    while (busy)
    {
        busy = gpio_get_level(BUSY_PIN);
        vTaskDelay(pdMS_TO_TICKS(AWAIT_BUSY_DELAY_MS));
    }
}

/*!
 * @brief Write a command to the display.
 * @param[in] spi_handle  A spi device handle structure.
 * @param[in] command     An 8-bit command code.
 */
static void display_write_command(spi_device_handle_t spi_handle, uint8_t command)
{
    // Set D/C# low for command mode
    gpio_set_level(DC_PIN, 0);
    // setup transaction
    spi_transaction_t transaction = {
        .length = BITS_PER_BYTE,
        .tx_buffer = &command
    };
    esp_err_t err = spi_device_transmit(spi_handle, &transaction);
    if (err != ESP_OK) {
        printf("SPI transmission failed\n");
    }
}

/*!
 * @brief Write data to the display.
 * @param[in] spi_handle  A spi device handle structure.
 * @param[in] p_data      A pointer to a byte array.
 * @param[in] length      The number of bytes to write.
 */
static void display_write_data(spi_device_handle_t spi_handle, uint8_t *p_data, uint8_t length)
{
    // Set D/C# high for data mode
    gpio_set_level(DC_PIN, 1);
    spi_transaction_t transaction = {
        .length = BITS_PER_BYTE * length,
        .tx_buffer = p_data
    };
    esp_err_t err = spi_device_transmit(spi_handle, &transaction);
    if (err != ESP_OK) {
        printf("SPI transmission failed\n");
    }
}

/*!
 * @brief Initializes display settings and clears the screen.
 * @return Esp error code. 
 */
esp_err_t display_initialize(spi_device_handle_t *p_spi_handle)
{
    esp_err_t err = ESP_OK;

    // initialize gpio
    err = display_initialize_gpio();
    if (err != ESP_OK)
    {
        return err;
    }

    // initialize spi bus
    err = display_initialize_spi(p_spi_handle);
    if (err != ESP_OK)
    {
        return err;
    }

    // set initial configuration
    display_reset(*p_spi_handle);

    // send initialization code
    display_configure_internals(*p_spi_handle);

    // load waveform lut
    display_set_update_sequence(*p_spi_handle);

    // clear screen
    display_clear_ram(*p_spi_handle);
    display_update(*p_spi_handle);
    display_clear_ram(*p_spi_handle);

    return err;
}

/*!
 * @brief Sets the ram of the display to all ones.
 */
void display_clear_ram(spi_device_handle_t spi_handle) {
    // create blank image, one row long
    uint8_t data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    image_t blank = {
        8U, 8U, data
    };
    // set ram and update
    display_write_to_ram(spi_handle, blank, 0, 0, DISPLAY_WIDTH / blank.w);
}

/*** end of file ***/
