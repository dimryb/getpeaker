#ifndef GETPEAKER_H
#define GETPEAKER_H

#include <stdint.h>

int16_t getPeak1(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD);
int16_t getPeak2(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD);
int16_t getPeak3(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD);

#endif // GETPEAKER_H
