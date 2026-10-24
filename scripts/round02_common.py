"""Shared catalog, local-include expansion and LF-stable evidence for ROUND02."""
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / 'notes/round02'

def read_json(path):
    return json.loads(Path(path).read_text(encoding='utf-8-sig'))

def digest(path, text=False):
    data = Path(path).read_bytes()
    if text:
        data = data.replace(b'\r\n', b'\n')
    return hashlib.sha256(data).hexdigest()

def json_write(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8', newline='\n')

def catalogs():
    old = {m['id']: m for p in sorted((ROOT/'notes/modules').glob('*.json'))
           for m in [read_json(p)]}
    new = {m['id']: m for p in sorted((CATALOG/'modules').glob('*.json'))
           for m in [read_json(p)]}
    if old.keys() & new.keys():
        raise ValueError('ROUND02 IDs must be distinct from ROUND01 IDs')
    return old, new

def cards():
    result = []
    for path in sorted((CATALOG/'cards').glob('*.json')):
        data = read_json(path)
        if not isinstance(data, list):
            raise ValueError(f'{path}: expected card array')
        result.extend(data)
    ids = [x['id'] for x in result]
    if len(set(ids)) != len(ids):
        raise ValueError('Duplicate card IDs')
    return result

def as_text(value):
    if isinstance(value, list):
        return '；'.join(as_text(x) for x in value)
    if isinstance(value, dict):
        return '；'.join(f'{k}：{as_text(v)}' for k, v in value.items())
    return str(value)

def test_paths(module):
    return sorted(set(re.findall(r'tests/[A-Za-z0-9_/]+\.cpp', as_text(module['test']))))

def expanded_source(source):
    """Inline real local includes once; keep all other canonical source lines."""
    visited, sources, lines = set(), {}, []
    def visit(path):
        path = path.resolve()
        relative = path.relative_to(ROOT).as_posix()
        if path in visited:
            return
        visited.add(path)
        sources[relative] = digest(path, text=True)
        for line in path.read_text(encoding='utf-8-sig').splitlines():
            match = re.fullmatch(r'\s*#\s*include\s*"([^"]+)"\s*(?://.*)?', line)
            if match:
                dependency = path.parent / match.group(1)
                if not dependency.exists():
                    dependency = ROOT / match.group(1)
                visit(dependency)
            elif line.strip() != '#pragma once':
                lines.append(line)
    visit(ROOT/source)
    return lines, sources

def code_digest(lines):
    return hashlib.sha256(('\n'.join(lines)+'\n').encode('utf-8')).hexdigest()

def load_reports():
    return [read_json(ROOT/'build'/f'round02_{mode}'/'results.json')
            for mode in ('release','sanitizers')]

def verified(module, reports):
    _, sources = expanded_source(module['source'])
    for report in reports:
        if report.get('status') != 'PASS':
            return False
        for path, expected in sources.items():
            if report.get('sources', {}).get(path) != expected:
                return False
        for path in test_paths(module):
            if report.get('test_sources', {}).get(path) != digest(ROOT/path, text=True):
                return False
            _, dependencies = expanded_source(path)
            for dep, expected in dependencies.items():
                if report.get('test_dependencies', {}).get(dep) != expected:
                    return False
    return True
