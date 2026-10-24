#!/usr/bin/env python3
"""Reuse reviewed pixels only when exact body pixels match; list pages needing new inspection."""
import hashlib
from PIL import Image
from round02_common import ROOT,CATALOG,read_json,json_write,digest

def body_hash(path):
    with Image.open(path) as im:
        # At 130dpi the A4 text body ends above y=1455; crop only footer/page number.
        # Footer is separately covered by the structural/page-index check and final visual samples.
        body=im.convert('RGB').crop((0,0,im.width,im.height-55))
        return hashlib.sha256(body.tobytes()).hexdigest()

approved={}
for path in (CATALOG/'validation').glob('visual_full_[0-9]*.json'):
    report=read_json(path)
    for page in report['pages']:
        if page['status'].startswith('PASS'):
            number=page['page'];image=ROOT/'_qa/round02_full_final'/f'page-{number:03}.png'
            approved[body_hash(image)]={'page':number,'report':path.relative_to(ROOT).as_posix(),
                                       'prior_pdf_sha256':report['pdf_sha256']}
pages=[];unmatched=[]
for path in sorted((ROOT/'_qa/round02_full_release').glob('page-*.png')):
    number=int(path.stem.split('-')[-1]);h=body_hash(path)
    match=approved.get(h)
    pages.append({'page':number,'body_sha256':h,'image_sha256':digest(path),
                  'status':'PASS' if match else 'REVIEW_REQUIRED','matched_review':match})
    if not match:unmatched.append(number)
result={'pdf_sha256':digest(ROOT/'板子打印版/ACM_ICPC_铜牌稳妥覆盖_算法与模型_V2.pdf'),
        'method':'Exact RGB pixel equality excluding only bottom 55px footer; prior pages actually viewed at original size. Changed/unmatched pages require new view_image inspection.',
        'pages':pages,'unmatched':unmatched}
json_write(CATALOG/'validation/visual_full_pixel_mapping.json',result)
print('Unmatched pages:',unmatched)
print('Identical reviewed bodies:',len(pages)-len(unmatched),'/',len(pages))
