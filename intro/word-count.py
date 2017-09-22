#!/usr/bin/env python3

import collections
import re

def wordcount(filename, n=10):
    text = open(filename).read().lower()
    counts = collections.Counter(re.findall('[a-z]+', text))
    for i, w in counts.most_common(n):
        print(i, w)

wordcount('sample.txt')
