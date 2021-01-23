#ifndef GETPEAKER_H
#define GETPEAKER_H

#include <stdint.h>

int16_t getPeak(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD);
int16_t getPeak3D(int16_t * RDX, int16_t r_size, int16_t d_size, int16_t x_size, uint64_t * outIndexRDX);

int16_t calcTreshold(int16_t * RD, int32_t size);

#endif // GETPEAKER_H
