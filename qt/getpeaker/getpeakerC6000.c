#include "getpeaker.h"
#include "limits.h"

enum {COEF_10dB = 10}; // = 10^1

int16_t calcTreshold_C6000(int16_t * RD, int32_t size){
    int64_t sumA = 0;
    int64_t sumB = 0;
    for(int32_t i = 0; i < size/2; ++i){
        sumA += RD[i];
        sumB += RD[i + size/2];
    }
    int16_t average = (sumA+sumB)/size;
    int16_t treshold = average * COEF_10dB;
    return treshold;
}

int16_t getPeak_C6000(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    static const int16_t offsetIndexes[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    int16_t treshold = calcTreshold_C6000(RD, r_size*d_size);

    int16_t numPeaks = 0;
    for(int16_t d = 0; d < d_size; ++d){
        for(int16_t r = 0; r < r_size; ++r){
            int16_t val = RD[d * r_size + r];
            int16_t topCnt = 0;
            if (val > treshold){
                for(int16_t i = 0; i < 8; ++i){
                    int16_t d_sh = d + offsetIndexes[i][0];
                    int16_t r_sh = r + offsetIndexes[i][1];
                    int16_t valAdjacent = ((r_sh < 0)||(d_sh < 0)||(r_sh >= r_size)||(d_sh >= d_size))
                            ? SHRT_MIN : RD[d_sh * r_size + r_sh];
                    topCnt += (val > valAdjacent);
                }
                if (topCnt == 8){
                    outIndexRD[numPeaks] = r << 16 | d;
                    ++numPeaks;
                }
            }
        }
    }

    return numPeaks;
}
