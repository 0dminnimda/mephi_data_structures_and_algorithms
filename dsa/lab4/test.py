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

    def add_reset(self) -> Test:
        self.operations.extend(["reset", "import_file", self.graph_file])
        return self

    def add(self, *item) -> Test:
        if self.renew:
            self.add_reset()
        self.operations.extend(
            ["clock_zero", *(str(sub) for sub in item), "clock_time"]
        )
        return self

    def to_str(self) -> str:
        return "\n".join(self.operations + ["quit"])


# def tests_from_files(
#     files: list[Path], tree_keys: list[int], iterations: int
# ) -> Iterable[Test]:

#     test = Test(graph_file="", renew=False)
#     for file in files:
#         test.graph_file = file.as_posix()
#         test.add_reset()
#         for i in range(iterations):
#             test.add("find", random.choice(tree_keys))
#     yield test


# def make_tests(
#     root: Path, size: int, trees: int = 2**4, iterations: int = 2**9
# ) -> Iterable[Test]:

#     file = root / f"graph_{size}"
#     tree = create_tree(file, size)
#     files = [file.with_stem(f"{file.stem}{i}") for i in range(trees)]
#     return tests_from_files(files, list(tree.keys()), iterations)


Tests = defaultdict[str, Test]


def populate_tests(tests: Tests, file: Path, keys: list[int], iterations: int) -> None:

    test = tests["find"].setup(file.as_posix(), False)
    for i in range(iterations):
        test.add("find", random.choice(keys))


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
    return {name: run(program, test.to_str()) for name, test in tests.items()}


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

    tests = make_tests(root, 2**10, 2**8, 2**4)
    outputs = run_tests(program, tests)
    print(analyze(outputs))


if __name__ == "__main__":
    main()
