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

#include "gsa.h"

uint64_t
compute_gsa(gsa_s** gsa, uint64_t** lcp, char** strings, uint64_t k) {

/*
  concatenate input strings, adding a distinct terminator for each input
  string. We assume that the input alphabet has at most 8 characters
*/
        char* text = strdupa("");
        int64_t len[k + 1];
        len[0] = 0;
        for (uint64_t seq = 0; seq < k; seq++) {
                char* tmp = strdupa(text);
                asprintf(&text, "%s%s%c", tmp, strings[seq], (char) seq + 1 + 'Z');
                len[seq + 1] = len[seq] + strlen(strings[seq]) + 1;
#ifdef DEBUG
                printf("Len:%d\n", len[seq + 1]);
#endif
        }
        /*
          Now len[i] is the cumulative length of first i strings
        */
        uint64_t n = strlen(text);
        int* sa = malloc(n * sizeof(*sa));
        unsigned int* lcpt = malloc(n * sizeof(*lcpt));
        *lcp = malloc(n * sizeof(**lcp));
        printf("%s\n", text);
        if(sais((unsigned char*) text, sa, n) != 0) {
                fprintf(stderr, "Cannot allocate memory.\n");
                exit(EXIT_FAILURE);
        }
        sais_lcp((unsigned char*) text, sa, lcpt, n);
        lcpt[0] = 0;
/* from SA to GSA */
        *gsa = GC_malloc(n * sizeof(**gsa));
        for (uint64_t i = 0; i < n; i++) {
                uint64_t seq = 0;
                for (; sa[i] >= len[seq + 1] && seq < k; seq++) {}
                (*gsa)[i].pos = sa[i] - len[seq];
                (*gsa)[i].seq = seq;
                (*lcp)[i] = lcpt[i];
#ifdef DEBUG
                printf("%6d: %6d %6d %6d - %6d\n", i, (*gsa)[i].seq, (*gsa)[i].pos, (*lcp)[i], sa[i]);
#endif
        }

        free(sa);
        return n;
}
