#ifndef MIN2PHASE_SEARCH_H
#define MIN2PHASE_SEARCH_H 1

#include "Coords.h"

namespace min2phase {

    class Search {
    public:

        static const int8_t MAX_PRE_MOVES = 20;
        static const int8_t MIN_P1LENGTH_PRE = 7;

        Coords::CoordCube nodeUD[MAX_PRE_MOVES + 1]{};
        CubieCube urfCubieCube[CubeInfo::N_BASIC_MOVES];
        Coords::CoordCube urfCoordCube[CubeInfo::N_BASIC_MOVES]{};
        CubieCube phase1Cubie[MAX_PRE_MOVES + 1];
        CubieCube preMoveCubes[MAX_PRE_MOVES + 1];
        CubieCube solveCube;
        CubieCube::OutputFormat solution;

        int8_t move[CubeInfo::MAX_MOVES_SOLVE]{};
        int8_t preMoves[MAX_PRE_MOVES]{};

        long selfSym = 0;
        int8_t conjMask = 0;
        int8_t urfIdx = 0;
        int8_t length1 = 0;
        int8_t depth1 = 0;
        int8_t maxDep2 = 0;
        int8_t solLen = 0;
        int probe = 0;
        int probeMax = 0;
        int probeMin = 0;
        int8_t verbose = 0;
        int8_t valid1 = 0;
        int8_t preMoveLen = 0;
        int8_t maxPreMoves = 0;
        bool allowShorter = false;

        Search() = default;

        /**
         * This is used to solve the cube. It has many configuration in base of the
         * power of the machine or the memory. Less the max depth is, more the time
         * to solve the cube is used, is suggested to use from 20 to 31 because it
         * indicates the max number of moves are used to solve the cube. The probe
         * indicate the number of cube that the algorithm is forced to explore.
         * If you need speed i suggest 0 for the probeMin and 100000000 for the max.
         * You can also change the formatting string in output adding it at verbose,
         * for example OutputFormat::USE_SEPARATOR | OutputFormat::INVERSE_SOLUTION.
         * This the order of the colors for the string:
         * U1 -> U9 R1 -> R9 F1 -> F9 D1 -> D9 L1 -> L9 B1 -> B9
         * Solved cube: UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB
         *
         *             |************|
         *             |*U1**U2**U3*|
         *             |************|
         *             |*U4**U5**U6*|
         *             |************|
         *             |*U7**U8**U9*|
         *             |************|
         * ************|************|************|************|
         * *L1**L2**L3*|*F1**F2**F3*|*R1**R2**R3*|*B1**B2**B3*|
         * ************|************|************|************|
         * *L4**L5**L6*|*F4**F5**F6*|*R4**R5**R6*|*B4**B5**B6*|
         * ************|************|************|************|
         * *L7**L8**L9*|*F7**F8**F9*|*R7**R8**R9*|*B7**B8**B9*|
         * ************|************|************|************|
         *             |************|
         *             |*D1**D2**D3*|
         *             |************|
         *             |*D4**D5**D6*|
         *             |************|
         *             |*D7**D8**D9*|
         *             |************|
         *
         * @param facelet  : the order of colors like the cube above.
         * @param maxDepth : the max of moves used to solve the cube, from 20 to 31.
         * @param probeMax : the max of cube explored in phase 2.
         * @param probeMin : the min of cube explored in phase 2.
         * @param verbose  : the format for the output strings.
         * @return         : the moved necessary to solve the cube. Delete the return value when
         *                   you don't need it anymore.
         */
        std::string solve(const std::string &facelets, int8_t maxDepth, int probeMax, int probeMin, int8_t verbose);

        int8_t verify(const std::string &facelets);

        void initSearch();

        std::string search();

        int8_t phase1PreMoves(int8_t maxl, int8_t lm, CubieCube *cc, unsigned short ssym);

        int8_t phase1(Coords::CoordCube *node, unsigned short ssym, int8_t maxl, int8_t lm);

        int8_t initPhase2Pre();

        int8_t initPhase2(unsigned short p2corn, int8_t p2csym, unsigned short p2edge, int8_t p2esym, int8_t p2mid, unsigned short edgei,
                          unsigned short corni);

        int8_t phase2(unsigned short edge, int8_t esym, unsigned short corn, int8_t csym, int8_t mid, int8_t maxl, int8_t depth,
                      int8_t lm);
    };
}

#endif //MIN2PHASE_SEARCH_H