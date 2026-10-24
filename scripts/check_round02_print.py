#!/usr/bin/env python3
"""Validate current sources, exact Word code, export binding and real PDF index."""
import argparse
import re
from pathlib import Path
from zipfile import ZipFile
from xml.etree import ElementTree as ET
from pypdf import PdfReader
from round02_common import ROOT, digest, read_json, json_write, expanded_source, code_digest

NS={'w':'http://schemas.openxmlformats.org/wordprocessingml/2006/main'}
W='{'+NS['w']+'}'

def check(path, save=True):
    m=read_json(path);stem=m['book']
    docx=path.with_name(stem+'.docx');pdf=path.with_name(stem+'.pdf')
    errors=[];pages={};reader=PdfReader(pdf)
    binding=read_json(pdf.with_name(pdf.name+'.binding.json'))
    if binding.get('docx_sha256')!=digest(docx) or binding.get('pdf_sha256')!=digest(pdf):
        errors.append('stale DOCX/PDF export binding')
    if len(reader.pages)!=m['planned_pages']:
        errors.append(f"pagination: actual {len(reader.pages)}, planned {m['planned_pages']}")
    text=[]
    for i,page in enumerate(reader.pages,1):
        value=page.extract_text() or '';text.append(value)
        folio=re.search(r'第\s*(\d+)\s*页',value)
        if not folio or int(folio.group(1))!=i:errors.append(f'page {i}: footer page number mismatch')
        if len(value.strip())<45:errors.append(f'page {i}: nearly empty')
        if '\ufffd' in value:errors.append(f'page {i}: replacement glyph')
        if abs(float(page.mediabox.width)-595.28)>2 or abs(float(page.mediabox.height)-841.89)>2:
            errors.append(f'page {i}: not A4')
    with ZipFile(docx) as z:xml=ET.fromstring(z.read('word/document.xml'))
    actual=[]
    for p in xml.findall('.//w:body/w:p',NS):
        style=p.find('w:pPr/w:pStyle',NS)
        if style is not None and style.get(W+'val')=='Code':
            actual.append(''.join(t.text or '' for t in p.findall('.//w:t',NS)))
    if actual!=m['expected_code_lines']:errors.append('DOCX exact code mismatch')
    # Word converts simple fields to complex fields when updating; inspect both forms.
    refs={}
    for row in xml.findall('.//w:tr',NS):
        cells=row.findall('w:tc',NS)
        if len(cells)!=2:continue
        field_text=' '.join(x.text or '' for x in cells[1].findall('.//w:instrText',NS))
        field_text+=' '+' '.join(x.get(W+'instr','') for x in cells[1].findall('.//w:fldSimple',NS))
        match=re.search(r'PAGEREF\s+(R2_\w+)',field_text)
        if match:
            result=''.join(x.text or '' for x in cells[1].findall('.//w:t',NS)).strip()
            refs[match.group(1)]=int(result) if result.isdigit() else None
    for section in m['sections']:
        ids=section.get('aliases',[section['catalog_id']])
        marker='【'+' / '.join(ids)+'】'
        compact=re.sub(r'\s+','',marker)
        matches=[i+1 for i,t in enumerate(text) if compact in re.sub(r'\s+','',t)]
        if not matches:
            errors.append('missing section '+section['id']);continue
        page=matches[0]
        if page!=section['planned_page']:
            errors.append(section['id']+' planned page mismatch')
        if section.get('indexed',True) and refs.get(section['bookmark'])!=page:
            errors.append(section['id']+' PAGEREF mismatch: '+str(refs.get(section['bookmark']))+' vs '+str(page))
        for key in ids:pages[key]=page
        if section['kind']=='module':
            lines,hashes=expanded_source(section['source'])
            if hashes!=section['source_hashes'] or code_digest(lines)!=section['expanded_code_sha256']:
                errors.append('changed canonical source '+section['source'])
        if section.get('example_source') and digest(ROOT/section['example_source'],text=True)!=section['example_sha256']:
            errors.append('changed example '+section['example_source'])
    result={'book':stem,'pages':len(reader.pages),'sections':pages,'code_lines':len(actual),
        'docx_sha256':digest(docx),'pdf_sha256':digest(pdf),'errors':errors,
        'structural_check':'PASS' if not errors else 'FAIL',
        'visual_check':'Required separately on every rendered page'}
    if save:json_write(ROOT/'_qa'/(stem+'.round02.check.json'),result)
    return result

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('manifest',type=Path,nargs='?');args=ap.parse_args()
    paths=[args.manifest] if args.manifest else sorted((ROOT/'板子打印版').glob('*.round02.json'))
    if not paths:raise SystemExit('No ROUND02 manifests')
    results=[check(path) for path in paths]
    for r in results:print(r['book'],r['pages'],r['structural_check'],r['errors'])
    raise SystemExit(0 if all(not r['errors'] for r in results) else 1)
