#ifndef GETPEAKERC6000_H
#define GETPEAKERC6000_H

#include <stdint.h>

int16_t getPeak_C6000(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD);

uint16_t calcTreshold_C6000(uint16_t * RD, uint32_t size);

#endif // GETPEAKERC6000_H
