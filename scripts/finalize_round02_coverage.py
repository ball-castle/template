#!/usr/bin/env python3
"""Bind topic coverage to tested current code and actual, visually checked PDFs."""
from collections import Counter
from round02_common import ROOT, CATALOG, catalogs, load_reports, verified, read_json, json_write, as_text

old,new=catalogs();modules=old|new;reports=load_reports()
assert all(verified(m,reports) for m in modules.values()),'Stale functional evidence'
books={}
for kind,stem in [('patch','成都站_赛前小技巧与题型补丁_2026'),
                  ('full','ACM_ICPC_铜牌稳妥覆盖_算法与模型_V2')]:
    result=read_json(ROOT/'_qa'/(stem+'.round02.check.json'))
    assert result['structural_check']=='PASS'
    visual=read_json(CATALOG/'validation'/('visual_'+kind+'_final.json'))
    assert visual['status']=='PASS' and visual['pdf_sha256'].lower()==result['pdf_sha256'].lower()
    assert sorted(x['page'] for x in visual['pages'])==list(range(1,result['pages']+1))
    books[kind]=result
    json_write(CATALOG/'validation'/('print_'+kind+'.json'),result)

def locations(ids,kind):
    lookup=books[kind]['sections'];result=[]
    for key in ids:
        if key in lookup:result.append(lookup[key]);continue
        for m in new.values():
            if key in m.get('replaces',[]) and m['id'] in lookup:result.append(lookup[m['id']])
    return sorted(set(result))

def safe(value):return as_text(value).replace('|','／').replace('\n',' / ')

stl=read_json(CATALOG/'stl_coverage.json')
for row in stl:
    if row['group'][0] in ('A','B'):
        tip='TIP_'+row['id']
        if tip not in row['print_ids']:row['print_ids'].append(tip)
    row['print_locations']=[{'book':kind,'pages':locations(row['print_ids'],kind)} for kind in books]
    assert locations(row['print_ids'],'full'),row['id']+' missing printed content'
    assert row['readiness'] and all(row['checks'][k] for k in ('scenario','usable_code','complexity','pitfalls','test_or_example')),row['id']
    assert all(verified(modules[k],reports) for k in row['code_refs']),row['id']
    row['checks']['print_position']=True;row['status']='VERIFIED';row['print_status']='VERIFIED'
    row['readiness_note']='六项均已核对；当前源码/测试双平台哈希与真实PDF页码、全页视觉QA绑定。'
json_write(CATALOG/'stl_coverage.json',stl)
out=['# STL 与基础数据结构逐项验收','',
     f'共 {len(stl)} 项，全部 VERIFIED。每项均有场景、实际调用、复杂度、易错点、测试和真实打印位置；不是按README是否出现名字判断。',
     '',f"打印位置：补丁 {books['patch']['pages']} 页；V2 {books['full']['pages']} 页。表中数字为该PDF印刷页码。",'',
     '| 知识点 / 状态 | 何时使用与实际调用 | 复杂度与易错 | 验证证据 | 打印位置 |','|---|---|---|---|---|']
for r in stl:
    out.append('| '+safe(r['topic'])+' / VERIFIED | '+safe(r['scenario'])+'<br>'+safe(r['actual_calls'])+
      ' | '+safe(r['complexity'])+'<br>'+safe(r['pitfalls'])+' | '+safe(r['validation'])+
      ' | '+safe(r['print_locations'])+' |')
out+=['','源码和精确摘录路径、六项布尔验收与测试字段见 [stl_coverage.json](notes/round02/stl_coverage.json)。',
       '原 `read.cpp` 完全保留；IO03只是单独的cin字符串转i128辅助。']
(ROOT/'STL_AND_DATA_STRUCTURE_COVERAGE.md').write_text('\n'.join(out)+'\n',encoding='utf-8',newline='\n')

data=read_json(CATALOG/'bronze_coverage.json')
for row in data['rows']:
    row['print_pages']={kind:locations(row['print_ids'],kind) for kind in books}
    if not row['required']:
        row['status']='OPTIONAL';continue
    assert row['readiness'] and row['print_pages']['full'],row['id']
    assert all(verified(modules[k],reports) for k in row['code_refs']),row['id']
    row['in_word_pdf']=True
    row['status']='VERIFIED_CODE' if row['code_status']=='VERIFIED_CODE' else 'MODEL_ONLY'
data['status']='VERIFIED_WITH_EXPLICIT_MODEL_BOUNDARIES'
data['scope']='ACM02/02A/02B最终逐项覆盖；用户后续原read与旧代码直接复用要求优先。'
data['verification_basis']='统一release与ASan/UBSan当前源码/测试/依赖哈希；真实PDF索引和逐页视觉检查。'
data['print_policy']='所有必需知识点已绑定V2真实页码；MODEL_ONLY为要求讲解的框架/概念，不冒充通用代码。'
data['summary']['verified_print_rows']=sum(r['in_word_pdf'] for r in data['rows'])
data['summary']['status_counts']=dict(Counter(r['status'] for r in data['rows']))
data['mandatory_gaps']=[]
json_write(CATALOG/'bronze_coverage.json',data)
out=['# 铜牌知识覆盖矩阵','',
     '按需求逐项验收。VERIFIED_CODE是当前可复用代码与测试；MODEL_ONLY是已讲清的概念/证明框架，相关组件不等于万能解法；OPTIONAL明确不纳入铜牌主线。',
     '',safe(data['summary']),'',
     '基础STL每个API的额外用法/复杂度/坑也已进入V2末尾附录；逐项位置见 [STL验收](STL_AND_DATA_STRUCTURE_COVERAGE.md)。',
     '', '| 知识点 | 状态 | 场景 / 证明 | 代码与调用例 | 测试 / 复杂度 / 坑 | PDF页码 |','|---|---|---|---|---|---|']
for r in data['rows']:
    out.append('| '+safe(r['id']+' '+r['topic'])+' | '+r['status']+' | '+safe(r['scene'])+'<br>'+safe(r['proof'])+
       ' | '+safe(r['code_refs'])+'<br>'+safe(r['example'])+' | '+safe(r['test'])+'<br>'+safe(r['complexity'])+
       '<br>'+safe(r['pitfalls'])+' | '+safe(r['print_pages'])+' |')
out+=['','## 模型边界','']
for r in data['intentional_model_boundaries']:
    out.append('- '+r['topic']+'：'+safe(r['notes']))
(ROOT/'BRONZE_COVERAGE_MATRIX.md').write_text('\n'.join(out)+'\n',encoding='utf-8',newline='\n')
print('PASS: coverage bound to current code, actual page numbers and visual QA')
