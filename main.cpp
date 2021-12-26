#include <chrono>
#include <iostream>
#include <min2phase/min2phase.h>
#include <min2phase/tools.h>
#include <string>

int main(int argc, char *argv[]){
    uint8_t movesUsed;

    min2phase::tools::setRandomSeed(time(nullptr));

    auto start = std::chrono::high_resolution_clock::now();
    //min2phase::init();
    min2phase::loadFile("coords");
    auto end = std::chrono::high_resolution_clock::now();

    std::cout <<  std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms\n";

    start = std::chrono::high_resolution_clock::now();
    std::cout << min2phase::solve("RUDBUDRLFLFBURRBBFFFUBFFBFDURLDDRDUDULUDLDFLLRLBUBRRBL", 21, 1000000, 0, min2phase::APPEND_LENGTH | min2phase::USE_SEPARATOR, &movesUsed) << std::endl;
    end = std::chrono::high_resolution_clock::now();

    std::cout <<  std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms\n";

    return 0;
}