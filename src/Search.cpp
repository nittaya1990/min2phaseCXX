#include "Search.h"

//configure object for solving
std::string min2phase::Search::solve(const std::string& facelets, int8_t maxDepth, int probeMax, int probeMin, int8_t verbose) {
    int8_t error = verify(facelets);

    if (error != 0)
        return std::to_string((int)error);

    this->solLen = maxDepth+1;
    this->probe = 0;
    this->probeMax = probeMax;
    this->probeMin = probeMin;
    this->verbose = verbose;

    initSearch();

    return  search();
}

int8_t min2phase::Search::verify(const std::string& facelets) {
    char centers[CubeInfo::FACES];
    int8_t cube [CubeInfo::N_PLATES];
    int count = 0x000000;
    int8_t i, j, len;
    bool notQuit;

    for(len = 0; facelets[len] != '\0'; len++);

    if(len != CubeInfo::N_PLATES)
        return CubeInfo::MALFORMED_STRING;

    centers[0] = facelets[CubeInfo::U5];
    centers[1] = facelets[CubeInfo::R5];
    centers[2] = facelets[CubeInfo::F5];
    centers[3] = facelets[CubeInfo::D5];
    centers[4] = facelets[CubeInfo::L5];
    centers[5] = facelets[CubeInfo::B5];


    for (i = 0; i < CubeInfo::N_PLATES; i++) {
        cube[i] = -1;
        notQuit = true;

        for(j = 0; notQuit && j < CubeInfo::FACES; j++){
            if(centers[j] == facelets[i]){
                notQuit = false;
                cube[i] = j;
            }
        }

        if (cube[i] == -1)
            return CubeInfo::MALFORMED_STRING;

        count += 1 << (cube[i] << 2);
    }

    if (count != 0x999999)
        return CubeInfo::MALFORMED_STRING;

    CubieCube::toCubieCube(cube, solveCube);

    return solveCube.verify();
}

void min2phase::Search::initSearch() {
    int8_t i;
    selfSym = solveCube.selfSymmetry();
    conjMask |= (selfSym >> 16 & 0xffff) != 0 ? 0x12 : 0;
    conjMask |= (selfSym >> 32 & 0xffff) != 0 ? 0x24 : 0;
    conjMask |= (selfSym >> 48 & 0xffff) != 0 ? 0x38 : 0;
    selfSym &= 0xffffffffffffL;
    maxPreMoves = conjMask > 7 ? 0 : MAX_PRE_MOVES;

    for (i = 0; i < CubeInfo::N_BASIC_MOVES; i++) {
        urfCubieCube[i].copy(solveCube);
        urfCoordCube[i].setWithPrun(urfCubieCube[i], 20);
        solveCube.URFConjugate();

        if (i%3 == 2)
            solveCube.invCubieCube();
    }
}

std::string min2phase::Search::search() {
    std::string errorCase;

    for (length1 = 0; length1 < solLen; length1++) {
        maxDep2 = std::min((int)CubeInfo::PHASE1_MOVES, solLen-length1-1);

        for (urfIdx =  0; urfIdx < CubeInfo::N_BASIC_MOVES; urfIdx++) {

            if ((conjMask & 1 << urfIdx) != 0)
                continue;

            if (phase1PreMoves(maxPreMoves, -30, &urfCubieCube[urfIdx], (int)(selfSym & 0xffff)) == 0){
                if(!solution.isFound){
                    errorCase = std::to_string(CubeInfo::PROBE_LIMIT);
                    return errorCase;
                }

                return solution.toString();
            }
        }
    }

    if(!solution.isFound){
        errorCase = std::to_string(CubeInfo::SHORT_DEPTH);
        return errorCase;
    }

    return solution.toString();
}

int8_t min2phase::Search::phase1PreMoves(int8_t maxl, int8_t lm, CubieCube* cc, unsigned short ssym) {
    int8_t m, ret;
    int skipMoves;

    preMoveLen = maxPreMoves-maxl;

    if (preMoveLen == 0 || (0x36FB7 >> lm & 1) == 0) {
        depth1 = length1-preMoveLen;
        phase1Cubie[0] = *cc;
        allowShorter = depth1 == MIN_P1LENGTH_PRE && preMoveLen != 0;

        if (nodeUD[depth1+1].setWithPrun(*cc, depth1)
            && phase1(&nodeUD[depth1+1], ssym, depth1, -1) == 0)
            return 0;
    }

    if (maxl == 0 || preMoveLen+MIN_P1LENGTH_PRE >= length1)
        return 1;

    if (maxl == 1 || preMoveLen+1+MIN_P1LENGTH_PRE >= length1)
        skipMoves = 0x36FB7;
    else
        skipMoves = 0;

    lm = lm / 3 * 3;

    for (m = 0; m < CubeInfo::PHASE2_MOVES; m++) {
        if (m == lm || m == lm - 9 || m == lm + 9) {
            m += 2;
            continue;
        }

        if ((skipMoves & 1 << m) != 0)
            continue;

        CubieCube::CornMult(Coords::moveCube[m], *cc, preMoveCubes[maxl]);
        CubieCube::EdgeMult(Coords::moveCube[m], *cc, preMoveCubes[maxl]);
        preMoves[maxPreMoves - maxl] = m;
        ret = phase1PreMoves(maxl - 1, m, &preMoveCubes[maxl], ssym & (int) Coords::moveCubeSym[m]);

        if (ret == 0)
            return 0;
    }

    return 1;
}

int8_t min2phase::Search::phase1(Coords::CoordCube* node, unsigned short ssym, int8_t maxl, int8_t lm) {
    int8_t ret, axis, m, power, prun;

    if (node->prun == 0 && maxl < 5) {
        if (allowShorter || maxl == 0) {
            depth1 -= maxl;
            ret = initPhase2Pre();
            depth1 += maxl;
            return ret;
        }else
            return 1;
    }

    for (axis = 0; axis < CubeInfo::PHASE2_MOVES; axis += 3) {
        if (axis == lm || axis == lm - 9)
            continue;

        for (power = 0; power < 3; power++) {
            m = axis + power;

            prun = nodeUD[maxl].doMovePrun(*node, m);

            if (prun > maxl)
                break;
            else if (prun == maxl)
                continue;

            prun = nodeUD[maxl].doMovePrunConj(*node, m);
            if (prun > maxl)
                break;
            else if (prun == maxl)
                continue;

            move[depth1 - maxl] = m;
            valid1 = std::min(valid1, int8_t(depth1 - maxl));
            ret = phase1(&nodeUD[maxl], ssym & (int) Coords::moveCubeSym[m], maxl - 1, axis);

            if (ret == 0)
                return 0;
            else if (ret >= 2)
                break;
        }
    }

    return 1;
}

int8_t min2phase::Search::initPhase2Pre() {
    unsigned short p2corn, p2edge, edgei, corni;
    int8_t  p2csym, p2esym, p2mid, lastMove, lastPre, p2switchMax, p2switchMask;
    int8_t p2switch, ret;
    int8_t i, m;

    if (probe >= (!solution.isFound ? probeMax : probeMin))
        return 0;

    ++probe;

    for (i = valid1; i < depth1; i++) {
        CubieCube::CornMult(phase1Cubie[i], Coords::moveCube[move[i]], phase1Cubie[i + 1]);
        CubieCube::EdgeMult(phase1Cubie[i], Coords::moveCube[move[i]], phase1Cubie[i + 1]);
    }

    valid1 = depth1;

    p2corn = phase1Cubie[depth1].getCPermSym();
    p2csym = p2corn & 0xf;
    p2corn >>= 4;

    p2edge = phase1Cubie[depth1].getEPermSym();
    p2esym = p2edge & 0xf;
    p2edge >>= 4;

    p2mid = phase1Cubie[depth1].getMPerm();
    edgei = Coords::getPermSymInv(p2edge, p2esym, false);
    corni = Coords::getPermSymInv(p2corn, p2csym, true);

    lastMove = depth1 == 0 ? -1 : move[depth1 - 1];
    lastPre = preMoveLen == 0 ? -1 : preMoves[preMoveLen - 1];

    ret = 0;
    p2switchMax = (preMoveLen == 0 ? 1 : 2) * (depth1 == 0 ? 1 : 2);
    p2switchMask = (1 << p2switchMax)-1;

    for (p2switch = 0; p2switch < p2switchMax; p2switch++) {

        if ((p2switchMask >> p2switch & 1) != 0) {
            p2switchMask &= ~(1 << p2switch);
            ret = initPhase2(p2corn, p2csym, p2edge, p2esym, p2mid, edgei, corni);

            if (ret == 0 || ret > 2)
                break;
            else if (ret == 2)
                p2switchMask &= 0x4 << p2switch;
        }

        if (p2switchMask == 0)
            break;

        if ((p2switch & 1) == 0 && depth1 > 0) {
            m = CubeInfo::std2ud[lastMove / 3 * 3 + 1];
            move[depth1 - 1] = CubeInfo::ud2std[m] * 2 - move[depth1 - 1];

            p2mid = Coords::MPermMove[p2mid][m];
            p2corn = Coords::CPermMove[p2corn][Coords::SymMoveUD[p2csym][m]];
            p2csym = Coords::SymMult[p2corn & 0xf][p2csym];
            p2corn >>= 4;
            p2edge = Coords::EPermMove[p2edge][Coords::SymMoveUD[p2esym][m]];
            p2esym = Coords::SymMult[p2edge & 0xf][p2esym];
            p2edge >>= 4;
            corni = Coords::getPermSymInv(p2corn, p2csym, true);
            edgei = Coords::getPermSymInv(p2edge, p2esym, false);

        } else if (preMoveLen > 0) {
            m = CubeInfo::std2ud[lastPre / 3 * 3 + 1];
            preMoves[preMoveLen - 1] = CubeInfo::ud2std[m] * 2 - preMoves[preMoveLen - 1];

            p2mid = Coords::MPermInv[Coords::MPermMove[Coords::MPermInv[p2mid]][m]];
            p2corn = Coords::CPermMove[corni >> 4][Coords::SymMoveUD[corni & 0xf][m]];
            corni = p2corn & ~0xf | Coords::SymMult[p2corn & 0xf][corni & 0xf];
            p2corn = Coords::getPermSymInv(corni >> 4, corni & 0xf, true);
            p2csym = p2corn & 0xf;
            p2corn >>= 4;
            p2edge = Coords::EPermMove[edgei >> 4][Coords::SymMoveUD[edgei & 0xf][m]];
            edgei = p2edge & ~0xf | Coords::SymMult[p2edge & 0xf][edgei & 0xf];
            p2edge = Coords::getPermSymInv(edgei >> 4, edgei & 0xf, false);
            p2esym = p2edge & 0xf;
            p2edge >>= 4;
        }
    }

    if (depth1 > 0)
        move[depth1 - 1] = lastMove;

    if (preMoveLen > 0)
        preMoves[preMoveLen - 1] = lastPre;

    return ret == 0 ? 0 : 2;
}

int8_t min2phase::Search::initPhase2(unsigned short p2corn, int8_t p2csym, unsigned short p2edge, int8_t p2esym, int8_t p2mid, unsigned short edgei, unsigned short corni) {
    int8_t prun, depth2, i;
    int8_t ret;

    prun = std::max(
            Coords::getPruning(Coords::EPermCCombPPrun,
                                         (edgei >> 4) * CubeInfo::N_COMB + Coords::CCombPConj[Coords::Perm2CombP[corni >> 4] & 0xff][Coords::SymMultInv[edgei & 0xf][corni & 0xf]]),
            std::max(
                    Coords::getPruning(Coords::EPermCCombPPrun,
                                                 p2edge * CubeInfo::N_COMB + Coords::CCombPConj[Coords::Perm2CombP[p2corn] & 0xff][Coords::SymMultInv[p2esym][p2csym]]),
                    Coords::getPruning(Coords::MCPermPrun,
                                                 p2corn * CubeInfo::N_MPERM + Coords::MPermConj[p2mid][p2csym])));

    if (prun > maxDep2)
        return prun - maxDep2;

    for (depth2 = maxDep2; depth2 >= prun; depth2--) {
        ret = phase2(p2edge, p2esym, p2corn, p2csym, p2mid, depth2, depth1, 10);

        if (ret < 0)
            break;

        depth2 -= ret;
        solLen = 0;

        solution.isFound = true;
        solution.reset();
        solution.setArgs(verbose, urfIdx, depth1);

        for (i = 0; i < depth1 + depth2; i++)
            solution.appendSolMove(move[i]);

        for (i = preMoveLen - 1; i >= 0; i--)
            solution.appendSolMove(preMoves[i]);

        solLen = solution.length;
    }

    if (depth2 != maxDep2) {
        maxDep2 = std::min(int8_t (CubeInfo::PHASE2_MOVES), int8_t (solLen - length1 - 1));
        return probe >= probeMin ? 0 : 1;
    }

    return 1;
}

int8_t min2phase::Search::phase2(unsigned short edge, int8_t esym, unsigned short corn, int8_t csym, int8_t mid, int8_t maxl, int8_t depth, int8_t lm) {
    unsigned short cornx, edgex, edgei, corni;
    int8_t midx, csymx, prun;
    short moveMask;
    int8_t m, ret;

    if (edge == 0 && corn == 0 && mid == 0)
        return maxl;

    moveMask = CubeInfo::ckmv2bit[lm];

    for (m = 0; m < 10; m++) {

        if ((moveMask >> m & 1) != 0) {
            m += 0x42 >> m & 3;
            continue;
        }

        midx = Coords::MPermMove[mid][m];
        cornx = Coords::CPermMove[corn][Coords::SymMoveUD[csym][m]];
        csymx = Coords::SymMult[cornx & 0xf][csym];
        cornx >>= 4;
        edgex = Coords::EPermMove[edge][Coords::SymMoveUD[esym][m]];
        int8_t esymx = Coords::SymMult[edgex & 0xf][esym];
        edgex >>= 4;
        edgei = Coords::getPermSymInv(edgex, esymx, false);
        corni = Coords::getPermSymInv(cornx, csymx, true);

        prun = Coords::getPruning(Coords::EPermCCombPPrun,
                                            (edgei >> 4) * CubeInfo::N_COMB + Coords::CCombPConj[Coords::Perm2CombP[corni >> 4] & 0xff][Coords::SymMultInv[edgei & 0xf][corni & 0xf]]);
        if (prun > maxl + 1)
            return maxl - prun + 1;
        else if (prun >= maxl) {
            m += 0x42 >> m & 3 & (maxl - prun);
            continue;
        }

        prun = std::max(
                Coords::getPruning(Coords::MCPermPrun,
                                             cornx * CubeInfo::N_MPERM + Coords::MPermConj[midx][csymx]),
                Coords::getPruning(Coords::EPermCCombPPrun,
                                             edgex * CubeInfo::N_COMB + Coords::CCombPConj[Coords::Perm2CombP[cornx] & 0xff][Coords::SymMultInv[esymx][csymx]]));

        if(prun >= maxl) {
            m += 0x42 >> m & 3 & (maxl - prun);
            continue;
        }

        ret = phase2(edgex, esymx, cornx, csymx, midx, maxl - 1, depth + 1, m);

        if (ret >= 0) {
            move[depth] = CubeInfo::ud2std[m];
            return ret;
        }

        if (ret < -2)
            break;

        if (ret < -1)
            m += 0x42 >> m & 3;
    }

    return -1;
}