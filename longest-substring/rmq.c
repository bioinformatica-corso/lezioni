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

#include "rmq.h"
static inline uint64_t min(int a, int b) {
        return a < b ? a : b;
}

static uint64_t** __matrix__ = NULL;

uint64_t
rmq(uint64_t* a, uint64_t lb, uint64_t ub) {
        size_t size = sizeof(*a) / sizeof(a[0]);
        if (__matrix__ == NULL) {
                /* Initialize the matrix storing the partial RMQ,
                   __matrix__[i][j] stores the RMQ(i, i + 2^j - 1)
                */
                size_t logwidth = floor(log2(size));
                __matrix__ = GC_malloc(size * logwidth);
                size_t i, j;
                for (i = 0; i < size; i++)
                        __matrix__[i][0] = a[i];
                for (j = 1; j <= logwidth; j++) {
                        size_t width = 1 << j;
                        for (i = 0; i + width - 1 < size; i++)
                                __matrix__[i][j] = min(__matrix__[i][j - 1], __matrix__[i + width][j - 1]);
                }
        }
        size_t width = floor(log2(ub - lb + 1));
        return min(__matrix__[lb][width], __matrix__[ub - width + 1][width]);
}
