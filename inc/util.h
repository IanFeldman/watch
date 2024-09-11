#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

#define BITS_PER_BYTE 8


/*!
 * @brief Divide two integers and always round up. Dividing by zero results in zero.
 * @param[in] dividend The numerator.
 * @param[in] divisor The denominator.
 * @return The quotient.
 */
uint32_t util_ceiling_divide(uint32_t dividend, uint32_t divisor)
{
    if (divisor == 0)
    {
        return 0;
    }
    return (dividend + (divisor - 1U)) / divisor;
}


/*!
 * @brief Duplicate each bit in the source array into the destination array. Start from a bit offset
          in the source array. Assume the destination is large enough to fit the scaled source.
 * @param[out] dest 8-bit scaled array.
 * @param[in] src 8-bit unscaled input array.
 * @param[in] src_bit_offset Number of bits from the left that the source array starts on.
 * @param[in] length The length of src in bytes.
 * @param[in] scale The scale factor to be applied.
 */
void util_bitwise_scale(uint8_t *dest, uint8_t *src, uint8_t src_bit_offset, uint32_t length, uint32_t scale)
{
    uint32_t dest_byte_idx = 0;
    uint32_t dest_bit_idx  = 0;

    // iterate over bytes
    for (uint32_t i = 0; i < length; i++)
    {
        uint8_t j = 0x80;
        // apply src bit offset if this is byte 0
        if (i == 0)
        {
            j >>= src_bit_offset;
        }
        // iterate over bits left to right
        for (; j >= 0x01; j >>= 1U)
        {
            uint32_t src_bit = src[i] & j;

            // copy bit into destination k times
            for (uint32_t k = 0; k < scale; k++)
            {
                uint8_t dest_bit = 0x80 >> dest_bit_idx;
                if (src_bit)
                {
                    dest[dest_byte_idx] |= dest_bit;
                }
                else
                {
                    dest[dest_byte_idx] &= ~dest_bit;
                }

                // check if we are on the next byte
                if (++dest_bit_idx >= BITS_PER_BYTE)
                {
                    dest_byte_idx++;
                    dest_bit_idx = 0;
                }
            }
        }
    }
}


#endif // UTIL_H
