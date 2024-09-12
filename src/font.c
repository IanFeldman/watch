#include "display.h"
#include "font.h"

image_t characters_6x8[] =
{
    [ 'A' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0x80, 0x79, 0xe7, 0x9e} },
    [ 'B' ] = { 6U, 8U, (uint8_t[]) {0x50, 0xe7, 0x81, 0x79, 0xe7, 0x81} },
    [ 'C' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0xdf, 0x7d, 0xf7, 0xa1} },
    [ 'D' ] = { 6U, 8U, (uint8_t[]) {0x05, 0xe7, 0x9e, 0x79, 0xe7, 0x81} },
    [ 'E' ] = { 6U, 8U, (uint8_t[]) {0x81, 0xf7, 0xc1, 0x7d, 0xf7, 0xc0} },
    [ 'F' ] = { 6U, 8U, (uint8_t[]) {0x81, 0xf7, 0xc1, 0x7d, 0xf7, 0xdf} },
    [ 'G' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0xd8, 0x79, 0xe7, 0xa1} },
    [ 'H' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xe7, 0x80, 0x79, 0xe7, 0x9e} },
    [ 'I' ] = { 6U, 8U, (uint8_t[]) {0x70, 0x7d, 0xf7, 0xdf, 0x7d, 0xc1} },
    [ 'J' ] = { 6U, 8U, (uint8_t[]) {0x83, 0xdf, 0x7d, 0xf5, 0xd7, 0x63} },
    [ 'K' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xd6, 0xc7, 0x6d, 0xd7, 0x9e} },
    [ 'L' ] = { 6U, 8U, (uint8_t[]) {0x7d, 0xf7, 0xdf, 0x7d, 0xf7, 0xc0} },
    [ 'M' ] = { 6U, 8U, (uint8_t[]) {0x78, 0xc0, 0x12, 0x79, 0xe7, 0x9e} },
    [ 'N' ] = { 6U, 8U, (uint8_t[]) {0x38, 0x65, 0x96, 0x69, 0xa6, 0x1c} },
    [ 'O' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0x9e, 0x79, 0xe7, 0xa1} },
    [ 'P' ] = { 6U, 8U, (uint8_t[]) {0x50, 0xe7, 0x81, 0x7d, 0xf7, 0xdf} },
    [ 'Q' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0x9e, 0x79, 0x27, 0x21} },
    [ 'R' ] = { 6U, 8U, (uint8_t[]) {0x05, 0xe7, 0x81, 0x75, 0xe7, 0x9e} },
    [ 'S' ] = { 6U, 8U, (uint8_t[]) {0x81, 0xf7, 0xe1, 0xfb, 0xe7, 0xa1} },
    [ 'T' ] = { 6U, 8U, (uint8_t[]) {0x30, 0x7d, 0xf7, 0xdf, 0x7d, 0xf7} },
    [ 'U' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xe7, 0x9e, 0x79, 0xe7, 0xa1} },
    [ 'V' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xe7, 0x9e, 0xb6, 0xdc, 0xf3} },
    [ 'W' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xe7, 0x9e, 0x48, 0x03, 0x1e} },
    [ 'X' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xeb, 0x73, 0xce, 0xd7, 0x9e} },
    [ 'Y' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xeb, 0x73, 0xef, 0x7d, 0xef} },
    [ 'Z' ] = { 6U, 8U, (uint8_t[]) {0x30, 0xef, 0x7b, 0xde, 0xf7, 0xc0} },

    [ '0' ] = { 6U, 8U, (uint8_t[]) {0x84, 0xe5, 0x96, 0x69, 0xa7, 0x21} },
    [ '1' ] = { 6U, 8U, (uint8_t[]) {0xe7, 0x5b, 0x7d, 0xf7, 0xdf, 0x7d} },
    [ '2' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xef, 0xbd, 0xce, 0xf7, 0xc0} },
    [ '3' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xef, 0xb1, 0xfb, 0xe7, 0xa1} },
    [ '4' ] = { 6U, 8U, (uint8_t[]) {0x79, 0xe7, 0xa0, 0xfb, 0xef, 0xbe} },
    [ '5' ] = { 6U, 8U, (uint8_t[]) {0x10, 0xf7, 0xc1, 0xfb, 0xe7, 0xa1} },
    [ '6' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0xc1, 0x79, 0xe7, 0xa1} },
    [ '7' ] = { 6U, 8U, (uint8_t[]) {0x30, 0xef, 0x7b, 0xdf, 0x7d, 0xf7} },
    [ '8' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0xa1, 0x79, 0xe7, 0xa1} },
    [ '9' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xe7, 0xa0, 0xfb, 0xe7, 0xa1} },

    ['\'' ] = { 6U, 8U, (uint8_t[]) {0xdf, 0x7b, 0xff, 0xff, 0xff, 0xff} },
    [ '.' ] = { 6U, 8U, (uint8_t[]) {0xff, 0xff, 0xff, 0xff, 0xff, 0xef} },
    [ '!' ] = { 6U, 8U, (uint8_t[]) {0xbe, 0xfb, 0xef, 0xbf, 0xff, 0xef} },
    [ ',' ] = { 6U, 8U, (uint8_t[]) {0xff, 0xff, 0xff, 0xff, 0x7d, 0xef} },
    [ '?' ] = { 6U, 8U, (uint8_t[]) {0x85, 0xef, 0xb9, 0xdf, 0x7f, 0xf7} },
    [ ':' ] = { 6U, 8U, (uint8_t[]) {0xff, 0xfd, 0xff, 0xff, 0x7f, 0xff} },
    [ ';' ] = { 6U, 8U, (uint8_t[]) {0xff, 0xfd, 0xff, 0xff, 0x7d, 0xef} }
};

const image_t sphere_64x64 = {
    64U, 64U, (uint8_t[])
    {
        0xff, 0xff, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xfa, 0xff, 0xff, 0xbf, 0xff, 0xff, 
        0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
        0xff, 0xff, 0xbf, 0xff, 0xff, 0xfb, 0xff, 0xff, 
        0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 
        0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 
        0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 
        0xff, 0xeb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 
        0xff, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 
        0xfe, 0xab, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 
        0xfc, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 
        0xf9, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 
        0xf2, 0xab, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 
        0xf1, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 
        0xe2, 0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xc1, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xc2, 0xaa, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xfb, 
        0xc1, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 
        0x82, 0xaa, 0xae, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0x80, 0x77, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
        0x80, 0xaa, 0xab, 0xbf, 0xff, 0xff, 0xff, 0xff, 
        0x80, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
        0x0, 0xaa, 0xaa, 0xef, 0xff, 0xff, 0xff, 0xfe, 
        0x0, 0x57, 0x77, 0xff, 0xff, 0xff, 0xff, 0xfe, 
        0x0, 0xaa, 0xaa, 0xab, 0xbf, 0xff, 0xff, 0xfe, 
        0x0, 0x1, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xfe, 
        0x0, 0x2a, 0xaa, 0xaa, 0xef, 0xff, 0xff, 0xfe, 
        0x0, 0x5, 0x77, 0x7f, 0xff, 0xff, 0xff, 0xfe, 
        0x0, 0xa, 0xaa, 0xaa, 0xab, 0xbf, 0xff, 0xfe, 
        0x0, 0x0, 0x15, 0x7f, 0xff, 0xff, 0xff, 0xfe, 
        0x0, 0x2, 0xaa, 0xaa, 0xaa, 0xee, 0xff, 0xfe, 
        0x0, 0x0, 0x57, 0x77, 0x7f, 0xff, 0xff, 0xfe, 
        0x0, 0x0, 0xaa, 0xaa, 0xaa, 0xab, 0xbb, 0xba, 
        0x0, 0x0, 0x1, 0x15, 0xff, 0xff, 0xff, 0xfe, 
        0x0, 0x0, 0x2a, 0xaa, 0xaa, 0xaa, 0xae, 0xaa, 
        0x0, 0x0, 0x4, 0x57, 0x77, 0x7f, 0xff, 0xf6, 
        0x80, 0x0, 0xa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 
        0x80, 0x0, 0x0, 0x1, 0x15, 0x7f, 0xff, 0xf1, 
        0x80, 0x0, 0x0, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 
        0x80, 0x0, 0x0, 0x4, 0x55, 0x77, 0x77, 0x71, 
        0xc0, 0x0, 0x0, 0xa, 0xaa, 0xaa, 0xaa, 0xab, 
        0xc0, 0x0, 0x0, 0x0, 0x1, 0x11, 0x51, 0x3, 
        0xc0, 0x0, 0x0, 0x0, 0xaa, 0xaa, 0xaa, 0xa3, 
        0xe0, 0x0, 0x0, 0x0, 0x4, 0x45, 0x44, 0x7, 
        0xf0, 0x0, 0x0, 0x0, 0x2, 0xaa, 0xaa, 0xf, 
        0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 
        0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 
        0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 
        0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 
        0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 
        0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 
        0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 
        0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 
        0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 
        0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 
        0xff, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 
        0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 
        0xff, 0xff, 0x80, 0x0, 0x0, 0x1, 0xff, 0xff, 
        0xff, 0xff, 0xe0, 0x0, 0x0, 0x7, 0xff, 0xff, 
        0xff, 0xff, 0xf8, 0x0, 0x0, 0x1f, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 
    }
};
