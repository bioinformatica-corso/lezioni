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

#include "sa-matching.h"
KSEQ_INIT(gzFile, gzread)

/* compares a pattern (a string) and a suffix of the text, by extracting the prefix of
   the text with same length as the patter. The return value is:
   = 0 if the pattern is equal the prefix
   < 0 if the pattern precedes the prefix (in lexicographical order)
   > 0 if the pattern follows  the prefix (in lexicographical order)
*/
static int
suffix_cmp(const char* text, const unsigned int pos, const unsigned int n,
           const char* pattern, const unsigned int m) {
        size_t size = (m < n - pos + 1) ? m : n - pos + 1;
        return strncmp(pattern, text + pos, size);
}


static char* read_text(char* filename) {
        gzFile fp;
        kseq_t *seq;
        fp = gzopen(filename, "r");
        assert(fp != NULL && "Could not open fasta file\n");
        seq = kseq_init(fp);
        int res = kseq_read(seq);
        assert(res >= 0);
        gzclose(fp);
        char* text = NULL;
        int err = asprintf(&text, "%s", seq->seq.s);
        assert(err >= 0);
        kseq_destroy(seq);
        return text;
}

int main(int argc, char **argv) {
        static struct gengetopt_args_info args_info;
        assert(cmdline_parser(argc, argv, &args_info) == 0);
        char* pattern = args_info.pattern_orig;
        char* text = read_text(args_info.text_arg);
        uint32_t n = strlen(text);
        uint32_t m = strlen(pattern);


        int* sa = malloc(n * sizeof(*sa));
        unsigned int* lcp = malloc(n * sizeof(*sa));

        if(sais((unsigned char*) text, sa, (int)n) != 0) {
                fprintf(stderr, "%s: Cannot allocate memory.\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        /* printf("Suffix array: ok\n"); */
        sais_lcp((unsigned char*) text, sa, lcp, n);
        lcp[0] = 0;
        /* printf("LCP array: ok\n"); */

        unsigned int found = -1;
        if (suffix_cmp(text, sa[0], n, pattern, m) >= 0 &&
            suffix_cmp(text, sa[n - 1], n, pattern, m) <= 0) {
                /* The pattern is not outside the realm of available suffixes.
                   Let's search for an occurrence
                */
                unsigned int lp = 0;
                unsigned int rp = n - 1;
                while (lp <= rp) {
                        unsigned int mp = lp + (rp - lp) / 2;
                        int cmp = suffix_cmp(text, sa[mp], n, pattern, m);
                        if (cmp == 0) {
                                found = mp;
                                break;
                        }
                        if (cmp < 0)
                                rp = mp - 1;
                        else
                                lp = mp + 1;
                }
        }

        /* If found != -1 we have found at least one occurrence.
           Let's find all of them!
        */
        if (found != -1) {
                unsigned int lp = found;
                for (;lcp[lp] >= m; lp--) {}
                unsigned int rp = found + 1;
                for (;lcp[rp] >= m; rp++) {}
                for (unsigned int pos = lp; pos < rp; pos++) {
                        char* x = strndupa(text + sa[pos], m);
                        printf("Occurrence %s at position %d\n", x, sa[pos]);
                }
        }
        free(sa);
        free(lcp);
        free(text);
}
