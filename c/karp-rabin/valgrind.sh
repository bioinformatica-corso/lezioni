#!/bin/bash

./karp-rabin --text=tests/.data/testo.1.fa.gz --rounds=5 --pattern=CACTGCC
valgrind -v ./karp-rabin --text=tests/.data/testo.1.fa.gz --rounds=5 --pattern=CACTGCC
valgrind -v --leak-check=full --show-leak-kinds=all ./karp-rabin --text=tests/.data/testo.1.fa.gz --rounds=5 --pattern=CACTGCC
valgrind -q --tool=callgrind --dump-instr=yes  ./karp-rabin --text=tests/.data/testo.1.fa.gz --rounds=5 --pattern=CACTGCC
