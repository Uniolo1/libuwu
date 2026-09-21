#!/usr/bin/env python3
import json
import sys

if len(sys.argv) > 1:
    input_file = sys.argv[1]
else:
    input_file = "to.json"

with open(input_file, "r", encoding="utf-8") as f:
    data = json.load(f)

print(json.dumps(data, indent="\t", ensure_ascii=False, sort_keys=True))
