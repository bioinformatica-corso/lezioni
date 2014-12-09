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

#include "band.h"
#include "klib/kvec.h"
KSEQ_INIT(gzFile, gzread)

static void read_text(char* filename, char** sp1, char** sp2) {
        gzFile fp = gzopen(filename, "r");
        assert(fp != NULL && "Could not open fasta file\n");
        kseq_t* seq = kseq_init(fp);

        int kp = 0;
        kvec_t(char*) vec;
        kv_init(vec);

        int l = 0;
        while ((l = kseq_read(seq)) >= 0 && kp < 2) {
                char* tmp = NULL;
                int err = asprintf(&tmp, "%s", seq->seq.s);
                assert(err >= 0);
                kv_push(char*, vec, tmp);
                kp++;
        }
        assert(kp > 1 && "Not enough input sequences\n");
        *sp2 = strdup(kv_pop(vec));
        *sp1 = strdup(kv_pop(vec));
#ifdef DEBUG
        printf("Input sequence #1: %s\n", *sp1);
        printf("Input sequence #2: %s\n", *sp2);
#endif
        kseq_destroy(seq);
        gzclose(fp);
        kv_destroy(vec);
}

static uint64_t conv(uint64_t height, uint64_t base, uint64_t extra, uint64_t x, uint64_t y) {
        assert(x >= y || y - x <= extra + 1);
        assert(x < y || x - y <= base + 2 + extra);
        assert(y <= height);

        /* if the seeked cell is out of bounds, return 0, which is the worst
           possible outcome */
        if (x < y && y - x > extra) return 0;
        if (x > y && x - y > base + 1 + extra) return 0;

        if (y == 0) return x;
        uint64_t band = base + 1 + 2 * extra;
        return (band * y - 1) + (x - y + extra);
}

/*
  returns NULL if it could not compute an optimal alignment within the band
*/
static char* band_align(uint64_t base, uint64_t extra, char* s1, char* s2) {
        uint64_t l1 = strlen(s1);
        uint64_t l2 = strlen(s2);
        uint64_t band = base + 2 * extra;

        cell_s* m = malloc(band * (l1 + 1) * sizeof(cell_s));
        assert(m != NULL && "Cannot allocate matrix\n");

        /* boundary conditions */
        m[0].cell = 0;
        for (uint64_t x = 1; x < band; x++)
                m[x].cell = 0;

        for (uint64_t y = 1; y < l1; y++) {
                uint64_t sx = y - extra + 1;
                if (y < extra) sx = 0;
                uint64_t dx = y + base + extra;
                if (dx > l2) dx = l2;
                {
                        cell_s* tp = m + conv(l1, base, extra, sx, y);
                        tp->cell = 0;
                        tp->prev_x = sx -1;
                        tp->prev_y = y -1;
                        tp = m + conv(l1, base, extra, dx, y);
                        tp->cell = INT_MIN;
                        tp->prev_x = dx -1;
                        tp->prev_y = y -1;
                }
                for (uint64_t x = sx + 1; x <= dx; x++) {
#ifdef DEBUG
                        printf("StepB: %d, %d\n", x, y);
#endif
                        cell_s* tp = m + conv(l1, base, extra, x, y);
                        if (s1[y] == s2[x]) {
                                tp->cell = m[conv(l1, base, extra, x - 1, y - 1)].cell + 1;
                                tp->prev_x = x -1;
                                tp->prev_y = y -1;
                        } else if (m[conv(l1, base, extra, x - 1, y)].cell > m[conv(l1, base, extra, x, y - 1)].cell) {
                                tp->cell = m[conv(l1, base, extra, x - 1, y)].cell;
                                tp->prev_x = x - 1;
                                tp->prev_y = y;
                        } else {
                                tp->cell = m[conv(l1, base, extra, x, y - 1)].cell;
                                tp->prev_x = x;
                                tp->prev_y = y - 1;
                        }
#ifdef DEBUG
                        printf("StepB result: %d, %d, %d\n", m[conv(l1, base, extra, x, y)].cell,
                               m[conv(l1, base, extra, x, y)].prev_x, m[conv(l1, base, extra, x, y)].prev_y);
#endif
                }
        }

/* reconstruct the solution, if possible */
        uint64_t x = l2;
        uint64_t y = l1;
#ifdef DEBUG
        printf("StepC: Reconstruction\n");
#endif

        char* lcs = malloc(m[conv(l1, base, extra, l2, l1) + 1].cell * sizeof(char));
        for (cell_s c = m[conv(l1, base, extra, x, y)]; c.cell  > 0; x = c.prev_x, y = c.prev_y) {
#ifdef DEBUG
                printf("StepC: %d,%d,%d,%d,%d\n", x, y, c.cell, c.prev_x, c.prev_y);
#endif
                if (c.cell < 0) return NULL;
                if (x <= y - extra) return NULL;
                if (c.prev_x == x - 1 && c.prev_y == y - 1)
                        lcs[c.cell - 1] = s1[x];
        }
        return lcs;
}

int main(int argc, char **argv) {
        static struct gengetopt_args_info args_info;
        assert(cmdline_parser(argc, argv, &args_info) == 0);

        /* read input data */
        char* s1 = NULL;
        char* s2 = NULL;
        read_text(args_info.strings_arg, &s1, &s2);
        assert(s1 != NULL && "cannot read first string\n");
        assert(s2 != NULL && "cannot read second string\n");
        /* make s1 the shortest of the input strings */
        if (strlen(s2) < strlen(s1)) {
                char* tmp = s1;
                s1 = s2;
                s2 = tmp;
        }
        uint64_t l1 = strlen(s1);
        uint64_t l2 = strlen(s2);
        uint64_t base = l2 - l1 + 1;
        uint64_t extra = 2;
        char* lcs = NULL;
#ifdef DEBUG
        printf("Instance lengths: %d,%d\n", l1, l2);
#endif
        for (;lcs == NULL && base + 2 * extra <= l2 + 1; extra *= 2) {
#ifdef DEBUG
                printf("Step: %d,%d\n", base, extra);
#endif
                lcs = band_align(base, extra, s1, s2);
        }
        /* if (lcs != NULL) { */
        /*         lcs = std_align(s1, s2); */
        /* }     */

        if (lcs != NULL) {
                printf("Longest subsequence:%s\n", lcs);
        }
}
