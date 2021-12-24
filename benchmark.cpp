#include <chrono>
#include <min2phase/min2phase.h>
#include <min2phase/tools.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#define N_TIMES_BENCHMARK 10
#define MAX_MIN_PROBES 10240

unsigned long benchmarkInitialization(){
    using namespace std::chrono;

    unsigned long time_elapsed = 0;
    time_point<high_resolution_clock> begin, end;
    int i;

    for(i = 0; i < N_TIMES_BENCHMARK; i++){
        begin = high_resolution_clock::now();
        min2phase::init();
        end = high_resolution_clock::now();

        time_elapsed += duration_cast<milliseconds>(end - begin).count();
    }

    return time_elapsed / N_TIMES_BENCHMARK;
}

float benchmarkSolveMoves(int32_t probeMin, float* time_elapsed, int8_t maxMoves){
    using namespace std::chrono;

    std::string randomState;
    time_point<high_resolution_clock> begin, end;
    uint8_t usedMoves;
    uint8_t movesCount = 0;
    int i;

    *time_elapsed = 0;

    for(i = 0; i < N_TIMES_BENCHMARK; i++){
        randomState = min2phase::tools::randomCube();
        begin = high_resolution_clock::now();
        min2phase::solve(randomState, maxMoves, 1000000, probeMin, 0, &usedMoves);
        end = high_resolution_clock::now();

        *time_elapsed += (float)duration_cast<milliseconds>(end - begin).count();

        movesCount += usedMoves;
    }

    *time_elapsed /= N_TIMES_BENCHMARK;

    return (float)movesCount/N_TIMES_BENCHMARK;
}

int main(int argc, char* argv[]){

    using namespace std;
    unsigned long initTime = benchmarkInitialization();
    float time_elapsed;

    string processName;
    ifstream processordNameFile;

#ifdef __linux__

    system("cat /proc/cpuinfo | grep \"model name\" > processorName");
    processordNameFile.open("processorName");
    getline(processordNameFile, processName);
    processordNameFile.close();
    system("rm processorName");
    cout << "Processor used (OS Linux): " << processName.substr(processName.find(": ")+2, processName.size()) << endl << endl;

#elif defined(_WIN32)

    system("wmic cpu get name > processorName");
    processordNameFile.open("processorName");
    getline(processordNameFile, processName);
    getline(processordNameFile, processName);
    processordNameFile.close();
    system("del processorName");
    cout << "Processor used (OS Windows): " << processName << endl << endl;

#endif

    min2phase::tools::setRandomSeed(time(nullptr));

    cout << "Init time average: " << initTime << "ms" << endl << endl;

    cout << "| probeMin | Avg Length |   Time   |\n|:--------:|:----------:|:--------:|\n";
    for(int32_t probe = 5; probe <= MAX_MIN_PROBES; probe *= 2){
        cout << "|" << fixed << setw(7) << probe << "   |";
        cout << fixed << setprecision(1) << setw(8) << benchmarkSolveMoves(probe, &time_elapsed, 31) << "    |";
        cout << fixed << setprecision(1) << setw(6) << time_elapsed << " ms |\n";
    }

    cout << "\n|   Time    |  Max Moves |\n"
            "|:---------:|:----------:|\n";
    for(int8_t maxDepth = 31; maxDepth >= 20; maxDepth--){
        benchmarkSolveMoves(0, &time_elapsed, maxDepth);
        cout << "|" << fixed << setprecision(1) << setw(5) << time_elapsed << " ms   |";
        cout << fixed << setprecision(1) << setw(7) << (int32_t)maxDepth << "     |\n";
    }

    return 0;
}