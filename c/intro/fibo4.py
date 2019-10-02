#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description='Fibonacci.')
parser.add_argument('limit', metavar='N', type=int, nargs=1,
                    help='how many fibonacci numbers to compute')
args = parser.parse_args()

def fib():
    a,b = 1,1
    while True:
        yield a
        a, b = b, a + b

for index, f in enumerate(fib()):
     print(f)
     if index == args.limit[0] - 1:
         break
