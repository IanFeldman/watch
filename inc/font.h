#ifndef FONT_H
#define FONT_H

#include <stdint.h>

extern image_t characters_6x8[];
extern const image_t sphere_64x64;

void font_write_string(spi_device_handle_t spi_handle, uint8_t string[], uint8_t x, uint8_t y, uint8_t scale, uint8_t spacing);

#endif // FONT_H

