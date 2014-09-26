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
   @brief Compute the fibonacci numbers up to \c n and save the result in an
   array.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <glib.h>
#include "fibo-array3.h"


/**
   Since I do not know the number of elements in the sequence, I cannot fix the
   array size at declaration time.
   I save the results in a list, and then I copy the list to an array.

   The last element of \c results is a sentinel with value \c 0.
*/
int main(int argc, char **argv) {
        uint64_t limit = atoi(argv[1]);
        uint64_t* results = fibo_array(limit);
        for (uint64_t n = 0; results[n] > 0; n++)
                printf("%" PRIu64 ": %" PRIu64 "\n", n + 1, results[n]);
        free(results);
}

/**
   \brief actual function
*/
uint64_t* fibo_array(uint64_t limit) {
        GSList* list = NULL;
        uint64_t prev = 1;
        uint64_t prev2 = 1;
        uint64_t actual = 0;
        if (limit >= 1)
                list = g_slist_append(list, GINT_TO_POINTER(1));
        if (limit >= 2)
                list = g_slist_append(list, GINT_TO_POINTER(1));
        for (uint64_t n = 2; actual <= limit; n++) {
                actual = prev + prev2;
                list = g_slist_append(list, GINT_TO_POINTER(actual));
                prev2 = prev;
                prev = actual;
        }
        uint64_t size = g_slist_length(list);
        uint64_t* results = malloc((size + 1) * sizeof(uint64_t));
        uint64_t i = 0;
        for (GSList* p = list; p != NULL; p = g_slist_next(p), i++) {
                results[i] = GPOINTER_TO_INT(p->data);
        }
        results[i] = 0;
        g_slist_free(list);
        return results;
}
