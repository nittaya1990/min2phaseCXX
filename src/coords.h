/**
 * min2phaseCXX Copyright (C) 2021 Borgo Federico
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
 *
 * This file contains all the necessary for the coordinates.
 * This compute all the coordinates and symmetries
 * necessary for the algorithm.
 * In this are stored all the precomputed coordinate
 * and symmetries used to solve the cube.
 * If you move a face of the cube, the coordinates changes,
 * you can recompute them everytime but it's too slow. So,
 * we create a table where you can put the old coordinate and
 * move and you get the new coordinate.
 * If you apply F to the cube x1 and R to x2, the two cube are the same
 * because if you rotate x1, you get x2. This increase the speed of the algorithm,
 * this is called symmetry. You can also recompute the symmetries every time
 * but is too slow, so we create table.
 * In this class is also stored the pruning table, this is used to get immediately
 * the pruning values for phase 1 and 2 of every coordinate.
 * This class is used to increase the speed of the algorithm computing some coordinate.
 * In this can be also stored the coordinates and info used to
 * identify every cube.
 */

#ifndef MIN2PHASE_COORDS_H
#define MIN2PHASE_COORDS_H 1

#include "CubieCube.h"

namespace min2phase { namespace coords {


    /**
     * This struct contains all the coordinates used in the algorithm.
     */
    typedef struct coords_s{
        /**
         * In this array are stored the cubes result from each possible move
         * from a solved cube. The motion of the corner and edges moved from
         * one of the 18 move is constant. So, we can use basic cubes with the
         * scramble generated from the 18 moves of the cube. In this way,
         * whe can make a multiplication of a cube and a basic move to get
         * a new cube. This increase the speed of the algorithm.
         */
        CubieCube moveCube[info::N_MOVES]{};

        //symmetry
        /**
         * In this array are stored the symmetries generated by S_F2, S_U4
         * and S_LR2. To increase the speed of algorithm, we use the symmetries.
         * You can read the complete definition in the Kociemba's page.
         * If you apply F to the cube x1 and R to x2, the two cube are the same
         * because if you rotate x1, you get x2. This increase the speed of the algorithm.
         */
        CubieCube CubeSym[info::SYM];

        int firstMoveSym[info::FULL_SYM] = {0};


        int8_t SymMove[info::SYM][info::N_MOVES] = {0};//TODO
        int8_t Sym8Move[8 * info::N_MOVES] = {0};//TODO

        /**
         * This is used to convert a symmetry index into a permutation of edges or corner.
         */
        uint16_t PermInvEdgeSym[info::N_PERM_SYM] = {0};


        int64_t moveCubeSym[info::N_MOVES] = {0};//TODO

        /**
         * This matrix is used to convert symmetry index into one symmetry for the CubeSym.
         */
        int8_t SymMult[info::SYM][info::SYM] = {0};

        /**
         * This matrix is used to convert Symmetry for CubeSym into symmetry index.
         */
        int8_t SymMultInv[info::SYM][info::SYM] = {0};

        int8_t SymMoveUD[info::SYM][info::N_MOVES] = {0};//TODO

        /**
         * This is used to convert the middle slice perm.
         */
        int8_t MPermInv[info::N_MPERM] = {0};

        /**
         * This is used to convert the permutation into corner comb.
         */
        int8_t Perm2CombP[info::N_PERM_SYM] = {0};

        /**
         * Raw-Coordinate to Sym-Coordinate.
         * This type of coordinate are used to convert a normal coordinate of the
         * cube into his symmetry. We can compute the corresponding symmetry of a
         * coordinate every time but it's too slow.
         */

        /**
         * This is used to converts the symmetry into the edge orientation.
         * sym = FlipS2RF[ symmetry ]
         */
        uint16_t FlipS2RF[info::N_FLIP_SYM * info::SYM_CLASSES] = {0};

        /**
         * This is used to converts the edge or corner perm into the symmetry.
         * sym = EPermR2S[ edge perm]
         * sym = E2C(EPermR2S[corner perm])
         */
        uint16_t EPermR2S[info::N_PERM] = {0};

        /**
         * This is used to converts the edge orientation into the symmetry.
         * sym = FlipR2S[ edge orientation ]
         */
        uint16_t FlipR2S[info::N_FLIP] = {0};

        /**
         * This is used to converts the corner orientation into the symmetry.
         * sym = TwistR2S[ corner orientation ]
         */
        uint16_t TwistR2S[info::N_TWIST] = {0};

        /**
         * Below there are the coordinate used in phase 1 and 2 (see Kociemba's algorithm).
         * The coordinate XMove indicates the moving table for every coordinate. If you have
         * a cube and rotate a face, the coordinate changes, this matrix help to get the new
         * coordinate immediately.
         * The coordinate with XPrun indicate the table used to get the pruning value from each
         * coordinate.
         * the XConj coordinate indicates the Table used for symmetries.
         */

        ///phase1 coords

        /**
         * This matrix contains the moving table for the edge orientation.
         */
        uint16_t FlipMove[info::N_FLIP_SYM][info::N_MOVES] = {0};

        /**
         * This matrix contains the moving table for the corner orientation.
         */
        uint16_t TwistMove[info::N_TWIST_SYM][info::N_MOVES] = {0};

        /**
         * This matrix contains the moving table for the UDSlice coordinate.
         */
        uint16_t UDSliceMove[info::N_SLICE][info::N_MOVES] = {0};

        /**
         * This matrix contains the moving table for the symmetry in phase 1.
         */
        uint16_t UDSliceConj[info::N_SLICE][info::SYM_CLASSES] = {0};

        /**
         * This matrix contains the pruning table for the UDsliceTwist coordinate.
         */
        int32_t UDSliceTwistPrun[info::N_SLICE * info::N_TWIST_SYM / info::SYM_CLASSES + 1] = {0};

        /**
         * This matrix contains the pruning table for the UDsliceFlip coordinate.
         */
        int32_t UDSliceFlipPrun[info::N_SLICE * info::N_FLIP_SYM / info::SYM_CLASSES + 1] = {0};

        /**
         * This matrix contains the pruning table for the edge and corner orientation coordinate.
         */
        int32_t TwistFlipPrun[info::N_FLIP * info::N_TWIST_SYM / info::SYM_CLASSES + 1] = {0};


        ///phase 2 coords

        /**
         * This matrix contains the moving table for the corner permutation.
         */
        uint16_t CPermMove[info::N_PERM_SYM][info::N_MOVES2] = {0};

        /**
         * This matrix contains the moving table for the edge permutation.
         */
        uint16_t EPermMove[info::N_PERM_SYM][info::N_MOVES2] = {0};

        /**
         * This matrix contains the moving table for the UDSliceSorted permutation.
         */
        uint8_t MPermMove[info::N_MPERM][info::N_MOVES2] = {0};

        /**
         * This matrix contains the moving table for the symmetry of UDSliceSorted.
         */
        uint8_t MPermConj[info::N_MPERM][info::SYM] = {0};

        /**
         * This matrix contains the moving table for the symmetry of corner perm in the upper slice.
         */
        uint8_t CCombPConj[info::N_COMB][info::SYM] = {0};

        /**
         * This matrix contains the pruning table for the edge permutation coordinate.
         */
        int32_t MCPermPrun[info::N_MPERM * info::N_PERM_SYM/info::SYM_CLASSES+1] = {0};

        /**
         * This matrix contains the pruning table for the corner permutation coordinate.
         */
        int32_t EPermCCombPPrun[info::N_COMB*info::N_PERM_SYM/info::SYM_CLASSES+1] = {0};

        /**
         * This matrix contains the moving table for the corner comb permutation.
         */
        uint8_t CCombPMove[info::N_COMB][info::N_MOVES2] = {0};



        /**
         * This the S_URF symmetry that make a rotation of 120 degree of the cube around
         * an axis through the URF-corner and DBL-corner,
         * See Kociemba's algorithm Equivalent Cubes and Symmetry.
         */
        CubieCube urf{};

        /**
         * This the S_URF^-1 symmetry that make a rotation of -120 degree of the cube around
         * an axis through the URF-corner and DBL-corner,
         * See Kociemba's algorithm Equivalent Cubes and Symmetry.
         */
        CubieCube urfInv{};

        /**
         * This indicates if the coordinates and symmetries are initialized.
         */
        bool isInitialized = false;

    }coords_t;

    /**
     * The coordinates object for the algorithm.
     */
    extern coords_t coords;

    /**
     * This function is used to compute all the coordinates and symmetries.
     */
    void init();

    /**
     * Check if the coordinates are initialized.
     *
     * @return      : True if initialized, false if not.
     */
    bool isInit();

    /**
     * This function is used to convert the a cube into a new one from a symmetry index.
     * The symmetry is used to find 2 same cube and it increase the speed of the algorithm.
     * b = S_idx^-1 * a * S_idx for only corner. See the Kociemba's algorithm, Equivalent Cubes and Symmetry.
     *
     * @param a   : the cube to apply the symmetry.
     * @param idx : the index of the symmetry.
     * @param b   : the result cube reference.
     */
    void cornConjugate(const CubieCube &a, int8_t idx, CubieCube &b);

    /**
     * This function is used to convert the a cube into a new one from a symmetry index.
     * The symmetry is used to find 2 same cube and it increase the speed of the algorithm.
     * b = S_idx^-1 * a * S_idx for only edges. See the Kociemba's algorithm, Equivalent Cubes and Symmetry.
     *
     * @param a   : the cube to apply the symmetry.
     * @param idx : the index of the symmetry.
     * @param b   : the result cube reference.
     */
    void edgeConjugate(const CubieCube &a, int8_t idx, CubieCube &b);

    /**
     * This function is used to get the permutation from the symmetry.
     *
     * @param idx       : the symmetry index.
     * @param sym       : the symmetry used.
     * @param isCorner  : true if the permutation is a corner, false if not.
     * @return          : the permutation value.
     */
    uint16_t getPermSymInv(uint16_t idx, int8_t sym, bool isCorner);

    /**
     * This function is used to check if two cube are the same. It compares every
     * corner or edge from the two array.
     *
     * @param cube1  : the array containing the edge or corner of the first cube.
     * @param cube2  : the array containing the edge or corner of the second cube.
     * @param isEdge : true if the arrays contains the edge, false if not.
     * @return       : true is the two cubes are the same, false if not.
     */
    bool isSameCube(const int8_t cube1[], const int8_t cube2[], bool isEdge);

    /**
     * The edge perm and corner perm have the same symmetry structure.
     * So their ClassIndexToRepresentantArray (see Kociemba's algorithm) are the same.
     * So, when you want the symmetry of edge permutation you do y*16+k, where
     * x is the edge permutation.
     * Otherwise, if you need the symmetry of corner permutation you do
     * y*16+(k^e2c[k]) where x is the corner orientation.
     * This function do the y*16+(k^e2c[k]).
     *
     * @param idx : the edge permutation symmetry to convert.
     * @return    : the corner permutation symmetry.
     */
    int32_t ESym2CSym(uint16_t idx);

    /**
     * This function is ued to set the pruning in the pruning table.
     *
     * @param table : the table where the pruning will be stored.
     * @param index : the position of the pruning to store.
     * @param value : the value of pruning to store.
     */
    void setPruning(int32_t table[], int32_t index, int8_t value);

    /**
     * This function is used to extract a pruning value from a pruning table.
     *
     * @param table : the pruning table where is stored the value.
     * @param index : the position of the pruning to get.
     * @return      : the pruning value.
     */
    int8_t getPruning(const int32_t table[], int32_t index);

    /**
     * In this class are stored the coordinates and info used to
     * identify every cube.
     */
    class CoordCube {
    private:
        /**
         * This is the edge orientation.
         */
        int16_t twist;

        /**
         * This is the edge orientation symmetry.
         */
        int16_t tsym;

        /**
         * This is the corner orientation.
         */
        int16_t flip;

        /**
         * This is the corner orientation symmetry.
         */
        int16_t fsym;

        /**
         * This is the UDSlice coordinate
         */
        int16_t slice;

        /**
         * This is the edge orientation with a different cube rotation.
         */
        uint16_t twistc;

        /**
         * This is the corner orientation with a different cube rotation.
         */
        uint16_t flipc;

    public:

        /**
         * This is used to stored the moves pruning.
         */
        int8_t prun;

        //TODO
        void calcPrun(bool isPhase1);

        /**
         * This is used to set a cube and a max depth. It also check if the
         * depth is ok or too int16_t.
         *
         * @param cc    : the cube to store.
         * @param depth : the max depth to set.
         * @return      : true if the pruning moves are less then  depth, false if not.
         */
        bool setWithPrun(const CubieCube& cc, int8_t depth);

        /**
         * This is used to apply a move to the pruning.
         *
         * @param cc : the cube used to move.
         * @param m  : the move.
         * @return   : the pruning value.
         */
        int8_t doMovePrun(const CoordCube &cc, int8_t m);

        /**
         * This is used to apply a move to the pruning for the UDSliceSorted.
         *
         * @param cc : the cube used to move.
         * @param m  : the move.
         * @return   : the pruning value.
         */
        int8_t doMovePrunConj(const CoordCube &cc, int8_t m);
    };
} }

#endif //MIN2PHASE_COORDS_H