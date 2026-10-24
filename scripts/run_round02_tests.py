#!/usr/bin/env python3
"""Compile printed expanded recipes and execute declared independent suites."""
import argparse
import os
import platform
import re
import subprocess
import time
from round02_common import ROOT, cards, catalogs, code_digest, digest, expanded_source
from round02_common import json_write, test_paths, read_json

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--sanitize', action='store_true')
    ap.add_argument('--patch', action='store_true')
    ap.add_argument('--ids', help='Comma-separated module IDs, with dependencies')
    ap.add_argument('--refresh', action='store_true',help='Refresh selected modules in a prior complete PASS report')
    ap.add_argument('--resume', action='store_true',help='Reuse unchanged successful independent compile checks after an interrupted complete run')
    ap.add_argument('--compiler', default=os.environ.get('CXX', 'g++'))
    args = ap.parse_args()
    old, new = catalogs()
    all_modules = old | new
    if args.ids:
        selected = set(args.ids.split(','))
    elif args.patch:
        selected = {m['id'] for m in new.values() if m.get('patch')}
    else:
        selected = set(all_modules)
    pending = list(selected)
    while pending:
        key = pending.pop()
        if key not in all_modules:
            raise RuntimeError('Unknown module '+key)
        for dep in all_modules[key].get('dependencies', []):
            if dep not in selected:
                selected.add(dep); pending.append(dep)
    modules = [all_modules[key] for key in sorted(selected)]
    mode = 'sanitizers' if args.sanitize else 'release'
    out = ROOT/'build'/f'round02_{mode}'
    out.mkdir(parents=True, exist_ok=True)
    flags = ['-std=gnu++20','-Wall','-Wextra','-Wshadow']
    flags += (['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',
               '-fno-pie','-no-pie'] if args.sanitize else ['-O2'])
    env = dict(os.environ)
    if args.sanitize:
        env['ASAN_OPTIONS'] = 'detect_leaks=1:halt_on_error=1'
        env['UBSAN_OPTIONS'] = 'halt_on_error=1:print_stacktrace=1'
    report = {'status':'FAIL','scope':'selected' if args.patch or args.ids else 'complete',
              'mode':mode,'flags':flags,'platform':platform.platform(),
              'source_hash_mode':'LF-normalized',
              'compiler':subprocess.check_output([args.compiler,'--version'],text=True).splitlines()[0],
              'modules':{},'sources':{},'test_sources':{},'test_dependencies':{},
              'card_examples':{},'module_examples':{},'checks':[]}
    if args.refresh or args.resume:
        prior=read_json(out/'results.json')
        if args.refresh and (prior.get('scope')!='complete' or not args.ids):
            raise RuntimeError('--refresh requires a complete PASS report and --ids')
        if args.resume and (prior.get('scope')!='complete' or args.ids or prior.get('flags')!=flags):
            raise RuntimeError('--resume requires the same complete configuration')
        report=prior;report['status']='FAIL'
    def run(name, cmd, input_text=None, expected=None):
        start = time.monotonic()
        process = subprocess.run(cmd,cwd=ROOT,env=env,text=True,encoding='utf-8',
                                 errors='replace',stdout=subprocess.PIPE,
                                 stderr=subprocess.STDOUT,timeout=300,input=input_text)
        (out/(name+'.log')).write_text(process.stdout,encoding='utf-8')
        result={'name':name,'command':cmd,'exit_code':process.returncode,
                'seconds':round(time.monotonic()-start,3),'output':process.stdout}
        report['checks']=[x for x in report['checks'] if x['name']!=name]
        report['checks'].append(result)
        if expected is not None:
            result['expected_output']=expected
            result['output_matches']=(process.stdout.split()==expected.split())
            if not result['output_matches']:
                result['exit_code']=-1
                raise RuntimeError(name+' output mismatch: '+repr(process.stdout))
        print(('PASS ' if not process.returncode else 'FAIL ')+name,flush=True)
        if process.returncode:
            print(process.stdout[-6000:],flush=True)
            raise RuntimeError(name)
    try:
        tests = set()
        for module in modules:
            paths = test_paths(module)
            if not paths:
                raise RuntimeError('Missing declared functional test: '+module['id'])
            for path in paths:
                if not (ROOT/path).exists():
                    raise RuntimeError('Missing test '+path)
                tests.add(path)
            lines, sources = expanded_source(module['source'])
            reuse_compile=(args.resume and
                report['modules'].get(module['id'],{}).get('expanded_code_sha256')==code_digest(lines)
                and any(c['name']=='compile_'+module['id'] and c['exit_code']==0 for c in report['checks']))
            report['sources'].update(sources)
            report['modules'][module['id']] = {'source':module['source'],
                'expanded_code_sha256':code_digest(lines),'tests':paths}
            header = out/(module['id']+'_expanded.hpp')
            header.write_text('\n'.join(lines)+'\n',encoding='utf-8',newline='\n')
            driver = out/(module['id']+'_driver.cpp')
            body = '#include "'+header.name+'"\n'
            if not re.search(r'\b(?:int|signed)\s+main\s*\(', '\n'.join(lines)):
                body += 'int main() { return 0; }\n'
            driver.write_text(body,encoding='utf-8',newline='\n')
            binary = out/(module['id']+('.exe' if os.name=='nt' else ''))
            if reuse_compile:print('REUSE unchanged compile_'+module['id'],flush=True)
            else:run('compile_'+module['id'],[args.compiler,*flags,str(driver),'-o',str(binary)])
            if module.get('example_source'):
                path=module['example_source']
                example_lines, example_hashes=expanded_source(path)
                report['sources'].update(example_hashes)
                expanded=out/(module['id']+'_example.cpp')
                expanded.write_text('\n'.join(example_lines)+'\n',encoding='utf-8',newline='\n')
                example_binary=out/(module['id']+'_example'+('.exe' if os.name=='nt' else ''))
                run('example_'+module['id']+'_compile',
                    [args.compiler,*flags,str(expanded),'-o',str(example_binary)])
                run('example_'+module['id'],[str(example_binary)],
                    module.get('sample_input',''),module.get('sample_output'))
                report['module_examples'][module['id']]={'source':path,
                    'sha256':digest(ROOT/path,text=True)}
        for path in sorted(tests):
            # The actual suite uses its original includes, independent of printed expansion.
            _, before_dependencies=expanded_source(path)
            label = path.removesuffix('.cpp').replace('/','_')
            binary = out/(label+('.exe' if os.name=='nt' else ''))
            run(label+'_compile',[args.compiler,*flags,str(ROOT/path),'-o',str(binary)])
            run(label,[str(binary)])
            report['test_sources'][path] = digest(ROOT/path,text=True)
            _, dependencies = expanded_source(path)
            if dependencies!=before_dependencies:
                raise RuntimeError('Sources changed during test compilation/run: '+path+'; re-run this suite')
            report['test_dependencies'].update(dependencies)
        for card in cards():
            code = card.get('usage', '')
            if not isinstance(code,str) or not re.search(r'\bint\s+main\s*\(',code):
                continue
            if not set(card.get('code_refs',[])).issubset(selected):
                continue
            example = out/('example_'+card['id']+'.cpp')
            example.write_text(code,encoding='utf-8',newline='\n')
            binary = out/('example_'+card['id']+('.exe' if os.name=='nt' else ''))
            run('example_'+card['id']+'_compile',
                [args.compiler,*flags,'-I',str(ROOT),str(example),'-o',str(binary)])
            run('example_'+card['id'],[str(binary)],card.get('sample_input',''),
                card.get('expected_output'))
            report['card_examples'][card['id']] = code_digest(code.splitlines())
        if any(x['exit_code']!=0 for x in report['checks']):
            raise RuntimeError('Unresolved failed checks remain in the report')
        report['status']='PASS'
    finally:
        json_write(out/'results.json',report)
    print(f'{mode}: {len(modules)} modules; {len(tests)} suites; PASS',flush=True)

if __name__=='__main__':
    main()
