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


def generate(path: Path, size: int):
    keys = list(range(0, size * 10, 10))
    random.shuffle(keys)
    with path.open("w") as file:
        for k in keys:
            file.write(str(k))
            file.write(random.choice((",", ",", ",", "\n")))


generate(Path("numbers.csv"), 10_000)
