#!/usr/bin/env python3
"""Check actual PDF pagination, source parity and artifact hashes after rendering."""
import argparse
import hashlib
import json
from pathlib import Path
import re
from zipfile import ZipFile
from xml.etree import ElementTree as ET
from pypdf import PdfReader

ROOT = Path(__file__).resolve().parents[1]
NS = {'w': 'http://schemas.openxmlformats.org/wordprocessingml/2006/main'}

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def check(manifest_path):
    manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
    prefix = manifest_path.name.removesuffix('.manifest.json')
    docx, pdf = manifest_path.with_name(prefix+'.docx'), manifest_path.with_name(prefix+'.pdf')
    reader = PdfReader(pdf)
    errors = []
    binding_path = pdf.with_name(pdf.name + '.binding.json')
    if not binding_path.exists():
        errors.append('missing DOCX/PDF export binding; export the current DOCX again')
    else:
        binding = json.loads(binding_path.read_text(encoding='utf-8-sig'))
        if binding.get('docx_sha256') != sha(docx) or binding.get('pdf_sha256') != sha(pdf):
            errors.append('stale DOCX/PDF export binding; export the current DOCX again')
    if len(reader.pages) != manifest['planned_pages']:
        errors.append(f"pagination: actual {len(reader.pages)}, planned {manifest['planned_pages']}")
    for number, page in enumerate(reader.pages, 1):
        txt = page.extract_text() or ''
        if len(txt.strip()) < 45:
            errors.append(f'page {number}: empty or little text')
        if '\ufffd' in txt:
            errors.append(f'page {number}: replacement glyph')
        width, height = float(page.mediabox.width), float(page.mediabox.height)
        if abs(width-595.28)>2 or abs(height-841.89)>2:
            errors.append(f'page {number}: not A4')
    for module in manifest['modules']:
        if sha(ROOT/module['source']) != module['sha256']:
            errors.append('changed source: '+module['source'])
        page = module['start_page']
        if page <= len(reader.pages) and module['id'] not in (reader.pages[page-1].extract_text() or ''):
            errors.append(f"index mismatch: {module['id']} expected on page {page}")
    expected = [line for module in manifest['modules']
                for line in (ROOT/module['source']).read_text(encoding='utf-8-sig').splitlines()]
    with ZipFile(docx) as z:
        xml = ET.fromstring(z.read('word/document.xml'))
    actual = []
    for p in xml.findall('.//w:body/w:p', NS):
        style = p.find('w:pPr/w:pStyle', NS)
        if style is not None and style.get('{'+NS['w']+'}val') == 'Code':
            actual.append(''.join(p.itertext()))
    if actual != expected:
        errors.append('DOCX source text parity mismatch')
    result = {'book':prefix, 'pages':len(reader.pages), 'source_lines':len(expected),
              'docx_sha256':sha(docx), 'pdf_sha256':sha(pdf), 'errors':errors,
              'structural_check':'PASS' if not errors else 'FAIL',
              'visual_check':'Required separately on all rendered pages'}
    print(json.dumps(result, ensure_ascii=False, indent=2))
    (ROOT/'_qa').mkdir(exist_ok=True)
    (ROOT/'_qa'/(prefix+'.check.json')).write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
    return not errors

if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('manifest', type=Path, nargs='?')
    args = ap.parse_args()
    paths = [args.manifest] if args.manifest else sorted((ROOT/'板子打印版').glob('*.manifest.json'))
    if not paths:
        raise SystemExit('No print manifest')
    passed = [check(path) for path in paths]
    raise SystemExit(0 if all(passed) else 1)
