"""Compile every canonical standalone recipe and check the documented I/O."""
from pathlib import Path
import json
import re
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
manifest = json.loads((root / "notes/round02/segment_sources.json").read_text(encoding="utf-8"))
compiler = shutil.which("g++")
if compiler is None:
    raise SystemExit("g++ is required")
def flatten(path):
    """Exactly expand the local includes used by the printed recipe."""
    out = []
    for line in path.read_text(encoding="utf-8").splitlines():
        if line == "#pragma once":
            continue
        include = re.fullmatch(r'#include "([^"]+)"', line)
        out.append(flatten(path.parent / include.group(1)) if include else line)
    return "\n".join(out) + "\n"

with tempfile.TemporaryDirectory(prefix="r2_segment_examples_") as work:
    for ex in manifest["examples"]:
        printed = Path(work) / (ex["name"] + "_printed.cpp")
        printed.write_text(flatten(root / ex["source"]), encoding="utf-8")
        for mode, source in [("direct", root / ex["source"]), ("printed", printed)]:
            executable = Path(work) / (ex["name"] + "_" + mode + ".exe")
            cmd = [compiler, "-std=gnu++20", "-O2", "-Wall", "-Wextra", "-Wshadow",
                   str(source), "-o", str(executable)]
            compiled = subprocess.run(cmd, text=True, capture_output=True, check=True)
            if compiled.stderr.strip():
                raise AssertionError(ex["name"] + ": compiler warnings\n" + compiled.stderr)
            result = subprocess.run([str(executable)], input=ex["input"],
                                    text=True, capture_output=True, check=True, timeout=5)
            if result.stdout != ex["output"]:
                raise AssertionError(
                    f"{ex['name']}: expected {ex['output']!r}, got {result.stdout!r}")
            if result.stderr:
                raise AssertionError(ex["name"] + ": unexpected stderr " + result.stderr)
        print("PASS", ex["name"], "direct + printed")
print(f"PASS {len(manifest['examples'])} recipes in both source forms and documented I/O")
