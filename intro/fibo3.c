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
   @file fibo3.c
   @brief Compute the first \c n fibonacci numbers

   Iterative version
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/**
   \brief Prints the fibonacci sequence up to a certain value \c limit
   \param x: counter

   The invariant is that \c prev, \c prev2 are respectively \c fibo(x-1) and \c fibo(x-2)
*/
void fibo(uint32_t x) {
        if (x <= 1)
                printf("1\n");
        else if (x <= 2)
                printf("1 1\n");
        else {
                uint32_t prev = 1;
                uint32_t prev2 = 1;
                printf("1 1 ");
                for (uint32_t n = 3; n <= x; n++) {
                        uint32_t current = prev + prev2;
                        printf("%" PRIu32 " ", current);
                        prev2 = prev;
                        prev = current;
                }
        }
}

int main(int argc, char **argv) {
        uint32_t limit = atoi(argv[1]);
        fibo(limit);
}
