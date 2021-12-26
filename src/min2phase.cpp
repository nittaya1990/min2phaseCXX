/**
 * Copyright (C) 2021 Borgo Federico
 */

#include <fstream>
#include <min2phase/min2phase.h>
#include "Search.h"

namespace min2phase {

    //initialization
    void init() {
        info::init();
        coords::init();
    }

    //write coords
    bool writeFile(const std::string& name){
        if(!coords::isInit())
            init();

        std::ofstream out(name);

        if(out) {
            out.write(reinterpret_cast<char *>(&coords::coords), sizeof(coords::coords_t));
            out.close();
            return true;
        }

        return false;
    }

    //load coords
    bool loadFile(const std::string& name){
        std::ifstream in(name);

        if(in){
            in.read(reinterpret_cast<char*>(&coords::coords), sizeof(coords::coords_t));
            in.close();
            info::init();
            return true;
        }
        else
            init();

        return false;
    }

    //solve the cube
    std::string solve(const std::string &facelets, int8_t maxDepth, int32_t probeMax, int32_t probeMin,
                                     int8_t verbose, uint8_t* usedMoves) {

        return Search().solve(facelets, maxDepth, probeMax, probeMin, verbose, usedMoves);
    }
}
