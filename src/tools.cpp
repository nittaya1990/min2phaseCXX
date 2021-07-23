#include <min2phase/tools.h>
#include "coords.h"

namespace min2phase { namespace tools {

    //set random seed
    void setRandomSeed(uint32_t seed) {
        std::srand(seed);
    }

    //get random cube
    std::string randomState() {
        if (!coords::isInit())
            return "";

        CubieCube cube;
        int8_t parity;
        int16_t cornerOri, edgeOri;
        uint16_t cornerPerm;
        int32_t edgePerm;

        cornerPerm = std::rand() % (info::N_PERM + 1);
        cornerOri = std::rand() % (info::N_TWIST + 1);
        edgeOri = std::rand() % (info::N_FLIP + 1);
        parity = CubieCube::getNParity(cornerPerm, info::NUMBER_CORNER);

        do {
            edgePerm = std::rand() % (info::FULL_E_PERM + 1);
        } while (CubieCube::getNParity(edgePerm, info::NUMBER_EDGES) != parity);

        cube.setValues(cornerPerm, cornerOri, edgePerm, edgeOri);

        return CubieCube::toFaceCube(cube);
    }

    //convert moves to scrambled cube
    std::string fromScramble(const int8_t *scramble, uint8_t length) {
        uint8_t i;
        CubieCube c1;
        CubieCube c2;
        CubieCube tmp;

        for (i = 0; i < length; i++) {
            CubieCube::CornMult(c1, coords::moveCube[scramble[i]], c2);
            CubieCube::EdgeMult(c1, coords::moveCube[scramble[i]], c2);
            tmp = c1;
            c1 = c2;
            c2 = tmp;
        }

        delete[] scramble;

        return CubieCube::toFaceCube(c1);
    }

    //convert moves to cube
    std::string fromScramble(const std::string &s) {
        int8_t *arr, *ret;
        uint8_t length = 0, n_moves;
        int8_t i, j, axis;

        while (s[length] != '\0')
            length++;

        arr = new int8_t[length];
        j = 0;
        axis = -1;

        for (i = 0; i < length; i++) {
            arr[i] = 0;

            switch (s[i]) {
                case 'U':
                    axis = 0;
                    break;
                case 'R':
                    axis = 3;
                    break;
                case 'F':
                    axis = 6;
                    break;
                case 'D':
                    axis = 9;
                    break;
                case 'L':
                    axis = 12;
                    break;
                case 'B':
                    axis = 15;
                    break;
                case ' ':
                    if (axis != -1)
                        arr[j++] = axis;
                    axis = -1;
                    break;
                case '2':
                    axis++;
                    break;
                case '\'':
                    axis += 2;
                    break;
                default:
                    continue;
            }
        }

        if (axis != -1) arr[j++] = axis;

        ret = new int8_t[j];
        n_moves = j;

        while (--j >= 0)
            ret[j] = arr[j];

        delete[] arr;

        return fromScramble(ret, n_moves);
    }
} }