#ifndef MIN2PHASE_TOOLS_H
#define MIN2PHASE_TOOLS_H 1

#include <cstdint>
#include <string>

/**
 * This class contains some useful method for the cube.
 */
namespace min2phase { namespace tools {

    /**
     * This method is used to set a seed for randomize the cube.
     *
     * @param seed : the seed for the random number.
     */
    void setRandomSeed(uint32_t seed);

    /**
     * This is used to get a random cube state.
     *
     * @return : the string that contains the random cube.
     */
    std::string randomState();

    std::string fromScramble(const std::string& s);
} };

#endif //MIN2PHASE_TOOLS_H