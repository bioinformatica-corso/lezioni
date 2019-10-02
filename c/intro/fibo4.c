/* Compile with:
   make fibo CFLAGS="-g -Wall -std=gnu11 -O3"
   Mostly taken from 21st Century C (Ben Klemens)
*/
#include <stdio.h>
#include <stdlib.h>

unsigned long long int fibonacci(void);

unsigned long long int fibonacci() {
        static unsigned long long int first = 1;
        static unsigned long long int second = 1;
        unsigned long long int out = first + second;
        first = second;
        second = out;
        return out;
}

int main(int argc, char **argv) {
        unsigned int limit = atoi(argv[1]);
        for (unsigned int n = 0; n < limit; n++)
                printf("%d ", fibonacci());
        printf("\n");
}
