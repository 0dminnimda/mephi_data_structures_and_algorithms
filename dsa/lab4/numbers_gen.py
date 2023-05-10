from __future__ import annotations

import platform
import random
import re
import shlex
import string
import subprocess
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path
from pprint import pformat
from itertools import accumulate


def make_lengths(keys: list[int]) -> list[int]:
    cur_length = 0
    lengths = []
    for _ in keys:
        if random.choice((0, 0, 0, 1)):
            lengths.append(cur_length)
            cur_length = 0
        else:
            cur_length += 1
    lengths.append(cur_length)
    return lengths


def generate(path: Path, size: int):
    keys = list(range(0, size * 10, 10))
    random.shuffle(keys)

    lengths = make_lengths(keys)
    max_length = max(lengths)

    with path.open("w") as file:
        i = 0
        for length in lengths:
            for _ in range(length):
                file.write(str(keys[i]))
                file.write(",")
                i += 1
            for _ in range(max_length - length):
                file.write(",")
            file.write("\n")


generate(Path("numbers.csv"), 10_000)
