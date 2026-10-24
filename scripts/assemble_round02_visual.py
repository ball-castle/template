#!/usr/bin/env python3
"""Assemble exact-pixel inherited reviews and new full-size visual checks."""
from round02_common import ROOT,CATALOG,read_json,json_write,digest

mapping=read_json(CATALOG/'validation/visual_full_pixel_mapping.json')
pdf=ROOT/'板子打印版/ACM_ICPC_铜牌稳妥覆盖_算法与模型_V2.pdf'
assert mapping['pdf_sha256'].lower()==digest(pdf)
updates={}
for name in ('a','b','c'):
    report=read_json(CATALOG/'validation'/('visual_full_recheck_'+name+'.json'))
    assert report['pdf_sha256'].lower()==digest(pdf)
    for row in report['pages']:
        assert row['status'].startswith('PASS'),row
        updates[row['page']]=row
pages=[]
for row in mapping['pages']:
    if row['page'] in updates:
        pages.append(dict(updates[row['page']],method='Current PDF page actually viewed at original resolution'))
    else:
        assert row['status']=='PASS' and row['matched_review'],row
        pages.append(dict(row,method='Actually viewed prior body; current RGB body pixels identical; footer number checked separately'))
check=read_json(ROOT/'_qa/ACM_ICPC_铜牌稳妥覆盖_算法与模型_V2.round02.check.json')
assert check['structural_check']=='PASS' and check['pdf_sha256']==digest(pdf)
assert [r['page'] for r in pages]==list(range(1,check['pages']+1))
json_write(CATALOG/'validation/visual_full_final.json',{
    'status':'PASS','pdf_sha256':digest(pdf),'page_count':len(pages),'pages':pages,
    'method':'All 276 first-pass pages viewed at original 130dpi. After fixing short tails and adding STL appendix, 246 final page bodies match reviewed RGB pixels exactly; all 34 changed/new bodies plus footer samples 1/140 were viewed again uncropped. Every final footer number and index position checked against PDF.',
    'structural_evidence':'notes/round02/validation/print_full.json',
    'remaining_issues':[]})
print('PASS: all final V2 pages have visual evidence')
