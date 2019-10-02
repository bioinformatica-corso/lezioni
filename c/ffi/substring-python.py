#!/usr/bin/env python3

import itertools
import sys

strings = [b'The licenses for most software and other practical works are designed to take away your freedom to share and change the works.  By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.  We, the Free Software Foundation, use the GNU General Public License for most of our software; it applies also to any other work released this way by its authors.  You can apply it to your programs, too.',
           b'When we speak of free software, we are referring to freedom, not price.  Our General Public Licenses are designed to make sure that you have the freedom to distribute copies of free software (and charge for them if you wish), that you receive source code or can get it if you want it, that you can change the software or use pieces of it in new free programs, and that you know you can do these things.',
           b'To protect your rights, we need to prevent others from denying you these rights or asking you to surrender the rights.  Therefore, you have certain responsibilities if you distribute copies of the software, or if you modify it: responsibilities to respect the freedom of others.  For example, if you distribute copies of such a program, whether gratis or for a fee, you must pass on to the recipients the same freedoms that you received.  You must make sure that they, too, receive or can get the source code.  And you must show them these terms so they know their rights.',
           b'Developers that use the GNU GPL protect your rights with two steps: (1) assert copyright on the software, and (2) offer you this License giving you legal permission to copy, distribute and/or modify it.'
]



def substr2(s1, s2):
    for l in range(min(len(s1), len(s2)), 0, -1):
        for i1 in range(len(s1) + 1):
            for i2 in range(len(s2) - l + 1):
                if (s1[i1 : i1 + l] == s2[i2 : i2 + l]):
                    return l;
    return 0

print(max([substr2(x, y) for (x, y) in itertools.combinations(strings, 2)]))
