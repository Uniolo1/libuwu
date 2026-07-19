#!/usr/bin/env python3

import sys
import random
import readline
import os
import atexit
from data import to

stutter_chance = 6 # 1 in 6

# print to stderr
def eprint(*args, sep=' ', end='\n', flush=False):
    print(*args, sep=sep, end=end, flush=flush, file=sys.stderr)

# case insensitive substitution
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

        cat = False # 'cat' stands for continue after <I already forgot>
        if t.lower() in to:
            t = case_replace(t, t, to[t.lower()])
            cat = True
        elif '://' in t:
            a.append(t)
            continue

        if stutter_chance != 0:
            if random.randint(1, stutter_chance) == 1 and len(t) > 0:
                t = f"{t[0]}-{t}"

        if cat:
            a.append(t)
            continue

        t = cr(t, "r", "w")
        t = cr(t, "l", "w")

        a.append(t)

    return " ".join(a)

#print(uwuify(sys.argv[1]))
if __name__ == "__main__":
    # setup history file
    hf = os.path.expanduser("~/.uwuify")
    if os.path.isfile(hf):
        readline.read_history_file(hf)

    # save history file atexit
    def save_history():
        readline.write_history_file(hf)
    atexit.register(save_history)

    flag = None
    text = []

    for i in sys.argv[1:]:
        if flag != None:
            if flag in ["-s", "--studder"]:
                try:
                    if float(i) % 1 != 0:
                        eprint("Expected a integer")
                        sys.exit(1)
                    stutter_chance = int(i)
                    if stutter_chance < 0:
                        eprint("Expected a number greater than or equal to '0'")
                        sys.exit(1)
                except ValueError:
                    eprint("Expected a valid integer")
                    sys.exit(1)
            else:
                print(f"Unknown flag of '{flag}'")
                sys.exit(1)
            flag = None
        elif i.startswith('-'):
            flag = i
        else:
            text.append(i)

    if len(text) == 0:
        while True:
            print(uwuify(input('>>> ')))


    print(uwuify(" ".join(text)))
