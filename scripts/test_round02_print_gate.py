#!/usr/bin/env python3
"""Regression checks: stale pair, altered printed code, altered source binding, index."""
from pathlib import Path
from tempfile import TemporaryDirectory
import shutil
from zipfile import ZipFile, ZIP_DEFLATED
from xml.etree import ElementTree as ET
from round02_common import ROOT, read_json, json_write, digest
from check_round02_print import check, NS, W

source=ROOT/'板子打印版/成都站_赛前小技巧与题型补丁_2026.round02.json'
assert check(source,False)['structural_check']=='PASS'
with TemporaryDirectory() as tmp:
    dest=Path(tmp)
    def reset():
        m=read_json(source);stem=m['book']
        for suffix in ('.docx','.pdf','.pdf.binding.json','.round02.json'):
            shutil.copyfile(source.with_name(stem+suffix),dest/(stem+suffix))
        return dest/(stem+'.round02.json'),dest/(stem+'.docx'),dest/(stem+'.pdf')
    path,docx,pdf=reset()
    binding=read_json(pdf.with_name(pdf.name+'.binding.json'))
    binding['docx_sha256']='0'*64
    json_write(pdf.with_name(pdf.name+'.binding.json'),binding)
    assert 'stale DOCX/PDF export binding' in check(path,False)['errors']
    path,docx,pdf=reset();m=read_json(path)
    module=next(s for s in m['sections'] if s['kind']=='module')
    module['expanded_code_sha256']='0'*64;json_write(path,m)
    assert any('changed canonical source' in x for x in check(path,False)['errors'])
    path,docx,pdf=reset()
    with ZipFile(docx) as z:parts={name:z.read(name) for name in z.namelist()}
    root=ET.fromstring(parts['word/document.xml'])
    for p in root.findall('.//w:body/w:p',NS):
        style=p.find('w:pPr/w:pStyle',NS)
        if style is not None and style.get(W+'val')=='Code':
            text=p.find('.//w:t',NS)
            if text is not None:text.text=(text.text or '')+' // altered';break
    parts['word/document.xml']=ET.tostring(root,encoding='utf-8',xml_declaration=True)
    with ZipFile(docx,'w',ZIP_DEFLATED) as z:
        for name,data in parts.items():z.writestr(name,data)
    binding=read_json(pdf.with_name(pdf.name+'.binding.json'));binding['docx_sha256']=digest(docx)
    json_write(pdf.with_name(pdf.name+'.binding.json'),binding)
    assert 'DOCX exact code mismatch' in check(path,False)['errors']
    path,docx,pdf=reset();m=read_json(path);m['sections'][0]['bookmark']='R2_nonexistent';json_write(path,m)
    assert any('PAGEREF mismatch' in x for x in check(path,False)['errors'])
print('PASS: current artifact and four deliberately broken bindings are distinguished')
