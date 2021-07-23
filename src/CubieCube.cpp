#include "Coords.h"

namespace min2phase {

    /**
     * This is used to indicate if use a dot from the moves
     * used in phase 1 and 2.
     */
    const int8_t USE_SEPARATOR = 1;

    /**
     * This is used to find a scramble that generate the input cube from the
     * solved cube instead of generate a scramble that solve the cube.
     */
    const int8_t INVERSE_SOLUTION = 2;

    /**
     * This is used to add the number of moves used at the end of
     * the string.
     */
    const int8_t APPEND_LENGTH = 4;

    /**
     * This is used to remove the spaces from moves in the output string.
     */
    const int8_t REMOVE_SPACES = 8;

    //set che coordinates of the cube
    void CubieCube::setValues(unsigned short cPerm, short cOri, int ePerm, short eOri) {
        setNPerm(edges, ePerm, CubeInfo::NUMBER_EDGES, true);
        setCPerm(cPerm);
        setTwist(cOri);
        setFlip(eOri);
    }

    //set the cube from an existing one and coords
    void CubieCube::setValues(const CubieCube &cube) {
        copy(cube);
    }

    //copy a cube into the new one
    void CubieCube::copy(const CubieCube &cube) {
        uint8_t i;

        for (i = 0; i < CubeInfo::NUMBER_EDGES; i++) {
            if (i < CubeInfo::NUMBER_CORNER)
                this->corners[i] = cube.corners[i];
            this->edges[i] = cube.edges[i];
        }
    }

    //inverse cube
    void CubieCube::invCubieCube() {
        int8_t edge, corner;
        CubieCube temps;

        for (edge = 0; edge < CubeInfo::NUMBER_EDGES; edge++)
            temps.edges[edges[edge] >> 1] = (edge << 1 | edges[edge] & 1);

        for (corner = 0; corner < CubeInfo::NUMBER_CORNER; corner++)
            temps.corners[corners[corner] & 0x7] = (corner | 0x20 >> (corners[corner] >> 3) & 0x18);

        copy(temps);
    }

    //this = S_urf^-1 * this * S_urf
    void CubieCube::URFConjugate() {
        CubieCube temps;

        CornMult(Coords::urfInv, *this, temps);
        CornMult(temps, Coords::urf, *this);
        EdgeMult(Coords::urfInv, *this, temps);
        EdgeMult(temps, Coords::urf, *this);
    }

    long CubieCube::selfSymmetry() const {
        CubieCube c(*this);
        CubieCube d;
        int8_t urfInv;
        uint8_t i;
        unsigned short cpermx, cperm = c.getCPermSym() >> 4;
        long symType = 0L;

        for (urfInv = 0; urfInv < CubeInfo::N_BASIC_MOVES; urfInv++) {
            cpermx = c.getCPermSym() >> 4;

            if (cperm == cpermx) {
                for (i = 0; i < CubeInfo::SYM; i++) {
                    Coords::CornConjugate(c, Coords::SymMultInv[0][i], d);

                    if (Coords::isSameCube(d.corners, corners, false)) {
                        Coords::EdgeConjugate(c, Coords::SymMultInv[0][i], d);

                        if (Coords::isSameCube(d.edges, edges, true))
                            symType |= 1L << std::min(int8_t(urfInv << 4 | i), int8_t(CubeInfo::FULL_SYM));
                    }
                }
            }

            c.URFConjugate();

            if (urfInv % 3 == 2)
                c.invCubieCube();
        }

        return symType;
    }

    //check if the cube is possible
    int8_t CubieCube::verify() const {
        uint8_t i;
        unsigned short sum = 0;
        unsigned short mask = 0;

        for (i = 0; i < CubeInfo::NUMBER_EDGES; i++) {
            mask |= 1 << (edges[i] >> 1);
            sum ^= edges[i] & 1;
        }

        if (mask != 0xfff)
            return CubeInfo::MISSING_EDGE;

        if (sum != 0)
            return CubeInfo::TWISTED_EDGE;

        mask = 0;
        sum = 0;

        for (i = 0; i < CubeInfo::NUMBER_CORNER; i++) {
            mask |= 1 << (corners[i] & (CubeInfo::NUMBER_CORNER - 1));
            sum += corners[i] >> CubeInfo::CORNE_ORI_CASES;
        }

        if (mask != 0xff)
            return CubeInfo::MISSING_CORNER;

        if (sum % CubeInfo::CORNE_ORI_CASES != 0)
            return CubeInfo::TWISTED_CORNER;

        if ((getNParity(getNPerm(edges, CubeInfo::NUMBER_EDGES, true), CubeInfo::NUMBER_EDGES) ^
             getNParity(getCPerm(), CubeInfo::NUMBER_CORNER)) != 0)
            return CubeInfo::PARITY_ERROR;

        return CubeInfo::NO_ERROR;
    }

    //convert the array of colors into a cube object
    void CubieCube::toCubieCube(const int8_t f[], CubieCube &ccRet) {
        int8_t ori, i, j, col1, col2;

        for (i = 0; i < CubeInfo::NUMBER_CORNER; i++)
            ccRet.corners[i] = 0;

        for (i = 0; i < CubeInfo::NUMBER_EDGES; i++)
            ccRet.edges[i] = 0;

        for (i = 0; i < CubeInfo::NUMBER_CORNER; i++) {
            for (ori = 0; ori < CubeInfo::CORNE_ORI_CASES; ori++)
                if (f[CubeInfo::cornerFacelet[i][ori]] == CubeInfo::U ||
                    f[CubeInfo::cornerFacelet[i][ori]] == CubeInfo::D)
                    break;

            col1 = f[CubeInfo::cornerFacelet[i][(ori + 1) % CubeInfo::CORNE_ORI_CASES]];
            col2 = f[CubeInfo::cornerFacelet[i][(ori + 2) % CubeInfo::CORNE_ORI_CASES]];

            for (j = 0; j < CubeInfo::NUMBER_CORNER; j++) {
                if (col1 == CubeInfo::cornerFacelet[j][1] / 9 && col2 == CubeInfo::cornerFacelet[j][2] / 9) {
                    ccRet.corners[i] = (int8_t)(ori % CubeInfo::CORNE_ORI_CASES << CubeInfo::CORNE_ORI_CASES | j);
                    break;
                }
            }
        }

        for (i = 0; i < CubeInfo::NUMBER_EDGES; i++) {
            for (j = 0; j < CubeInfo::NUMBER_EDGES; j++) {
                if (f[CubeInfo::edgeFacelet[i][0]] == CubeInfo::edgeFacelet[j][0] / 9
                    && f[CubeInfo::edgeFacelet[i][1]] == CubeInfo::edgeFacelet[j][1] / 9) {
                    ccRet.edges[i] = j << 1;
                    break;
                }

                if (f[CubeInfo::edgeFacelet[i][0]] == CubeInfo::edgeFacelet[j][1] / 9
                    && f[CubeInfo::edgeFacelet[i][1]] == CubeInfo::edgeFacelet[j][0] / 9) {
                    ccRet.edges[i] = j << 1 | 1;
                    break;
                }
            }
        }
    }

    //from cube to string
    std::string CubieCube::toFaceCube(CubieCube cc) {
        int8_t i;
        int8_t j, ori;
        std::string f;

        f.reserve(CubeInfo::N_PLATES + 1);

        char ts[] = {'U', 'R', 'F', 'D', 'L', 'B'};

        for (i = 0; i < CubeInfo::N_PLATES; i++)
            f[i] = ts[i / CubeInfo::N_PLATES_X_FACE];

        for (i = 0; i < CubeInfo::NUMBER_CORNER; i++) {
            j = cc.corners[i] & 0x7;
            ori = cc.corners[i] >> 3;
            for (int8_t n = 0; n < CubeInfo::CORNE_ORI_CASES; n++) {
                f[CubeInfo::cornerFacelet[i][(n + ori) % CubeInfo::CORNE_ORI_CASES]] = ts[
                        CubeInfo::cornerFacelet[j][n] / CubeInfo::N_PLATES_X_FACE];
            }
        }

        for (i = 0; i < CubeInfo::NUMBER_EDGES; i++) {
            j = cc.edges[i] >> 1;
            ori = cc.edges[i] & 1;
            for (int8_t n = 0; n < CubeInfo::EDGE_ORI_CASES; n++) {
                f[CubeInfo::edgeFacelet[i][(n + ori) % CubeInfo::EDGE_ORI_CASES]] = ts[CubeInfo::edgeFacelet[j][n] /
                                                                                       CubeInfo::N_PLATES_X_FACE];
            }
        }

        f[CubeInfo::N_PLATES] = '\0';
        return f;
    }

    // a * b edge only
    void CubieCube::EdgeMult(const CubieCube &a, const CubieCube &b, CubieCube &prod) {
        for (int8_t edge = 0; edge < CubeInfo::NUMBER_EDGES; edge++)
            prod.edges[edge] = a.edges[b.edges[edge] >> 1] ^ (b.edges[edge] & 1);
    }

    //a * b corner only
    void CubieCube::CornMult(const CubieCube &a, const CubieCube &b, CubieCube &prod) {
        int8_t oriA, oriB, corn;

        for (corn = 0; corn < CubeInfo::NUMBER_CORNER; corn++) {
            oriA = a.corners[b.corners[corn] & 7] >> 3;
            oriB = b.corners[corn] >> 3;
            prod.corners[corn] = a.corners[b.corners[corn] & 7] & 7 | (oriA + oriB) % 3 << 3;
        }
    }

    //a * b corner with mirrored cases
    void CubieCube::CornMultFull(const CubieCube &a, const CubieCube &b, CubieCube &prod) {
        int8_t oriA, oriB, corn, ori;

        for (corn = 0; corn < CubeInfo::NUMBER_CORNER; corn++) {
            oriA = a.corners[b.corners[corn] & 7] >> 3;
            oriB = b.corners[corn] >> 3;
            ori = oriA + ((oriA < 3) ? oriB : 6 - oriB);
            ori = ori % 3 + ((oriA < 3) == (oriB < 3) ? 0 : 3);
            prod.corners[corn] = a.corners[b.corners[corn] & 7] & 7 | ori << 3;
        }
    }

    //get edge or corner from value
    int8_t CubieCube::getVal(int8_t val0, bool isEdge) {
        return isEdge ? val0 >> 1 : val0 & 7;
    }

    //get edge or corner into value
    int8_t CubieCube::setVal(int8_t val0, int8_t val, bool isEdge) {
        return (isEdge ? val << 1 | (val0 & 1) : val | (val0 & ~7));

    }

    //set permutation
    void CubieCube::setNPerm(int8_t arr[], int idx, uint8_t n, bool isEdge) {
        uint8_t i;
        unsigned long val = 0xFEDCBA9876543210L;
        uint8_t v;
        long m, extract = 0;

        for (i = 2; i <= n; i++) {
            extract = extract << 4 | idx % i;
            idx /= i;
        }

        for (i = 0; i < n - 1; i++) {
            v = (extract & 0xf) << 2;
            extract >>= 4;
            arr[i] = setVal(arr[i], val >> v & 0xf, isEdge);
            m = (1L << v) - 1;
            val = (val & m) | (val >> 4 & ~m);
        }

        arr[n - 1] = setVal(arr[n - 1], val & 0xf, isEdge);
    }

    //get permutation
    int CubieCube::getNPerm(const int8_t arr[], uint8_t n, bool isEdge) {
        uint8_t i, v;
        int idx = 0;
        unsigned long val = 0xFEDCBA9876543210L;

        for (i = 0; i < n - 1; i++) {
            v = getVal(arr[i], isEdge) << 2;
            idx = (n - i) * idx + (val >> v & 0xf);
            val -= 0x1111111111111110L << v;
        }

        return idx;
    }

    //set the parity of corners and edges
    void CubieCube::setComb(int8_t arr[], short idxC, uint8_t mask, bool isEdge) {
        int8_t fill = isEdge ? CubeInfo::NUMBER_EDGES - 1 : CubeInfo::NUMBER_CORNER - 1;
        int8_t i;
        int8_t r = 4;

        for (i = fill; i >= 0; i--) {
            if (idxC >= CubeInfo::Cnk[i][r]) {
                idxC -= CubeInfo::Cnk[i][r--];
                arr[i] = setVal(arr[i], r | mask, isEdge);
            } else {
                if ((fill & 0xc) == mask)
                    fill -= 4;

                arr[i] = setVal(arr[i], fill--, isEdge);
            }
        }
    }

    //get the parity of corners or edges
    short CubieCube::getComb(const int8_t arr[], uint8_t mask, bool isEdge) {
        int8_t i, perm, r = 4;
        short idxC = 0;

        for (i = isEdge ? CubeInfo::NUMBER_EDGES - 1 : CubeInfo::NUMBER_CORNER - 1; i >= 0; i--) {
            perm = getVal(arr[i], isEdge);

            if ((perm & 0xc) == mask)
                idxC += CubeInfo::Cnk[i][r--];
        }

        return idxC;
    }

    //get the parity permutation
    int8_t CubieCube::getNParity(unsigned int idx, uint8_t n) {
        int8_t i;
        int8_t p = 0;

        for (i = n - 2; i >= 0; i--) {
            p ^= idx % (n - i);
            idx /= (n - i);
        }

        return p & 1;
    }

    //phase 1 coords

    //get edge orientation
    short CubieCube::getFlip() const {
        short eOri = 0;
        uint8_t i;

        for (i = 0; i < CubeInfo::NUMBER_EDGES - 1; i++)
            eOri = eOri << 1 | edges[i] & 1;

        return eOri;
    }

    //set edge orientation
    void CubieCube::setFlip(short eOri) {
        int8_t parity = 0;
        short val;
        int8_t i;

        for (i = CubeInfo::NUMBER_EDGES - 2; i >= 0; i--, eOri >>= 1) {
            parity ^= (val = eOri & 1);
            edges[i] = (int8_t)(edges[i] & ~1 | val);
        }

        edges[CubeInfo::NUMBER_EDGES - 1] = (int8_t)(edges[CubeInfo::NUMBER_EDGES - 1] & ~1 | parity);
    }

    //get edge orientation symmetry
    short CubieCube::getFlipSym() const {
        return Coords::FlipR2S[getFlip()];
    }

    //get corner orientation
    short CubieCube::getTwist() const {
        short cOri = 0;
        uint8_t i;

        for (i = 0; i < CubeInfo::NUMBER_CORNER - 1; i++)
            cOri += (cOri << 1) + (corners[i] >> CubeInfo::CORNE_ORI_CASES);

        return cOri;
    }

    //set corner orientation
    void CubieCube::setTwist(short cOri) {
        int8_t i;
        int8_t ori = 15, val;

        for (i = CubeInfo::NUMBER_CORNER - 2; i >= 0; i--, cOri /= CubeInfo::CORNE_ORI_CASES) {
            ori -= (val = cOri % CubeInfo::CORNE_ORI_CASES);
            corners[i] = corners[i] & 0x7 | val << CubeInfo::CORNE_ORI_CASES;
        }

        corners[CubeInfo::NUMBER_CORNER - 1] = (corners[CubeInfo::NUMBER_CORNER - 1] & 0x7) |
                                               ((ori % CubeInfo::CORNE_ORI_CASES) << CubeInfo::CORNE_ORI_CASES);
    }

    //get corner orientation symmetry
    short CubieCube::getTwistSym() const {
        return Coords::TwistR2S[getTwist()];
    }

    //get udslice coord
    short CubieCube::getUDSlice() const {
        return CubeInfo::N_SLICE - 1 - getComb(edges, CubeInfo::NUMBER_CORNER, true);
    }

    //set udslice coord
    void CubieCube::setUDSlice(short udSlice) {
        setComb(edges, CubeInfo::N_SLICE - 1 - udSlice, CubeInfo::NUMBER_CORNER, true);
    }

    //phase 2 coords

    //get corner permutation
    unsigned short CubieCube::getCPerm() const {
        return getNPerm(corners, CubeInfo::NUMBER_CORNER, false);
    }

    //set corner permutation
    void CubieCube::setCPerm(unsigned short perm) {
        setNPerm(corners, perm, CubeInfo::NUMBER_CORNER, false);
    }

    //get corner permutation symmetry
    unsigned short CubieCube::getCPermSym() const {
        return Coords::ESym2CSym(Coords::EPermR2S[getCPerm()]);
    }

    //get edge permutation
    unsigned short CubieCube::getEPerm() const {
        return getNPerm(edges, CubeInfo::NUMBER_CORNER, true);
    }

    //set edge permutation
    void CubieCube::setEPerm(unsigned short idx) {
        setNPerm(edges, idx, CubeInfo::NUMBER_EDGES - 4, true);
    }

    //get edge permutation symmetry
    unsigned short CubieCube::getEPermSym() const {
        return Coords::EPermR2S[getEPerm()];
    }

    //get udslicesorted permutation
    int8_t CubieCube::getMPerm() const {
        return getNPerm(edges, CubeInfo::NUMBER_EDGES, true) % CubeInfo::N_MPERM;
    }

    //set udslicesorted permutation
    void CubieCube::setMPerm(int8_t idx) {
        setNPerm(edges, idx, CubeInfo::NUMBER_EDGES, true);
    }

    //get corner symmetry
    uint8_t CubieCube::getCComb() const {
        return getComb(corners, 0, false);
    }

    //set corner symmetry
    void CubieCube::setCComb(uint8_t idx) {
        setComb(corners, idx, 0, false);
    }

    //index to string
    std::string CubieCube::OutputFormat::move2str[] = {
            "U ", "U2", "U'", "R ", "R2", "R'", "F ", "F2", "F'",
            "D ", "D2", "D'", "L ", "L2", "L'", "B ", "B2", "B'"
    };

    //reset output
    void CubieCube::OutputFormat::reset() {
        format = 0;
        urfIdx = 0;
        depth1 = 0;
        length = 0;
    }

    //set data
    void CubieCube::OutputFormat::setArgs(int8_t format, int8_t urfIdx, int8_t depth1) {
        this->format = format;
        this->urfIdx = urfIdx;
        this->depth1 = depth1;
    }

    //add move
    void CubieCube::OutputFormat::appendSolMove(int8_t curMove) {
        int8_t axisCur, axisLast, pow;

        if (length == 0) {
            moves[length++] = curMove;
            return;
        }

        axisCur = curMove / CubeInfo::N_GROUP_MOVES;
        axisLast = moves[length - 1] / CubeInfo::N_GROUP_MOVES;

        if (axisCur == axisLast) {
            pow = (curMove % CubeInfo::N_GROUP_MOVES + moves[length - 1] % CubeInfo::N_GROUP_MOVES + 1) % 4;

            if (pow == CubeInfo::N_GROUP_MOVES)
                length--;
            else
                moves[length - 1] = axisCur * CubeInfo::N_GROUP_MOVES + pow;

            return;
        }

        if (length > 1 && axisCur % CubeInfo::N_GROUP_MOVES == axisLast % CubeInfo::N_GROUP_MOVES &&
            axisCur == moves[length - 2] / CubeInfo::N_GROUP_MOVES) {
            pow = (curMove % CubeInfo::N_GROUP_MOVES + moves[length - 2] % CubeInfo::N_GROUP_MOVES + 1) % 4;

            if (pow == CubeInfo::N_GROUP_MOVES) {
                moves[length - 2] = moves[length - 1];
                length--;
            } else
                moves[length - 2] = axisCur * CubeInfo::N_GROUP_MOVES + pow;

            return;
        }

        moves[length++] = curMove;
    }

    //compute the string
    std::string CubieCube::OutputFormat::toString() {
        std::string solution;
        int8_t urf = (format & INVERSE_SOLUTION) != 0 ? (urfIdx + CubeInfo::N_GROUP_MOVES) % 6 : urfIdx;
        const bool useInv = urf < CubeInfo::N_GROUP_MOVES;
        int8_t s;

        solution = "";

        for (s = useInv ? 0 : length - 1; useInv ? s < length : s >= 0; useInv ? s++ : s--) {
            if (useInv && s == depth1 && (format & USE_SEPARATOR) != 0) {
                solution += '.';

                if ((format & REMOVE_SPACES) == 0)
                    solution += ' ';
            }

            solution += move2str[CubeInfo::urfMove[urf][moves[s]]];

            if ((format & REMOVE_SPACES) == 0)
                solution += ' ';

            if (!useInv && (format & USE_SEPARATOR) != 0 && s == depth1) {
                solution += '.';

                if ((format & REMOVE_SPACES) == 0)
                    solution += ' ';
            }
        }

        if ((format & APPEND_LENGTH) != 0)
            solution += '(' + std::to_string(length) + "f)";

        return solution;
    }
}