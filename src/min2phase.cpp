#include <min2phase/min2phase.h>
#include "Search.h"

namespace min2phase {

    void init() {
        info::init();
        coords::init();
    }

    //solve the cube
    std::string solve(const std::string &facelets, int8_t maxDepth, int32_t probeMax, int32_t probeMin,
                                     int8_t verbose, uint8_t* usedMoves) {

        return Search().solve(facelets, maxDepth, probeMax, probeMin, verbose, usedMoves);;
    }
}
