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
#include "sais-lite-2.4.1/sais.h"

typedef struct gsa_s {
        uint64_t pos;
        uint64_t seq;
} gsa_s;

/**
   \param an array of strings and the number of the strings (i.e. the size of
   the array).
   computes the GSA and LCP arrays, and allocates the required memory

   \return the length of the GSA (and of the LCP)
*/
uint64_t
compute_gsa(gsa_s** gsa, uint64_t** lcp, char** strings, uint64_t k);
