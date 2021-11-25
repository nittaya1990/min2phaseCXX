#include <min2phase/tools.h>
#include <memory>
#include "coords.h"

namespace min2phase { namespace tools {

    //set random seed
    void setRandomSeed(uint32_t seed) {
        std::srand(seed);
    }

    //get random cube
    std::string randomCube() {
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
    std::string fromScramble(const int8_t scramble[], uint8_t length) {
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

        return CubieCube::toFaceCube(c1);
    }

    //convert moves to cube
    std::string fromScramble(const std::string &s) {
        std::unique_ptr<int8_t> arr(new int8_t[s.length()]);

        int8_t n_moves, axis;

        n_moves = 0;
        axis = -1;

        for (size_t i = 0; i < s.length(); i++) {
            arr.get()[i] = 0;

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
                        arr.get()[n_moves++] = axis;
                    axis = -1;
                    break;
                case '2':
                    axis++;
                    break;
                case '\'':
                    axis += 2;
                    break;
                default:
                    break;
            }
        }

        if (axis != -1)
            arr.get()[n_moves++] = axis;

        return fromScramble(arr.get(), n_moves);
    }

    //gen super flip
    std::string superFlip(){
        return CubieCube::toFaceCube(CubieCube(0, 0, 0, info::N_FLIP-1));
    }
} }
