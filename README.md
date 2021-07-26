# min2phase C++
- Rubik's Cube solver or scrambler. [![Build Status](https://travis-ci.org/cs0x7f/min2phase.svg?branch=master)](https://travis-ci.org/cs0x7f/min2phase)
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

    std::cout << min2phase::solve(min2phase::tools::randomCube(), 21, 1000000, 0, min2phase::APPEND_LENGTH | min2phase::USE_SEPARATOR);
    return 0;
}
```

# License GPLv3

    Copyright (C) 2019  Shuang Chen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    aint64_t with this program.  If not, see <http://www.gnu.org/licenses/>.

