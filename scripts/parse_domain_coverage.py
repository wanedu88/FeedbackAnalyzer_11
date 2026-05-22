#!/usr/bin/env python3
"""Parse gcov .gcov files for FeedbackAnalyzer domain sources (Mission 2)."""
from __future__ import annotations

import re
import sys
from pathlib import Path

DOMAIN_SOURCES = {
    "ParseUtils.cpp",
    "Constants.cpp",
    "Filters.cpp",
    "TextAnalyzer.cpp",
    "Filters.h",
    "TextAnalyzer.h",
}

GCOV_DIRS = [
    "build/CMakeFiles/feedback_analyzer_tests.dir/src/cpp",
    "build/CMakeFiles/feedback_analyzer_tests.dir/tests",
]


def parse_gcov(path: Path) -> tuple[int, int, list[int]]:
    executed = missed = 0
    missed_lines: list[int] = []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        if line.startswith("#####:"):
            missed += 1
            m = re.search(r":\s*(\d+):", line)
            if m:
                missed_lines.append(int(m.group(1)))
        elif re.match(r"^\s*\d+:\s*\d+:", line):
            executed += 1
    return executed, missed, missed_lines


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    rows: list[tuple[str, int, int, float, list[int]]] = []

    for rel in GCOV_DIRS:
        d = root / rel
        if not d.is_dir():
            continue
        for gcov in sorted(d.glob("*.gcov")):
            if gcov.name not in {s + ".gcov" for s in DOMAIN_SOURCES}:
                continue
            ex, miss, miss_lines = parse_gcov(gcov)
            total = ex + miss
            pct = 100.0 * ex / total if total else 0.0
            rows.append((gcov.name.replace(".gcov", ""), ex, total, pct, miss_lines))

    if not rows:
        print("No domain .gcov files found. Run scripts/run_coverage.ps1 first.", file=sys.stderr)
        return 1

    print("Domain line coverage (gcov)")
    print("-" * 72)
    print(f"{'File':<22} {'Exec':>6} {'Total':>6} {'Cover':>8}  Missing lines")
    print("-" * 72)
    total_ex = total_all = 0
    below90: list[tuple[str, float, list[int]]] = []
    for name, ex, tot, pct, miss in rows:
        total_ex += ex
        total_all += tot
        flag = "  <-- <90%" if pct < 90.0 else ""
        print(f"{name:<22} {ex:6} {tot:6} {pct:7.1f}%{flag}  {miss[:12]}")
        if pct < 90.0:
            below90.append((name, pct, miss))
    overall = 100.0 * total_ex / total_all if total_all else 0.0
    print("-" * 72)
    print(f"{'TOTAL (domain)':<22} {total_ex:6} {total_all:6} {overall:7.1f}%")
    if below90:
        print("\nFiles below 90%:", ", ".join(f[0] for f in below90))
    else:
        print("\nAll domain files >= 90% line coverage.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
