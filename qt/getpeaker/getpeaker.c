
#include "getpeaker.h"
#include "limits.h"

enum {COEF_10dB = 10}; // = 10^1

uint16_t calcTreshold(uint16_t * RD, uint32_t size){
    uint64_t sum = 0;
    for(uint32_t i = 0; i < size; ++i){
        sum += RD[i];
    }
    uint16_t average = sum/size;
    uint32_t treshold = (uint32_t)average * COEF_10dB;
    treshold = (treshold > SHRT_MAX) ? SHRT_MAX : treshold;
    return treshold;
}

int16_t getPeak(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    static const int16_t offsetIndexes[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    uint16_t* rd = (uint16_t *)RD; // unsighed 16 bit real data !!!
    uint16_t treshold = calcTreshold(rd, r_size*d_size);
    int16_t numPeaks = 0;
    for(int16_t d = 0; d < d_size; ++d){
        for(int16_t r = 0; r < r_size; ++r){
            uint16_t val = rd[d * r_size + r];
            int16_t topCnt = 0;
            if (val > treshold){
                for(int16_t i = 0; i < 8; ++i){
                   int16_t d_sh = d + offsetIndexes[i][0];
                   int16_t r_sh = r + offsetIndexes[i][1];
                   uint16_t valAdjacent = 0;
                   if ((r_sh < 0)||(d_sh < 0)||(r_sh >= r_size)||(d_sh >= d_size)){
                       valAdjacent = 0;
                   }else{
                       valAdjacent = rd[d_sh * r_size + r_sh];
                   }
                   if (val > valAdjacent){
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
    }

    return numPeaks;
}


int16_t getPeak3D(uint16_t * RDX, int16_t r_size, int16_t d_size, int16_t x_size, uint64_t * outIndexRDX){
    static const int16_t offsetIndexes[26][3] = {
        {-1, -1, -1}, {0, -1, -1}, {1, -1, -1},
        {-1, 0, -1},  {0, 0, -1},  {1, 0, -1},
        {-1, 1, -1},  {0, 1, -1},  {1, 1, -1},

        {-1, -1, 0},  {0, -1, 0},  {1, -1, 0},
        {-1, 0, 0},                {1, 0, 0},
        {-1, 1, 0},   {0, 1, 0},   {1, 1, 0},

        {-1, -1, 1},  {0, -1, 1},  {1, -1, 1},
        {-1, 0, 1},   {0, 0, 1},   {1, 0, 1},
        {-1, 1, 1},   {0, 1, 1},   {1, 1, 1}
    };
    uint16_t treshold = calcTreshold(RDX, r_size*d_size*x_size);
    int16_t numPeaks = 0;
    for(int16_t x = 0; x < x_size; ++x){
        for(int16_t d = 0; d < d_size; ++d){
            for(int16_t r = 0; r < r_size; ++r){
                uint16_t val = RDX[x * r_size * d_size + d * r_size + r];
                int16_t topCnt = 0;
                if (val > treshold){
                    for(int16_t i = 0; i < 26; ++i){
                        int16_t r_sh = r + offsetIndexes[i][0];
                        int16_t d_sh = d + offsetIndexes[i][1];
                        int16_t x_sh = x + offsetIndexes[i][2];
                        uint16_t valAdjacent = 0;
                        valAdjacent = ((r_sh < 0)||(d_sh < 0)||(x_sh < 0)||(r_sh >= r_size)||(d_sh >= d_size)||(x_sh >= x_size))
                           ? 0 : RDX[x_sh*r_size*d_size + d_sh*r_size + r_sh];
                        if (val > valAdjacent){
                           ++topCnt;
                        }
                    }
                    if (topCnt == 26){
                        outIndexRDX[numPeaks] = ((int64_t)r << 32) | ((int64_t)d << 16) | x;
                        ++numPeaks;
                    }
                }
            }
        }
    }

    return numPeaks;
}





