import os
import json
with open(os.path.join(os.path.dirname(__file__), "to.json"), "r") as f:
    to = json.load(f)
