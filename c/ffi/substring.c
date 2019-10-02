/*
  Copyright (C) 2014-2015 by Gianluca Della Vedova


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

/*
  return the length of the longest common substring of s1, s2
*/
uint32_t substr2(char* s1, char* s2) {
        uint32_t n1 = strlen(s1);
        uint32_t n2 = strlen(s2);
        for (uint32_t l = min(n1, n2); l > 0; l--)
                for (uint32_t i1 = 0; i1 <= n1 - l; i1++)
                        for (uint32_t i2 = 0; i2 <= n2 - l; i2++)
                                if (strncmp(s1 + i1, s2 + i2, l) == 0)
                                        return l;
        return 0;
}


int main(int argc, char **argv) {
        char s1[100];
        char s2[100];
        // char* s1, s2;
        int n1 = scanf("%s", &s1);
        int n2 = scanf("%s", &s2);
        if (n1 == 0 || n2 == 0) {
                fprintf(stderr, "Insert 2 strings\n");
                exit(EXIT_FAILURE);
        }

        printf("Length: %d\n", substr2(s1, s2));
}
