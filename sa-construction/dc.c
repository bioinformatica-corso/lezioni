/*
  Copyright (C) 2014 by Gianluca Della Vedova


*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

static bool cmp2(unsigned int a1, unsigned int a2, unsigned int b1, unsigned int b2) {
        if (a1 == b1) return (a2 <= b2);
        return (a1 < b1);
}
static bool cmp3(unsigned int a1, unsigned int a2, unsigned int a3,   unsigned int b1, unsigned int b2, unsigned int b3) {
        if (a1 != b1) return(a1 < b1);
        if (a2 != b2) return(a2 < b2);
        return (a3 < b3);
}

/*
  Get two arrays a, b with n elements
  The array a stores the indices, while the values are in the array r
  Each element is smaller than or equal to alphabet_size
  Notice that r can contain many more elements than a

  Sort the indexed elements of r and put the corresponding indices in b
  Use an array c that counts the number of elements with any given value.
*/
static void pass1(unsigned int* a, unsigned int* b, unsigned int* r, unsigned int n, unsigned int alphabet_size)
{
        unsigned int c[alphabet_size + 1];
        for (unsigned int i = 0; i <= alphabet_size; i++) c[i] = 0;

        /* count */
        for (unsigned int i = 0; i < n; i++)
                c[r[a[i]]]++;
        /* Transform the count into a sequence of positions, via a prefix sum */
        for (unsigned int i = 0, sum = 0; i <= alphabet_size; i++) {
                unsigned int t = c[i];
                c[i] = sum;
                sum += t;
        }
        for (unsigned int i = 0; i < n; i++)
                b[c[r[a[i]]]++] = a[i];
}

// find the suffix array SA of s[0..n-1] in {1..alphabet_size}^n
// require s[n]=s[n+1]=s[n+2]=0, n>=2

/*
  Notice that this implementation recurses on the elements with mod 3 equal to 1
  or 2 (stored in SA12) and computes from the sorted SA12 the sorted SA0, array
  of the elements with mod 3 = 0.

  The s0 and s12 arrays contains the positions of the corresponding elements of
  SA0, SA12.

  The s array is the alphabet
*/
static void suffix_array(unsigned int* s, unsigned int* SA, unsigned int n, unsigned int alphabet_size) {
        unsigned int n0 = (n + 2) / 3; // num elements with modulus 0
        unsigned int n1 = (n + 1) / 3; // num elements with modulus 1
        unsigned int n2 = (n + 0) / 3; // num elements with modulus 2
        unsigned int n02 = n0 + n2;
        unsigned int s12[n02 + 3];
        s12[n02] = 0; s12[n02+1] = 0; s12[n02+2] = 0;
        unsigned int SA12[n02 + 3];
        SA12[n02] = 0; SA12[n02+1] = 0; SA12[n02+2] = 0;
        unsigned int s0[n0];
        unsigned int SA0[n0];


        /* Compute the new alphabet and aggregate all triplets into single symbols */

        unsigned int limit = (n % 3 == 1) ? n + 1 : n;
        for (unsigned int i=0, j=0; i < limit; i++)
                if (i%3 != 0) s12[j++] = i;
        /* Now s12 contains the positions with % 3 equal to 1 or 2 */


        // least significant bit radix sort the mod 1 and mod 2 triples
        pass1(s12 , SA12, s+2, n02, alphabet_size);
        pass1(SA12, s12 , s+1, n02, alphabet_size);
        pass1(s12 , SA12, s  , n02, alphabet_size);
        /* Now SA12 contains a sorted array of all triplets */


        /* compute the new alphabet of the suffixes of SA12, consisting of all
           triplets found.
           Since the triplets in SA12 are sorted, a new symbol
           appears iff the current triplet is different from the
           preceding triplet.
        */

        unsigned int name = 0, c0 = -1, c1 = -1, c2 = -1;
        for (unsigned int i = 0; i < n02; i++) {
                /*
                  Given the suffix starting in position SA12[i], the heading
                  triple consists of the symbols s[SA12[i]] s[SA12[i]+1] s[SA12[i]+2]
                */
                if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
                        name++;
                        c0 = s[SA12[i]];
                        c1 = s[SA12[i]+1];
                        c2 = s[SA12[i]+2];
                }
                /*
                  The first n0 elements of s12 are reserved for suffixes where
                  % 3 == 1, while the other part of s12 (starting with position
                  n0) is reserved for suffixes where % 3 == 1.
                */
                unsigned int pos = SA12[i] / 3; // integer division
                if (SA12[i] % 3 == 2) pos += n0;
                s12[pos] = name;
        }
        /*
          End of alphabet change.
          Invariant: SA12[i] < n0 iff i %3 == 1 */

        /*
          Base case: all alphabet symbols are distinct.
          In this case the sorted alphabet is also the suffix array.
        */
        if (name >= n02) {
                for (unsigned int i = 0; i < n02; i++)
                        SA12[s12[i] - 1] = i;
        } else {
                suffix_array(s12, SA12, n02, name);
// store unique names in s12 using the suffix array
                for (unsigned int i = 0; i < n02; i++)
                        s12[SA12[i]] = i + 1;
        }

        /* Extract and sort s0 from SA12 */
        /* the sorting is obtained by a radix sort pass on the first character */
        for (unsigned int i=0, j=0; i < n02; i++)
                if (SA12[i] < n0)
                        s0[j++] = 3*SA12[i];
        pass1(s0, SA0, s, n0, alphabet_size);

        /* Merge phase.
           The input sorted arrays are SA0 and SA12 */
        for (unsigned int p = 0, t = n0 - n1, k = 0; k < n; k++) {
                unsigned int i = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
                /* i is the position inside SA12 of the i-th suffix of SA12 */
                unsigned int j = SA0[p]; // pos of current offset 0 suffix
                if (SA12[t] < n0 ?
                    cmp2(s[i], s12[SA12[t] + n0],
                         s[j], s12[j/3]) :
                    cmp3(s[i],s[i+1],s12[SA12[t]-n0+1],
                         s[j],s[j+1],s12[j/3+n0]))
                { // suffix from SA12 is smaller
                        SA[k] = i; t++;
                        if (t == n02) // done --- only SA0 suffixes left
                                for (k++; p < n0; p++, k++)
                                        SA[k] = SA0[p];
                } else {
                        SA[k] = j; p++;
                        if (p == n0)  // done --- only SA12 suffixes left
                                for (k++; t < n02; t++, k++)
                                        SA[k] = (SA12[t] < n0 ? SA12[t]*3+1 : (SA12[t] - n0)*3+2);
                }
        }
}

static void compute_sa(unsigned int* sa, const char* str, size_t len) {
        unsigned int str2[len];
/*
  We do not copy the last element of str, since it is a "\n"
*/
        unsigned int i = 0;
        for (; i < len && str[i] != '\n'; i++)
                str2[i] = str[i];
        suffix_array(str2, sa, i, 4);
}

int main(void) {
        char *line = NULL;
        int read;
        size_t len;
        read = getline(&line, &len, stdin);
        if (read < 0) exit(EXIT_FAILURE);
        unsigned int sa[len];
        compute_sa(sa, line, len);
        printf(" Index         SA\n");
        for (unsigned int i = 0; i < len; i++)
                printf("%12u : %12u\n", i, sa[i]);
        free(line);
}
