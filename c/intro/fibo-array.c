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
   @file fibo-array.c
   @brief Compute the first \c n fibonacci numbers and save the result in an array
*/
#include <stdlib.h>
#include <stdio.h>
#include "fibo-array.h"

int main(int argc, char **argv) {
        unsigned int limit = atoi(argv[1]);
        unsigned int results[limit];
        fibo_array(results, limit);
        for (unsigned int n = 0; n < limit; n++)
                printf("%d: %d\n", n + 1, results[n]);
}

/**
   \brief actual function
*/
void fibo_array(unsigned int* results, unsigned int limit) {
        for (unsigned int n = 1; n <= limit; n++)
                results[n - 1] = fibo(n);
}


/**
   \brief Prints the fibonacci sequence up to a certain value \c limit
   \param limit
*/
unsigned int fibo(unsigned int x) {
        if (x <= 2)
                return 1;
        else
                return fibo(x - 1) + fibo(x - 2);
}
