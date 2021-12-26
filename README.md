# min2phase C++
- Rubik's Cube solver or scrambler.
- [Algorithm](Algorithm.md)
- [Benchmarks](Benchmarks.md)

# Usage

See [min2phase.h](include/min2phase/min2phase.h) for the arguments explanation of the solver.

```C++
#include <iostream>
#include <min2phase/min2phase.h>
#include <min2phase/tools.h>

int main(int argc, char *argv[]){

    min2phase::init();//precomputed coordinates

    std::cout << min2phase::solve(min2phase::tools::randomCube(), 21, 1000000, 0, min2phase::APPEND_LENGTH | min2phase::USE_SEPARATOR, nullptr);
    return 0;
}
```
See [tools.h](include/min2phase/tools.h) for some useful function for the solver.
You can also read the coordinates from a file, see [min2phase.h](include/min2phase/min2phase.h). Reading from a file the coordinates will increase the speed of the algorithm.


#Compiling

```bash
make
g++ -I include/ example.cpp -L. -lmin2phase -o example -Wl,-rpath,.
```