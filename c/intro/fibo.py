#!/usr/bin/env python3

fib = [1, 1]
for x in range(2, 10):
    fib.append(fib[-1] + fib[-2])

print(fib)
