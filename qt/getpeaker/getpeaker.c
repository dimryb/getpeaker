
#include "getpeaker.h"



int16_t getPeak1(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
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

int16_t getPeak2(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    int16_t numPeaks = 0;
    for(int16_t r = 0; r < r_size; ++r){
        for(int16_t d = 0; d < d_size; ++d){
            int16_t val21 = (d > 0) ? RD[r * r_size + (d - 1)] : 0x8000;
            int16_t val22 = RD[r * r_size + d];
            if(val22 > val21){
                int16_t val23 = (d < d_size - 1) ? RD[r * r_size + (d + 1)] : 0x8000;
                if(val22 > val23){
                    int16_t val12 = (r > 0) ? RD[(r - 1) * r_size + d] : 0x8000;
                    if(val22 > val12){
                        int16_t val32 = (r < r_size - 1) ? RD[(r + 1) * r_size + d] : 0x8000;
                        if(val22 > val32){
                            outIndexRD[numPeaks] = r << 16 | d;
                            ++numPeaks;
                        }
                    }
                }
            }
        }
    }

    return numPeaks;
}


static const int16_t offsetIndexes[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1}, {1, 0}, {1, 1}
};

int16_t getPeak3(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    int16_t numPeaks = 0;
    for(int16_t r = 0; r < r_size; ++r){
        for(int16_t d = 0; d < d_size; ++d){
            int16_t val22 = RD[r * r_size + d];
            int16_t topCnt = 0;
            for(int16_t i = 0; i < 8; ++i){
               int16_t r_sh = r + offsetIndexes[i][0];
               int16_t d_sh = d + offsetIndexes[i][1];
               int16_t val = 0;
               if ((r_sh < 0)||(d_sh < 0)||(r_sh >= r_size)||(d_sh >= d_size)){
                   val = 0x8000;
               }else{
                   val = RD[r_sh * r_size + d_sh];
               }
               if (val22 > val){
                   ++topCnt;
               }else{
                   break;
               }
            }
            if (topCnt == 8){
                outIndexRD[numPeaks] = r << 16 | d;
                ++numPeaks;
            }
        }
    }

    return numPeaks;
}
