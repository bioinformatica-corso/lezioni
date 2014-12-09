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

/**
   \param: the description of a band, the y coordinate
   \returns: a range_s that contains the min and max values for the legal range
   for the y coordinate
*/
static range_s range(band_s band, uint64_t y) {
        range_s r = {0, 0};
        r.sx = y - band.extra;
        if (y < band.extra) r.sx = 0;
        r.dx = y + (band.base - 1) + band.extra;
        if (r.dx > band.l2) r.dx = band.l2;
        return r;
}

static uint64_t conv(band_s band, uint64_t x, uint64_t y) {
        /* assert(x <= y || y - x <= extra); */
        /* assert(x > y || x - y <= base - 1 + extra); */
        assert(y <= band.l1);

        /* check if the seeked cell is out of bounds   */
        assert(x >= y || y - x <= band.extra);
        assert(x <= y || x - y <= band.base - 1 + band.extra);

        uint64_t width = band.base + 2 * band.extra;
        return (width * y) + (x - y + band.extra);
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
        band_s b = {
                .base = base,
                .extra = extra,
                .l1 = l1,
                .l2 = l2
        };

        for (uint64_t y = 1; y <= l1; y++) {
                range_s r = range(b, y);
                {
                        cell_s* tp = m + conv(b, r.sx, y);
                        tp->cell = 0;
                        tp->prev_x = 0;
                        tp->prev_y = 0;
                        tp = m + conv(b, r.dx, y);
                        tp->cell = 0;
                        tp->prev_x = 0;
                        tp->prev_y = 0;
                }
                for (uint64_t x = r.sx + 1; x <= r.dx; x++) {
#ifdef DEBUG
                        printf("StepB: %d, %d\n", x, y);
#endif
                        cell_s* tp = m + conv(b, x, y);
                        tp->cell = m[conv(b, x - 1, y - 1)].cell;
                        tp->prev_x = x -1;
                        tp->prev_y = y -1;
                        if (s1[y - 1] == s2[x - 1]) ++(tp->cell);
                        if (m[conv(b, x - 1, y)].cell > tp->cell) {
                                tp->cell = m[conv(b, x - 1, y)].cell;
                                tp->prev_x = x - 1;
                                tp->prev_y = y;
                        }
                        if (x < r.dx && m[conv(b, x, y - 1)].cell > tp->cell) {
                                tp->cell = m[conv(b, x, y - 1)].cell;
                                tp->prev_x = x;
                                tp->prev_y = y - 1;
                        }
#ifdef DEBUG
                        printf("StepB result: %d, %d, %d\n", m[conv(b, x, y)].cell,
                               m[conv(b, x, y)].prev_x, m[conv(b, x, y)].prev_y);
#endif
                }
        }

#ifdef DEBUG
        printf("CONV: base=%d, extra=%d\n", base, extra);
        for (uint64_t y = 0; y <= l1; y++) {
                range_s r = range(b, y);
                for (uint64_t x = 0; x <= l2; x++) {
                        if (x < r.sx || x > r.dx)
                                printf("XX|");
                        else
                                printf("%2d|", conv(b, x, y));
                }
                printf("\n");
        }
        printf("MATRIX: base=%d, extra=%d\n", base, extra);
        printf("      ");
        for (uint64_t x = 1; x <= l2; x++)
                printf(" %c ", s2[x - 1]);
        printf("\n");

        for (uint64_t y = 0; y <= l1; y++) {
                if (y > 0)
                        printf(" %c ", s1[y - 1]);
                else
                        printf("   ");
                range_s r = range(b, y);
                for (uint64_t x = 0; x <= l2; x++) {
                        if (x < r.sx || x > r.dx)
                                printf("XX|");
                        else
                                printf("%2d|", m[conv(b, x, y)].cell);
                }
                printf("\n");
        }
        printf("DIRECTION: base=%d, extra=%d\n", base, extra);
        printf("      ");
        for (uint64_t x = 1; x <= l2; x++)
                printf(" %c ", s2[x - 1]);
        printf("\n");

        for (uint64_t y = 0; y <= l1; y++) {
                if (y > 0)
                        printf(" %c ", s1[y - 1]);
                else
                        printf("   ");
                range_s r = range(b, y);
                for (uint64_t x = 0; x <= l2; x++) {
                        if (x < r.sx || x > r.dx)
                                printf("XX|");
                        else {
                                (m[conv(b, x, y)].prev_x < x) ? printf("<") : printf(" ");
                                (m[conv(b, x, y)].prev_y < y) ? printf("^|") : printf(" |");
                        }
                }
                printf("\n");
        }
#endif


/* reconstruct the solution, if possible */
        uint64_t x = l2;
        uint64_t y = l1;
#ifdef DEBUG
        printf("StepC: Reconstruction\n");
#endif

        char* lcs = malloc((m[conv(b, l2, l1)].cell + 1) * sizeof(char));
        lcs[m[conv(b, l2, l1)].cell] = '\0';
        for (cell_s c = m[conv(b, x, y)]; c.cell  > 0;
             x = c.prev_x, y = c.prev_y, c = m[conv(b, x, y)]) {
#ifdef DEBUG
                printf("StepC: %d,%d,%d,%d,%d=%c%c\n", x, y, c.cell, c.prev_x, c.prev_y, s1[y-1], s2[x-1]);
#endif
                if (c.cell < 0) return NULL;
                range_s r = range(b, y);
                if (x == r.sx && r.sx > 0 || x == r.dx && r.dx < l2) return NULL;
                if (s1[y - 1] == s2[x - 1])
                        lcs[c.cell - 1] = s1[y - 1];
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
        uint64_t extra = 1;
        char* lcs = band_align(base, 0, s1, s2);
#ifdef DEBUG
        printf("Instance lengths: %d,%d\n", l1, l2);
#endif
        for (;lcs == NULL && base + 2 * extra <= l2 + 1; extra *= 2) {
#ifdef DEBUG
                printf("Step: %d,%d\n", base, extra);
#endif
                lcs = band_align(base, extra, s1, s2);
        }
        if (lcs == NULL) {
                lcs = band_align(base, l1, s1, s2);
        }

        if (lcs != NULL) {
                printf("Longest subsequence:%s\n", lcs);
        }
}
