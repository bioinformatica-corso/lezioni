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
#include "uthash.h"

/* A struct as defined in uthash user guide, this should represent the key/value mapping in the hash */
struct match {
  uint32_t id;            
  uint32_t count;
  UT_hash_handle hh;
};

void add_occ(struct match** occ, uint32_t pos) {
  struct match *s;
  HASH_FIND_INT(*occ, &pos, s);
  if (s == NULL) {
    s = malloc(sizeof(struct match));
    s->id = pos;
    s->count = 1;
    HASH_ADD_INT(*occ, id, s );
  } else {
    uint32_t new_count = s->count;
    ++new_count;
    struct match *o = malloc(sizeof(struct match));
    o->id = pos;
    o->count = new_count;
    HASH_REPLACE_INT(*occ, id, o, s);
    free(s);
    // I should free the deleted object memory
  }
}

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
  /* occ[] stores if a position is an occurrence */
  /* uint32_t* occ = calloc(n, sizeof(*occ)); */
  struct match* occ = NULL;

  /* Initialize random number generator */
  gsl_rng *rng = gsl_rng_alloc(gsl_rng_mt19937);
  uint32_t num_rounds = (size_t) args_info.rounds_arg;
#pragma omp parallel for schedule (dynamic) 
  for (size_t i = 0; i < num_rounds; i++) {
    uint32_t mod = random_prime(rng);
    for (size_t j = 0; j < m; j++)
      for (size_t c = 0; c < 4; c++)
	corrections[c] = (j == 0) ? c : (corrections[c] << 2) % mod;
    uint32_t pattern_h = init_h(pattern, m, mod);
    uint32_t pos = m;
    for (uint32_t text_h = init_h(text, m, mod); pos < n;
	 text_h = next_h(text_h, text[pos - m], text[pos], mod), pos++)
      if (pattern_h == text_h) {
	uint32_t tmp = pos-m;
#pragma omp critical(dataupdate)
	add_occ(&occ, tmp);
      }
  }
  for (struct match *s = occ; s != NULL; s=s->hh.next)
	  
    if (s->count >= num_rounds) {
      char* x = strndupa(text + s->id, m);
      printf("Occurrence %s at position %d\n", x, s->id);
    }
  //I don't care about freeing memory, right now
}
