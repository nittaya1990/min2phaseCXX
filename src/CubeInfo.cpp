#include "CubeInfo.h"

namespace min2phase { namespace CubeInfo {

    short Cnk[PRECOMPUTED_CNK][PRECOMPUTED_CNK] = {0};
    int8_t std2ud[N_MOVES] = {0};
    short ckmv2bit[N_MOVES2+1] = {0};

    void init(){
        initCnk();
        initStd2ud();
        initCkmv2bit();
    }


    void initCnk() {
        uint8_t i, j;

        for (i = 0; i < PRECOMPUTED_CNK; i++) {
            Cnk[i][0] = Cnk[i][i] = 1;

            for (j = 1; j < i; j++)
                Cnk[i][j] = Cnk[i - 1][j - 1] + Cnk[i - 1][j];
        }
    }

    void initStd2ud() {
        for (int8_t i = 0; i < N_MOVES; i++)
            std2ud[ud2std[i]] = i;

    }

    void initCkmv2bit() {
        uint8_t i, j, ix, jx;

        for (i = 0; i < N_MOVES2; i++) {
            ix = ud2std[i] / 3;
            ckmv2bit[i] = 0;

            for (j = 0; j < N_MOVES2; j++) {
                jx = ud2std[j] / 3;
                ckmv2bit[i] |= ((ix == jx) || ((ix % 3 == jx % 3) && (ix >= jx)) ? 1 : 0) << j;
            }
        }

        ckmv2bit[10] = 0;
    }
} }