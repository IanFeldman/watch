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
          in the source array. Assume the destination is large enough to fit the scaled source and is
          initialized to all 0xFF.
 * @param[out] dest 8-bit scaled array.
 * @param[in] src 8-bit unscaled input array.
 * @param[in] src_bit_offset Number of bits from the left that the source array starts on.
 * @param[in] src_bit_len The length of src in bits.
 * @param[in] scale The scale factor to be applied.
 */
void util_bitwise_scale(uint8_t *dest, uint8_t *src, uint8_t src_bit_offset, uint32_t src_bit_len, uint32_t scale)
{
    uint32_t src_byte_idx = 0;
    uint32_t src_bit_idx = src_bit_offset;
    uint32_t dest_byte_idx = 0;
    uint32_t dest_bit_idx = 0;
    uint32_t src_bit_counter = 0;

    while (src_bit_counter < src_bit_len)
    {
        // get source bit value
        uint32_t src_bit_val = src[src_byte_idx] & (0x80 >> src_bit_idx);

        // duplicate value into dest
        for (uint32_t i = 0; i < scale; i++)
        {
            if (!src_bit_val)
            {
                dest[dest_byte_idx] &= ~(0x80 >> dest_bit_idx);
            }

            // increment destination bit
            if (++dest_bit_idx == BITS_PER_BYTE)
            {
                dest_bit_idx = 0;
                dest_byte_idx++;
            }
        }

        // increment source bit
        if (++src_bit_idx == BITS_PER_BYTE)
        {
            src_bit_idx = 0;
            src_byte_idx++;
        }

        src_bit_counter++;
    }
}


#endif // UTIL_H
