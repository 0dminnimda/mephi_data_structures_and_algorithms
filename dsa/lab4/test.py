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


def run(command: str, input: str) -> str:
    return subprocess.check_output(
        shlex.split(command), input=input, encoding="utf-8", stderr=subprocess.STDOUT
    )


def random_string(
    min_len: int = 5,
    max_len: int = 25,
    alphabet: str = string.digits + string.ascii_letters + " \t",
) -> str:
    return "".join(
        random.choice(alphabet) for _ in range(random.randint(min_len, max_len))
    )


def create_tree(path: Path, size: int) -> dict[int, str]:
    items = list(range(0, size * 10, 10))
    random.shuffle(items)
    tree = {items[i]: random_string() for i in range(size)}
    path.write_text("\n".join(f"{k}\n{v}" for k, v in tree.items()))
    return tree


@dataclass
class Test:
    graph_file: str = ""
    renew: bool = False
    operations: list[str] = field(default_factory=list)

    def __post_init__(self) -> None:
        self.add_reset()

    def setup(self, graph_file: str, renew: bool) -> Test:
        Test.__init__(self, graph_file, renew)
        return self

    def extend(self, *item) -> Test:
        self.operations.extend(str(sub) for sub in item)
        return self

    def add_reset(self) -> Test:
        self.extend("reset", "import_file", self.graph_file)
        return self

    def add_timed(self, *item) -> Test:
        if self.renew:
            self.add_reset()
        self.extend("clock_zero", *item, "clock_time")
        return self

    def to_str(self) -> str:
        return "\n".join(self.operations + ["quit"])


Tests = defaultdict[str, Test]


def populate_tests(tests: Tests, file: Path, keys: list[int], iterations: int) -> None:
    test = tests["add"].setup(file.as_posix(), False)
    for i in range(iterations // 2):
        key = random.choice(keys) + 1
        test.add_timed("add", key, random_string())
        test.extend("delete", key)
    for i in range(iterations // 2):
        key = random.choice(keys)
        test.add_timed("add", key, random_string())
        test.extend("delete", key)

    test = tests["delete"].setup(file.as_posix(), True)
    for i in range(iterations // 2):
        test.add_timed("delete", random.choice(keys) + 1)
    for i in range(iterations // 2):
        test.add_timed("delete", random.choice(keys))

    test = tests["find"].setup(file.as_posix(), False)
    for i in range(iterations // 2):
        test.add_timed("find", random.choice(keys) + 1)
    for i in range(iterations // 2):
        test.add_timed("find", random.choice(keys))

    test = tests["max_diff"].setup(file.as_posix(), False)
    for i in range(iterations // 2):
        test.add_timed("max_diff", random.choice(keys) + 1)
    for i in range(iterations // 2):
        test.add_timed("max_diff", random.choice(keys))

    # it's pointless trying to mesure the traverse with the random args
    test = tests["output"].setup(file.as_posix(), False)
    for i in range(int(iterations**0.75)):
        test.add_timed("output")

    # I don't think that timing any of
    # dump_dot/image/import_file/clock_zero/clock_time/reset/quit
    # makes sence either


def make_tests(
    root: Path, size: int, iterations: int = 2**9, trees: int = 2**4
) -> Tests:

    size_root = root / f"graph_{size}"
    if not size_root.exists():
        size_root.mkdir()

    tests = Tests(Test)
    for i in range(trees):
        file = size_root / f"no_{i}"
        tree = create_tree(file, size)
        populate_tests(tests, file, list(tree.keys()), iterations)
    return tests


def run_tests(program: str, tests: Tests) -> dict[str, str]:
    return {
        name: run(program, test.to_str())
        for name, test in tests.items()
        if not print(f"Running {name}")
    }


def analyze(outputs: dict[str, str]) -> dict[str, float]:
    pattern = re.compile(r"Time: (\d+\.\d+) seconds")

    result = {}
    for name, output in outputs.items():
        times = [float(match.group(1)) for match in pattern.finditer(output)]
        result[name] = sum(times) / len(times)
    return result


def main() -> None:
    program = "main.out"
    if platform.system() != "Windows":
        program = "./" + program

    root = Path("tests")
    if not root.exists():
        root.mkdir()

    print("Preparing tests")
    tests = make_tests(root, 2**15, 2**8, 2**4)
    outputs = run_tests(program, tests)
    print(analyze(outputs))


if __name__ == "__main__":
    main()
