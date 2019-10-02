#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description='Fibonacci.')
parser.add_argument('limit', metavar='N', type=int, nargs=1,
                    help='how many fibonacci numbers to compute')
args = parser.parse_args()


fib = [1, 1]
for x in range(2, args.limit[0]):
    fib.append(fib[-1] + fib[-2])

print(fib)
