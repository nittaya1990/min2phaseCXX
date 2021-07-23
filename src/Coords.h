#ifndef MIN2PHASE_COORDS_H
#define MIN2PHASE_COORDS_H 1

#include "CubieCube.h"

/**
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

namespace min2phase { namespace Coords{

    typedef enum : uint8_t {
        MCPP_IDX,
        PCPP_IDX,
        STP_IDX,
        SFP_IDX,
        TFP_IDX,
        FLIP_TYPE,
        TWIST_TYPE,
        EPERM_TYPE
    }CoordType;

    /**
     * In this array are stored the cubes result from each possible move
     * from a solved cube. The motion of the corner and edges moved from
     * one of the 18 move is constant. So, we can use basic cubes with the
     * scramble generated from the 18 moves of the cube. In this way,
     * whe can make a multiplication of a cube and a basic move to get
     * a new cube. This increase the speed of the algorithm.
     */
    extern CubieCube moveCube[CubeInfo::N_MOVES];

    //symmetry
    /**
     * In this array are stored the symmetries generated by S_F2, S_U4
     * and S_LR2. To increase the speed of algorithm, we use the symmetries.
     * You can read the complete definition in the Kociemba's page.
     * If you apply F to the cube x1 and R to x2, the two cube are the same
     * because if you rotate x1, you get x2. This increase the speed of the algorithm.
     */
    extern CubieCube CubeSym[CubeInfo::SYM];

    extern long moveCubeSym[CubeInfo::N_MOVES];//TODO

    /**
     * This matrix is used to convert symmetry index into one symmetry for the CubeSym.
     */
    extern int8_t SymMult[CubeInfo::SYM][CubeInfo::SYM];

    /**
     * This matrix is used to convert Symmetry for CubeSym into symmetry index.
     */
    extern int8_t SymMultInv[CubeInfo::SYM][CubeInfo::SYM];

    extern int8_t SymMove[CubeInfo::SYM][CubeInfo::N_MOVES];//TODO
    extern int8_t Sym8Move[8 * CubeInfo::N_MOVES];//TODO
    extern int8_t SymMoveUD[CubeInfo::SYM][CubeInfo::N_MOVES];//TODO

    /**
     * This is used to convert the middle slice perm.
     */
    extern int8_t MPermInv[CubeInfo::N_MPERM];

    /**
     * This is used to convert the permutation into corner comb.
     */
    extern int8_t Perm2CombP[CubeInfo::N_PERM_SYM];

    /**
     * This is used to convert a symmetry index into a permutation of edges or corner.
     */
    extern unsigned short PermInvEdgeSym[CubeInfo::N_PERM_SYM];


    /**
     * Raw-Coordinate to Sym-Coordinate.
     * This type of coordinate are used to convert a normal coordinate of the
     * cube into his symmetry. We can compute the corresponding symmetry of a
     * coordinate every time but it's too slow.
     */

    /**
     * This is used to converts the edge or corner perm into the symmetry.
     * sym = EPermR2S[ edge perm]
     * sym = E2C(EPermR2S[corner perm])
     */
    extern unsigned short EPermR2S[CubeInfo::N_PERM];

    /**
     * This is used to converts the edge orientation into the symmetry.
     * sym = FlipR2S[ edge orientation ]
     */
    extern unsigned short FlipR2S[CubeInfo::N_FLIP];

    /**
     * This is used to converts the corner orientation into the symmetry.
     * sym = TwistR2S[ corner orientation ]
     */
    extern unsigned short TwistR2S[CubeInfo::N_TWIST];

    /**
     * This is used to converts the symmetry into the edge orientation.
     * sym = FlipS2RF[ symmetry ]
     */
    extern unsigned short FlipS2RF[CubeInfo::N_FLIP_SYM * CubeInfo::SYM_CLASSES];

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
    extern unsigned short FlipMove[CubeInfo::N_FLIP_SYM][CubeInfo::N_MOVES];

    /**
     * This matrix contains the moving table for the corner orientation.
     */
    extern unsigned short TwistMove[CubeInfo::N_TWIST_SYM][CubeInfo::N_MOVES];

    /**
     * This matrix contains the moving table for the UDSlice coordinate.
     */
    extern unsigned short UDSliceMove[CubeInfo::N_SLICE][CubeInfo::N_MOVES];

    /**
     * This matrix contains the moving table for the symmetry in phase 1.
     */
    extern unsigned short UDSliceConj[CubeInfo::N_SLICE][CubeInfo::SYM_CLASSES];

    /**
     * This matrix contains the pruning table for the UDsliceTwist coordinate.
     */
    extern int UDSliceTwistPrun[CubeInfo::N_SLICE*CubeInfo::N_TWIST_SYM/CubeInfo::SYM_CLASSES+1];

    /**
     * This matrix contains the pruning table for the UDsliceFlip coordinate.
     */
    extern int UDSliceFlipPrun[CubeInfo::N_SLICE*CubeInfo::N_FLIP_SYM/CubeInfo::SYM_CLASSES+1];

    /**
     * This matrix contains the pruning table for the edge and corner orientation coordinate.
     */
    extern int TwistFlipPrun[CubeInfo::N_FLIP*CubeInfo::N_TWIST_SYM/CubeInfo::SYM_CLASSES+1];

    ///phase 2 coords

    /**
     * This matrix contains the moving table for the corner permutation.
     */
    extern unsigned short CPermMove[CubeInfo::N_PERM_SYM][CubeInfo::N_MOVES2];

    /**
     * This matrix contains the moving table for the edge permutation.
     */
    extern unsigned short EPermMove[CubeInfo::N_PERM_SYM][CubeInfo::N_MOVES2];

    /**
     * This matrix contains the moving table for the UDSliceSorted permutation.
     */
    extern uint8_t MPermMove[CubeInfo::N_MPERM][CubeInfo::N_MOVES2];

    /**
     * This matrix contains the moving table for the corner comb permutation.
     */
    extern uint8_t CCombPMove[CubeInfo::N_COMB][CubeInfo::N_MOVES2];

    /**
     * This matrix contains the moving table for the symmetry of UDSliceSorted.
     */
    extern uint8_t MPermConj[CubeInfo::N_MPERM][CubeInfo::SYM];

    /**
     * This matrix contains the moving table for the symmetry of corner perm in the upper slice.
     */
    extern uint8_t CCombPConj[CubeInfo::N_COMB][CubeInfo::SYM];

    /**
     * This matrix contains the pruning table for the edge permutation coordinate.
     */
    extern int MCPermPrun[CubeInfo::N_MPERM * CubeInfo::N_PERM_SYM/CubeInfo::SYM_CLASSES+1];

    /**
     * This matrix contains the pruning table for the corner permutation coordinate.
     */
    extern int EPermCCombPPrun[CubeInfo::N_COMB * CubeInfo::N_PERM_SYM/CubeInfo::SYM_CLASSES+1];


    /**
     * This the S_URF symmetry that make a rotation of 120 degree of the cube around
     * an axis through the URF-corner and DBL-corner,
     * See Kociemba's algorithm Equivalent Cubes and Symmetry.
     */
    extern CubieCube urf;

    /**
     * This the S_URF^-1 symmetry that make a rotation of -120 degree of the cube around
     * an axis through the URF-corner and DBL-corner,
     * See Kociemba's algorithm Equivalent Cubes and Symmetry.
     */
    extern CubieCube urfInv;

    /**
     * This method is used to compute all the coordinates and symmetries.
     */
    void init();

    /**
     * Check if the coordinates are initialized.
     *
     * @return      : True if initialized, false if not.
     */
    bool isInit();

    /**
     * This method is used to convert the a cube into a new one from a symmetry index.
     * The symmetry is used to find 2 same cube and it increase the speed of the algorithm.
     * b = S_idx^-1 * a * S_idx for only corner. See the Kociemba's algorithm, Equivalent Cubes and Symmetry.
     *
     * @param a   : the cube to apply the symmetry.
     * @param idx : the index of the symmetry.
     * @param b   : the result cube reference.
     */
    void CornConjugate(const CubieCube &a, int8_t idx, CubieCube &b);

    /**
     * This method is used to convert the a cube into a new one from a symmetry index.
     * The symmetry is used to find 2 same cube and it increase the speed of the algorithm.
     * b = S_idx^-1 * a * S_idx for only edges. See the Kociemba's algorithm, Equivalent Cubes and Symmetry.
     *
     * @param a   : the cube to apply the symmetry.
     * @param idx : the index of the symmetry.
     * @param b   : the result cube reference.
     */
    void EdgeConjugate(const CubieCube &a, int8_t idx, CubieCube &b);

    /**
     * This method is used to get the permutation from the symmetry.
     *
     * @param idx       : the symmetry index.
     * @param sym       : the symmetry used.
     * @param isCorner  : true if the permutation is a corner, false if not.
     * @return          : the permutation value.
     */
    unsigned short getPermSymInv(unsigned short idx, int8_t sym, bool isCorner);

    /**
     * This method is used to check if two cube are the same. It compares every
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
     * This method do the y*16+(k^e2c[k]).
     *
     * @param idx : the edge permutation symmetry to convert.
     * @return    : the corner permutation symmetry.
     */
    int ESym2CSym(unsigned short idx);

    /**
     * This method is ued to set the pruning in the pruning table.
     *
     * @param table : the table where the pruning will be stored.
     * @param index : the position of the pruning to store.
     * @param value : the value of pruning to store.
     */
    void setPruning(int table[], int index, int8_t value);

    /**
     * This method is used to extract a pruning value from a pruning table.
     *
     * @param table : the pruning table where is stored the value.
     * @param index : the position of the pruning to get.
     * @return      : the pruning value.
     */
    int8_t getPruning(const int table[], int index);

    /**
     * This method is used to check if a pruning value has the 0 value.
     *
     * @param val : the pruning value to check.
     * @return    : true if has 0, false if not.
     */
    bool hasZero(int val);

    /**
     * This method is used to make a rotation of 3 cubes.
     *
     * @param a : the cube where will stored b.
     * @param b : the cube where will stored c.
     * @param d : the cube where will stored a.
     */
    void rotateCube(CubieCube &a, CubieCube &b, CubieCube &d);

    /**
     * This method compute the cubes generated from the 18 moves and store them in an array.
     */
    void initMove();

    /**
     * This method compute the cubes generated from the combinations of S_F2, S_U4
     * and S_LR2.
     */
    void initSym();

    /**
     * This method is used to initialize the coords transformation from symmetry to
     * raw permutation coordinate.
     * raw coordinate = transformation array[symmetry]
     */
    void initPermSym2Raw();

    /**
     * This method is used to initialize the coords transformation from symmetry to coordinate.
     * raw coordinate = transformation array[symmetry].
     *
     * @param N_RAW    : the size of raw coordinate.
     * @param Sym2Raw  : the array where is stored the convertor from symmetry to raw.
     * @param Raw2Sym  : the array where is stored the convertor from raw to symmetry,
     * @param SymState : the array containing the symmetry of the coordinate.
     * @param coord    : the type of coordinate, flip, twist or edge perm.
     */
    void initSym2Raw(unsigned short N_RAW, unsigned short Sym2Raw[], unsigned short Raw2Sym[], unsigned short SymState[], const CoordType& coord);

    /**
     * This method compute the corner and edge permutation
     * move table table to increase the speed of the algorithm.
     */
    void initPermsMove();

    /**
     * This method compute the UDSliceSorted move table.
     */
    void initMPermMoveConj();

    /**
     * THis method compute the corner comb move table.
     */
    void initCombPMoveConj();

    /**
     * This method compute the edge orientation move table.
     */
    void initFlipMove();

    /**
     * This method is used to compute the corner orientation move table.
     */
    void initTwistMove();

    /**
     * This methos is used to compute the UDSlice move tavble.
     */
    void initUDSliceMoveConj();

    /**
     * This method is used to create a configuration for the pruning compute.
     */
    void initAllPrun();

    /**
     * This method compute the pruning table for every coordinate.
     *
     * @param PrunTable     : the array to store the pruning table.
     * @param RawMove1      : the matrix of raw moves for phase 2
     * @param RawConj1      : the matrix of table for raw symmetry in phase 2.
     * @param SymMoveVect1  : the matrix of raw moves coordinate for phase 2
     * @param RawMove2      : the matrix of raw moves for phase 1.
     * @param RawConj2      : the matrix of table for raw symmetry in phase 1.
     * @param SymMoveVect2  : the matrix of raw moves coordinate for phase 1 .
     * @param SymState      : the array of symmetries.
     * @param PrunFlag      : the flag used to generate the pruning.
     * @param PrunTableSize : the size of the PrunFlag.
     * @param type          : the type of pruning computed, you determinate if from the type of coordinate.
     */
    void initRawSymPrun(int PrunTable[],
     uint8_t RawMove1[][CubeInfo::N_MOVES2], uint8_t RawConj1[][CubeInfo::SYM], unsigned short SymMoveVect1[][CubeInfo::N_MOVES2],
     unsigned short RawMove2[][CubeInfo::N_MOVES], unsigned short RawConj2[][CubeInfo::SYM_CLASSES], unsigned short SymMoveVect2[][CubeInfo::N_MOVES],
     const unsigned short SymState[], int PrunFlag, int PrunTableSize, const CoordType& type);


    /**
     * In this class are stored the coordinates and info used to
     * identify every cube.
     */
    class CoordCube {
    private:
        /**
         * This is the edge orientation.
         */
        short twist;

        /**
         * This is the edge orientation symmetry.
         */
        short tsym;

        /**
         * This is the corner orientation.
         */
        short flip;

        /**
         * This is the corner orientation symmetry.
         */
        short fsym;

        /**
         * This is the UDSlice coordinate
         */
        short slice;

        /**
         * This is the edge orientation with a different cube rotation.
         */
        unsigned short twistc;

        /**
         * This is the corner orientation with a different cube rotation.
         */
        unsigned short flipc;

    public:

        /**
         * This is used to stored the moves pruning.
         */
        int8_t prun;

        /**
         * This is used to set a cube and a max depth. It also check if the
         * depth is ok or too short.
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