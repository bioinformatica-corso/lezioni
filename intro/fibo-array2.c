/*
  Copyright (C) 2014 by Gianluca Della Vedova


  You can redistribute this file and/or modify it
  under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Box is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this file
  If not, see <http://www.gnu.org/licenses/>.
*/


/**
   @file fibo-array2.c
   @brief Compute the first \c n fibonacci numbers and save the result in an
   array.
   Iterative version.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "fibo-array2.h"

int main(int argc, char **argv) {
        uint64_t limit = atoi(argv[1]);
        uint64_t results[limit];
        fibo_array(results, limit);
        for (uint64_t n = 0; n < limit; n++)
                printf("%" PRIu64 ": %" PRIu64 "\n", n + 1, results[n]);
}

/**
   \brief actual function
*/
void fibo_array(uint64_t* results, uint64_t limit) {
        if (limit >= 1) results[0] = 1;
        if (limit >= 2) results[1] = 1;
        for (uint64_t n = 2; n < limit; n++)
                results[n] = results[n - 1] + results[n - 2];
}
