#!/usr/bin/env python3
"""GNU++20 independent drivers, differential suites and source-hash evidence."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--sanitize', action='store_true')
    ap.add_argument('--p0', action='store_true', help='Emergency-book modules and P0 test cases only')
    ap.add_argument('--compiler', default=os.environ.get('CXX', 'g++'))
    args = ap.parse_args()
    mode = 'sanitizers' if args.sanitize else 'release'
    build = ROOT / 'build' / mode
    build.mkdir(parents=True, exist_ok=True)
    flags = ['-std=gnu++20', '-Wall', '-Wextra', '-Wshadow', '-g']
    if args.p0:
        flags.append('-DP0_ONLY')
    flags += (['-O1', '-fno-omit-frame-pointer', '-fsanitize=address,undefined', '-fno-pie', '-no-pie']
              if args.sanitize else ['-O2'])
    env = dict(os.environ)
    if args.sanitize:
        env['ASAN_OPTIONS'] = 'detect_leaks=1:halt_on_error=1'
        env['UBSAN_OPTIONS'] = 'halt_on_error=1:print_stacktrace=1'
    report = {'mode': mode, 'platform': platform.platform(), 'flags': flags,
              'compiler': subprocess.check_output([args.compiler, '--version'], text=True).splitlines()[0],
              'sources': {}, 'test_sources': {}, 'checks': [], 'status': 'FAIL'}
    modules = [json.loads(p.read_text(encoding='utf-8-sig'))
               for p in sorted((ROOT / 'notes/modules').glob('*.json'))]
    if args.p0:
        modules = [m for m in modules if m['priority'] == 'P0']
    if not modules:
        raise RuntimeError('No module metadata found')
    declared_tests = set()
    for module in modules:
        names = re.findall(r'tests/[A-Za-z0-9_]+\.cpp', str(module['test']))
        if not names:
            raise RuntimeError('Missing functional test declaration: '+module['id'])
        for name in names:
            if not (ROOT/name).is_file():
                raise RuntimeError('Missing declared test suite: '+name)
            declared_tests.add(name)
    def run(label, cmd, input_text=None):
        started = time.monotonic()
        p = subprocess.run(cmd, cwd=ROOT, input=input_text, text=True,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           env=env, timeout=240, encoding='utf-8', errors='replace')
        log = build / (label + '.log')
        log.write_text(p.stdout, encoding='utf-8')
        report['checks'].append({'name': label, 'command': cmd, 'exit_code': p.returncode,
                                'seconds': round(time.monotonic()-started, 3),
                                'output': p.stdout,
                                'log': str(log.relative_to(ROOT)).replace('\\', '/')})
        print(('PASS ' if p.returncode == 0 else 'FAIL ') + label, flush=True)
        if p.returncode:
            print(p.stdout[-6000:], flush=True)
            raise RuntimeError(label)
    try:
        for module in modules:
            source = ROOT / module['source']
            data = source.read_bytes()
            report['sources'][module['source']] = hashlib.sha256(data).hexdigest()
            label = 'standalone_' + module['id']
            driver = source
            if source.suffix != '.cpp':
                driver = build / (label + '.cpp')
                driver.write_text('#include "' + source.as_posix() + '"\nint main() { return 0; }\n', encoding='utf-8')
            binary = build / (label + ('.exe' if os.name == 'nt' else ''))
            run(label, [args.compiler, *flags, str(driver), '-o', str(binary)])
            if source.suffix == '.cpp':
                run(label + '_smoke', [str(binary)], '0\n')
        suites = sorted((ROOT / 'tests').glob('test_*.cpp'))
        if not suites:
            raise RuntimeError('No functional suites found')
        for suite in suites:
            label = suite.stem
            binary = build / (label + ('.exe' if os.name == 'nt' else ''))
            run(label + '_compile', [args.compiler, *flags, str(suite), '-o', str(binary)])
            run(label, [str(binary)])
            name = suite.relative_to(ROOT).as_posix()
            report['test_sources'][name] = hashlib.sha256(suite.read_bytes()).hexdigest()
        if not declared_tests.issubset(report['test_sources']):
            raise RuntimeError('A declared functional suite was not executed')
        report['status'] = 'PASS'
    finally:
        (build / 'results.json').write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding='utf-8')
    print(f"{mode}: {len(modules)} modules; {len(suites)} functional suites; PASS")

if __name__ == '__main__':
    main()
