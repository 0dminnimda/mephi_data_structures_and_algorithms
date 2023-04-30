from __future__ import annotations
import platform

import random
import re
import shlex
import string
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import List


def run(command: str, input: str) -> str:
    return subprocess.check_output(shlex.split(command), input=input, encoding="utf-8")


def random_string(
    min_len: int = 5,
    max_len: int = 25,
    alphabet: str = string.digits + string.ascii_letters + " \t",
) -> str:
    return "".join(
        random.choice(alphabet) for _ in range(random.randint(min_len, max_len))
    )


def create_tree(path: Path, size: int) -> dict[int, str]:
    items = list(range(0, size*10, 10))
    random.shuffle(items)
    tree = {items[i]: random_string() for i in range(size)}
    path.write_text("\n".join(f"{k}\n{v}" for k, v in tree.items()))
    return tree


@dataclass
class Test:
    graph_file: str
    renew: bool
    operations: list[str] = field(default_factory=list)

    def __post_init__(self) -> None:
        self.add_reset()

    def add_reset(self) -> "Test":
        self.operations.extend(["reset", "import_file", self.graph_file])
        return self

    def add(self, *item) -> "Test":
        if self.renew:
            self.add_reset()
        self.operations.extend(
            ["clock_zero", *(str(sub) for sub in item), "clock_time"]
        )
        return self

    def to_str(self) -> str:
        return "\n".join(self.operations + ["quit"])


def make_tests(root: Path, size: int, per_operation: int = 2**9) -> list[Test]:
    file = root / f"graph_{size}"

    tree = create_tree(file, size)
    tree_keys = list(tree.keys())

    test = Test(graph_file=file.as_posix(), renew=False)

    for i in range(per_operation):
        test.add("find", random.choice(tree_keys))

    return [test]


def run_tests(program: str, tests: List[Test]) -> list[str]:
    return [run(program, test.to_str()) for test in tests]


def analyze(outputs: list[str]) -> list[float]:
    pattern = re.compile(r"Time: (\d+\.\d+) seconds")

    result = []
    for output in outputs:
        times = [float(match.group(1)) for match in pattern.finditer(output)]
        result.append(sum(times) / len(times))
    return result


def main() -> None:
    program = "main.out"
    if platform.system() != "Windows":
        program = "./" + program

    root = Path("tests")
    if not root.exists():
        root.mkdir()

    tests = make_tests(root, 2**10, 2**10)
    outputs = run_tests(program, tests)
    print(analyze(outputs))


if __name__ == "__main__":
    main()
