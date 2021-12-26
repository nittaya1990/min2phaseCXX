/**
 * This file contains all the function used to tests the
 * algorithm.
 * It chek the errors and the output of the search.
 */

#ifndef MIN2PHASE_TESTS_H
#define MIN2PHASE_TESTS_H

namespace min2phase{ namespace tests{

    /**
     * Tests the errors of a malformed input.
     */
    void testInput();

    /**
     * Tests the errors of a malformed search.
     */
    void testSearchError();

    /**
     * Test if the algorithm generate a valid solution.
     */
    void testSearch();

}   }

#endif //MIN2PHASE_TESTS_H
