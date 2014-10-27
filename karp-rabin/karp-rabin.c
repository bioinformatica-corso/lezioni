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

#include "karp-rabin.h"
KSEQ_INIT(gzFile, gzread)

static uint32_t corrections[4];

static bool primep(const uint32_t n) {
        const uint32_t bases32[] = {2, 7, 61};
        return efficient_mr32(bases32, 3, n);
}

/*
  We will use a 30-bit random prime, so that all operations can be represented
  with 32-bit integers, since the alphabet ACGT can be represented with 2 bits.
*/
static uint32_t random_prime(gsl_rng* generator) {
        uint32_t p = 2 * gsl_rng_uniform_int(generator, 2 << 29) + 1;
        while (!primep(p)) {
                p = 2 * gsl_rng_uniform_int(generator, 2 << 29) + 1;
        }
        return p;
}

/* converts a characters into its numeric encoding */
static uint32_t ctoi (const char c) {
        if (c == 'C' || c == 'c') return 1;
        if (c == 'G' || c == 'g') return 2;
        if (c == 'T' || c == 't') return 3;
        return 0;
}

/*
  computes the fingerprint of the first len characters of a string
*/
static uint32_t init_h(const char* str, const uint32_t len, const uint32_t mod) {
        uint32_t f = ctoi(str[0]);
        for (size_t i = 1; i < len; i++) {
                f *= 4;
                f = f % mod;
                f += ctoi(str[i]);
                f = f % mod;
        }
        return f;
}

/*
  updates the fingerprint, given the incoming and the outgoing character
*/
static uint32_t next_h(uint32_t f, const char out, const char in, const uint32_t mod) {
        if (f < corrections[ctoi(out)])
                f += mod;
        return ((4 * (f - corrections[ctoi(out)]) % mod) + ctoi(in)) % mod;
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
        return seq->seq.s;
        // kseq_destroy(seq);
}

int main(int argc, char **argv) {
        static struct gengetopt_args_info args_info;
        assert(cmdline_parser(argc, argv, &args_info) == 0);
        char* pattern = args_info.pattern_orig;
        char* text = read_text(args_info.text_arg);
        uint32_t n = strlen(text);
        uint32_t m = strlen(pattern);
        /* hash is an hash table, declared and initialized as defined in UThash docs */
        struct occurrence* hash = NULL;

        /* Initialize random number generator */
        gsl_rng *rng = gsl_rng_alloc(gsl_rng_mt19937);
        uint32_t num_rounds = (size_t) args_info.rounds_arg;
        for (size_t i = 0; i < num_rounds; i++) {
                uint32_t mod = random_prime(rng);
                for (size_t j = 0; j < m; j++)
                        for (size_t c = 0; c < 4; c++)
                                corrections[c] = (j == 0) ? c : (corrections[c] << 2) % mod;
                uint32_t pattern_h = init_h(pattern, m, mod);
                uint32_t pos = m;
                for (uint32_t text_h = init_h(text, m, mod); pos < n;
                     text_h = next_h(text_h, text[pos - m], text[pos], mod), pos++)
                        if (pattern_h == text_h)
                                /* Insert the element in the hash */
                                insert(&hash, pos-m);
        }
        /* Iterates on the keys of the hash */
        visit(&hash, num_rounds, m, text);
}
