#!/usr/bin/env python3

import sys
from data import to

def case_replace(input: str, from_: str, to: str) -> str:
    def apply_case(src: str, replacement: str) -> str:
        if not src:
            return replacement

        result = []

        for i, ch in enumerate(replacement):
            if i < len(src):
                template = src[i]
            else:
                template = src[-1]

            if template.isupper():
                result.append(ch.upper())
            elif template.islower():
                result.append(ch.lower())
            else:
                result.append(ch)

        return "".join(result)

    result = []
    i = 0

    while i < len(input):
        if input[i:i + len(from_)] == from_:
            result.append(apply_case(from_, to))
            i += len(from_)
        else:
            result.append(input[i])
            i += 1

    return "".join(result)
cr = case_replace

def uwuify(input: str) -> str:
    a = []
    for t in input.split(' '):
        if '://' in t:
            a.append(t)
            continue

        if t.lower() in to:
            a.append(case_replace(t, t, to[t.lower()]))
            continue

        t = cr(t, "r", "w")
        t = cr(t, "l", "w")

        a.append(t)

    return " ".join(a)

print(uwuify(sys.argv[1]))
