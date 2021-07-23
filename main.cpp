#include <chrono>
#include <iostream>
#include <min2phase/min2phase.h>
#include <min2phase/Tools.h>
#include <sys/>

int main(int argc, char *argv[]){

    auto start = std::chrono::high_resolution_clock::now();
    min2phase::init();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout <<  std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms\n";
    //BRR GYW OGY BRB YBB WBO GRO OOB YYG GGR WWR OYG YOY BGW WGR WYR ORW WOB
    std::cout << min2phase::solve(min2phase::Tools::randomState(), 21, 1000000, 0, min2phase::APPEND_LENGTH | min2phase::USE_SEPARATOR);
    return 0;
}