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

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "cmdline.h"
#include "klib/kseq.h"
#include <zlib.h>
#include <gc.h>

typedef struct cell_s {
        int64_t cell;
        uint64_t prev_x;
        uint64_t prev_y;
} cell_s;

typedef struct range_s {
        uint64_t sx;
        uint64_t dx;
} range_s;

typedef struct band_s {
        uint64_t l1;
        uint64_t l2;
        uint64_t base;
        uint64_t extra;
} band_s;
