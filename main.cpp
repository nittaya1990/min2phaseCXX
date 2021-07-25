#include <chrono>
#include <iostream>
#include <min2phase/min2phase.h>
#include <min2phase/tools.h>
#include <string>


int main(int argc, char *argv[]){
    uint8_t movesUsed;

    auto start = std::chrono::high_resolution_clock::now();
    min2phase::init();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout <<  std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms\n";

    std::cout << min2phase::solve(min2phase::tools::randomState(), 21, 1000000, 0, min2phase::APPEND_LENGTH | min2phase::USE_SEPARATOR, &movesUsed) << std::endl;

    return 0;
}