#include <min2phase/min2phase.h>
#include "Search.h"

namespace min2phase {

    void init() {
        CubeInfo::init();
        Coords::init();
    }

    //solve the cube
    std::string solve(const std::string &facelets, int8_t maxDepth, int probeMax, int probeMin,
                                     int8_t verbose) {
        Search s;
        std::string solution;

        solution = s.solve(facelets, maxDepth, probeMax, probeMin, verbose);
        return solution;
    }
}