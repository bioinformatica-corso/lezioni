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
        const uint32_t bases[] = {2,  3,  5,  7,  11, 13, 17, 19, 23, 29,
                                  31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
                                  73, 79, 83, 89, 97,101,103,107,109,113,
                                  127,131,137,139,149,151,157,163,167,173,
                                  179,181,191,193,197,199,211,223,227,229,
                                  233,239,241,251,257,263,269,271,277,281,
                                  283,293,307,311,313,317,331,337,347,349,
                                  353,359,367,373,379,383,389,397,401,409,
                                  419,421,431,433,439,443,449,457,461,463,
                                  467,479,487,491,499,503,509,521,523,541,
                                  547,557,563,569,571,577,587,593,599,601,
                                  607,613,617,619,631,641,643,647,653,659,
                                  661,673,677,683,691,701,709,719,727,733,
                                  739,743,751,757,761,769,773,787,797,809,
                                  811,821,823,827,829,839,853,857,859,863,
                                  877,881,883,887,907,911,919,929,937,941,
                                  947,953,967,971,977,983,991,997,1009
        };
        for(uint32_t i = 0; i < sizeof(bases) / sizeof(bases[0]); i++) {
                if ((n % bases[i]) == 0) return false;
        }
        for(uint32_t d = 1011; d < (1<<16); d+=2) {
                if ((n % d) == 0) return false;
        }
        return true;
}

/*
  We will use a 30-bit random prime, so that all operations can be represented
  with 32-bit integers, since the alphabet ACGT can be represented with 2 bits.
*/
static uint32_t random_prime(gsl_rng* generator) {
        uint32_t p = 0;
        do {
                p = 2 * gsl_rng_uniform_int(generator, 1 << 29) + 1;
        } while (p < (1 << 16) || !primep(p));
        assert(p < (1<<30));
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
        /* occ[] stores if a position is an occurrence */
        uint32_t* occ = calloc(n, sizeof(*occ));
        assert(occ != NULL);

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
                                occ[pos - m]++;
        }
        for (uint32_t pos = 0; pos < n; pos++)
                if (occ[pos] >= num_rounds) {
                        char* x = strndupa(text + pos, m);
                        printf("Occurrence %s at position %d\n", x, pos);
                }
        free(occ);
}
