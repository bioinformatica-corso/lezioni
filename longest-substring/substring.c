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

#include "substring.h"
#include "gsa.h"
#include "rmq.h"
#include "klib/kvec.h"
KSEQ_INIT(gzFile, gzread)

static uint64_t
min_array(uint64_t* a, size_t n) {
        uint64_t x = a[0];
        for (size_t i = 1; i < n; i++)
                if (a[i] < x)
                        x = a[i];
        return x;
}

static uint64_t
max_array(uint64_t* a, size_t n) {
        uint64_t x = a[0];
        for (size_t i = 1; i < n; i++)
                if (a[i] > x)
                        x = a[i];
        return x;
}

static char** read_text(char* filename, uint64_t* kp) {
        gzFile fp = gzopen(filename, "r");
        assert(fp != NULL && "Could not open fasta file\n");
        kseq_t* seq = kseq_init(fp);

        *kp = 0;
        kvec_t(char*) vec;
        kv_init(vec);


        int l = 0;

        while ((l = kseq_read(seq)) >= 0) {
                char* tmp = NULL;
                int err = asprintf(&tmp, "%s", seq->seq.s);
                assert(err >= 0);
                kv_push(char*, vec, tmp);
                (*kp)++;
        }
        assert(*kp > 1 && "Not enough input sequences\n");
        char** strings = GC_malloc(*kp * sizeof(char*));
        for (size_t i = 0; i < *kp; i++) {
                strings[i] = strdup(kv_pop(vec));
#ifdef DEBUG
                printf("Input sequence #%d: %s\n", i, strings[i]);
#endif
        }
        kseq_destroy(seq);
        gzclose(fp);
        kv_destroy(vec);
        return strings;
}

int main(int argc, char **argv) {
        static struct gengetopt_args_info args_info;
        assert(cmdline_parser(argc, argv, &args_info) == 0);

        /* read data and create the GSA */
        uint64_t k = 0;
        char** strings = read_text(args_info.strings_arg, &k);
        gsa_s* gsa = NULL;
        uint64_t* lcp = NULL;
        uint64_t n = compute_gsa(&gsa, &lcp, strings, k);
        /*
          find the longest substring in two different way, depending on whether
          we have more than two input strings
        */
        uint64_t max = 0;
        uint64_t substr_pos = 0;
        uint64_t substr_seq = 0;
        if (k == 2) {
                for (uint64_t i = 1; i < n; i++) {
                        if (gsa[i].seq != gsa[i - 1].seq && lcp[i] > max) {
                                max = lcp[i];
                                substr_pos = gsa[i].pos;
                                substr_seq = gsa[i].seq;
                        }
                }
        } else {
                /* prev contains, for each input string, the index of the most
                   recently seen suffix of that input string */
                uint64_t prev[k];
                _Bool found[k];
                for (uint64_t i = 0; i < k; i++) {
                        prev[i] = 0;
                        found[i] = false;
                }
                for(uint64_t ok = 0, i = 0; ok < k; i++) {
                        if (!found[gsa[i].seq]) {
                                found[gsa[i].seq] = true;
                                ok++;
                        }
                        prev[gsa[i].seq] = i;
                }
#ifdef DEBUG
                printf("Looking for substrings\n");
#endif
                for(uint64_t i = max_array(prev, k); i < n; i++) {
                        prev[gsa[i].seq] = i;
                        uint64_t ub = i;
                        uint64_t lb = min_array(prev, k);
                        uint64_t new = rmq(lcp, n, lb, ub);
#ifdef DEBUG
                        printf("%6d:%6d,%6d => id=%6d", i, lb, ub, new);
#endif
                        if (new > max) {
                                max = new;
                                substr_pos = gsa[i].pos;
                                substr_seq = gsa[i].seq;
                        }
                }
        }
        if (max > 0) {
                char* x = strndupa(strings[substr_seq] + substr_pos, max);
                printf("Longest substring:%s\n", x);
        }
}
