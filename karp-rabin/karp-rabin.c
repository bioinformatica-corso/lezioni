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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "karp-rabin.h"
#include "cmdline.h"
#include "klib/kseq.h"
KSEQ_INIT(gzFile, gzread)

static bool primep(const uint32_t n) {
        const uint32_t bases32[] = {2, 7, 61};
        return efficient_mr32(bases32, 3, n);
}

static char* read_text(char* filename) {
        gzFile fp;
        kseq_t *seq;
        fp = gzopen(filename, "r");
        assert(fp != NULL && "Could not open fasta file\n");
        seq = kseq_init(fp);
        assert(kseq_read(seq) >= 0);
        gzclose(fp);
        return seq->seq.s;
        // kseq_destroy(seq);
}

int main(int argc, char **argv) {
        static struct gengetopt_args_info args_info;
        assert(cmdline_parser(argc, argv, &args_info) == 0);
        assert(args_info.inputs_num >= 2);
        char* pattern = strdup(args_info.pattern);
        char* text = read_text(args_info.filename);
}
