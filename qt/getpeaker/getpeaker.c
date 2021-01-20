
#include "getpeaker.h"



int16_t getPeak(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    int16_t numPeaks = 0;
    for(int16_t r = 0; r < r_size; ++r){
        for(int16_t d = 0; d < d_size; ++d){
            int16_t val12 = (r > 0) ? RD[(r - 1) * r_size + d] : 0x8000;
            int16_t val21 = (d > 0) ? RD[r * r_size + (d - 1)] : 0x8000;
            int16_t val22 = RD[r * r_size + d];
            int16_t val23 = (d < d_size - 1) ? RD[r * r_size + (d + 1)] : 0x8000;
            int16_t val32 = (r < r_size - 1) ? RD[(r + 1) * r_size + d] : 0x8000;
            if((val22 > val12) && (val22 > val21) && (val22 > val23) && (val22 > val32)){
                outIndexRD[numPeaks] = r << 16 | d;
                ++numPeaks;
            }
        }
    }

    return numPeaks;
}
