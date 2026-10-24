#!/usr/bin/env python3
"""Reproduce the legacy audit without editing any of the 32 root sources.

Compile-only is deliberately separate from behavioral evidence. Temporary
drivers and JSON diagnostics stay in _audit/. No known-broken legacy algorithm
is made into a production replacement by this script.
"""
from __future__ import annotations
import concurrent.futures
import hashlib
import itertools
import json
import math
import os
from pathlib import Path
import random
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "_audit"
OUT.mkdir(exist_ok=True)
CXX = os.environ.get("CXX", "g++")
FLAGS = ["-std=gnu++20", "-O2", "-Wall", "-Wextra"]
RNG = random.Random(20261024)

def read_source(p):
    return p.read_bytes().decode("utf-8", errors="replace")

def invoke(args, data="", timeout=15):
    try:
        p = subprocess.run(args, input=data.encode(), capture_output=True, timeout=timeout)
        return {"returncode": p.returncode, "stdout": p.stdout.decode("utf-8", "replace"),
                "stderr": p.stderr.decode("utf-8", "replace")}
    except subprocess.TimeoutExpired:
        return {"returncode": None, "timeout": True, "stdout": "", "stderr": "timeout"}

def include_driver(name, body, rename=False):
    prefix = "#define main legacy_main\n" if rename else ""
    suffix = "#undef main\n" if rename else ""
    return prefix + '#include "../' + name + '"\n' + suffix + body

NO_MAIN = {
    "DSU.cpp": "int main(){init(8); unite(1,2); unite(2,3); unite(1,3); if(!same(1,3)||same(1,4)) return 1; init(8); if(same(1,3)) return 2; std::cout<<\"PASS\\n\";}\n",
    "read.cpp": "int main(){std::cout<<read()<<'\\n';}\n",
    "interaction.cpp": "int main(){}\n",
    "Segment tree2.cpp": "int main(){}\n",
}

def compile_one(pair):
    idx, p = pair
    src = p
    mode = "original-main"
    if p.name in NO_MAIN:
        src = OUT / (f"original_{idx:02}.cpp")
        src.write_text(include_driver(p.name, NO_MAIN[p.name]), encoding="utf-8")
        mode = "unchanged-source-with-driver"
    exe = OUT / (f"original_{idx:02}" + (".exe" if os.name == "nt" else ""))
    result = invoke([CXX, *FLAGS, str(src), "-o", str(exe)], timeout=60)
    return {"file": p.name, "sha256": hashlib.sha256(p.read_bytes()).hexdigest(),
            "bytes": p.stat().st_size, "compile_mode": mode,
            "compile": result, "binary": str(exe), "checks": []}

files = sorted(ROOT.glob("*.cpp"), key=lambda p:p.name.casefold())
if len(files) != 32:
    raise SystemExit(f"Expected 32 legacy sources, got {len(files)}")
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
    rows = list(pool.map(compile_one, enumerate(files)))
by_name = {r["file"]:r for r in rows}
expected_compile_failures = {"binary_lifting.cpp", "Fenwick_tree.cpp", "mex.cpp", "Segment tree2.cpp"}
actual_compile_failures = {r["file"] for r in rows if r["compile"]["returncode"] != 0}
if actual_compile_failures != expected_compile_failures:
    raise AssertionError(("legacy compile results changed", actual_compile_failures))

def run(name, data="", timeout=3):
    row = by_name[name]
    if row["compile"]["returncode"] != 0:
        raise AssertionError(f"Cannot run {name}: original did not compile")
    return invoke([row["binary"]], data, timeout)

def check(name, label, data, expected):
    result = run(name, data)
    actual = result["stdout"].split()
    passed = result["returncode"] == 0 and actual == str(expected).split()
    if not passed:
        raise AssertionError((name,label,data,expected,result))
    by_name[name]["checks"].append({"label":label,"result":"PASS"})

def counterexample(name,label,data,expected,timeout=3):
    result = run(name,data,timeout)
    by_name[name]["checks"].append({"label":label,"input":data,"expected":expected,"observed":result,"result":"KNOWN_FAILURE_REPRODUCED"})
    assert result.get("timeout") or result["returncode"] != 0 or result["stdout"].split() != str(expected).split()

def extra_driver(name, tag, body, rename=True, source_override=None):
    src = OUT / (tag + ".cpp")
    if source_override is None:
        text = include_driver(name,body,rename)
    else:
        text = source_override + "\n" + body
    src.write_text(text,encoding="utf-8")
    exe = OUT / (tag + (".exe" if os.name=="nt" else ""))
    compiled = invoke([CXX,*FLAGS,str(src),"-o",str(exe)],timeout=60)
    if compiled["returncode"] != 0:
        raise AssertionError((tag,compiled))
    return str(exe), compiled

# Independent small-input oracles, with fixed seed and explicit input domains.
for case in range(16):
    n=RNG.randint(1,8); capacity=RNG.randint(0,15)
    items=[(RNG.randint(1,8),RNG.randint(0,20)) for _ in range(n)]
    expected=max(sum(items[i][1] for i in range(n) if mask>>i&1)
                 for mask in range(1<<n)
                 if sum(items[i][0] for i in range(n) if mask>>i&1)<=capacity)
    data=f"{n} {capacity}\n"+"\n".join(f"{w} {v}" for w,v in items)+"\n"
    check("01bag.cpp",f"exhaustive-subsets-{case}",data,expected)
    dp=[0]*(capacity+1)
    for c in range(1,capacity+1):
        dp[c]=max([dp[c-1]]+[dp[c-w]+v for w,v in items if w<=c])
    check("完全背包dp.cpp",f"capacity-oracle-{case}",data,dp[-1])

for case in range(16):
    n=RNG.randint(0,10); a=[RNG.randint(-5,5) for _ in range(n)]
    expected=0
    for mask in range(1<<n):
        s=[a[i] for i in range(n) if mask>>i&1]
        if all(s[i]<s[i+1] for i in range(len(s)-1)):expected=max(expected,len(s))
    check("线性dp.cpp",f"exhaustive-subsequence-{case}",f"{n}\n"+" ".join(map(str,a))+"\n",expected)

from functools import lru_cache
for case in range(12):
    n=RNG.randint(1,7); a=[RNG.randint(0,15) for _ in range(n)]
    @lru_cache(None)
    def merge_cost(t):
        if len(t)==1:return 0
        return min(t[i]+t[i+1]+merge_cost(t[:i]+(t[i]+t[i+1],)+t[i+2:]) for i in range(len(t)-1))
    check("区间dp.cpp",f"all-merge-orders-{case}",f"{n}\n"+" ".join(map(str,a))+"\n",merge_cost(tuple(a)))

for a,b in [(0,0),(0,42),(42,0),(54,24),(10**15,123456789)]:
    check("gcd.cpp",f"nonnegative-{a}-{b}",f"{a} {b}\n",math.gcd(a,b))

for case in range(15):
    a=sorted(RNG.sample(range(-100,101),RNG.randint(1,25)))
    q=[RNG.randint(-110,110) for _ in range(10)]
    expected=[a.index(x)+1 if x in a else -1 for x in q]
    check("binary_search_num.cpp",f"sorted-unique-{case}",f"{len(a)} {len(q)}\n"+" ".join(map(str,a))+"\n"+" ".join(map(str,q))," ".join(map(str,expected)))
dup=run("binary_search_num.cpp","5 1\n1 2 2 2 3\n2\n")
assert int(dup["stdout"]) in (2,3,4)
by_name["binary_search_num.cpp"]["checks"].append({"label":"duplicates-any-occurrence","result":"PASS"})

for case in range(20):
    s="".join(RNG.choice("ab") for _ in range(RNG.randint(1,35)))
    p="".join(RNG.choice("ab") for _ in range(RNG.randint(1,8)))
    expected=[i for i in range(len(s)-len(p)+1) if s[i:i+len(p)]==p]
    check("KMP.cpp",f"naive-substring-{case}",f"{s} {p}\n"," ".join(map(str,expected)))
check("KMP.cpp","overlap","aaaaa aa\n","0 1 2 3")

for case in range(12):
    n=RNG.randint(1,25); a=[RNG.randint(-100,100) for _ in range(n)]; now=a[:]
    ops=[]; answers=[]
    for _ in range(70):
        kind=RNG.randint(1,3)
        if kind<3:
            i=RNG.randrange(n);v=RNG.randint(-100,100);ops.append(f"{kind} {i+1} {v}")
            if kind==1:now[i]=v
            else:now[i]+=v
        else:
            l=RNG.randrange(n);r=RNG.randrange(l,n);ops.append(f"3 {l+1} {r+1}");answers.append(sum(now[l:r+1]))
    check("segmen tree1.cpp",f"vector-oracle-{case}",f"{n} {len(ops)}\n"+" ".join(map(str,a))+"\n"+"\n".join(ops)," ".join(map(str,answers)))

for case in range(12):
    n=RNG.randint(1,25); a=[RNG.randint(0,10**6) for _ in range(n)]
    qs=[(l,r) for l in range(n) for r in range(l,n)]
    expected=[]
    for l,r in qs:
        g=0
        for i in range(l+1,r+1):g=math.gcd(g,abs(a[i]-a[i-1]))
        expected.append(g)
    check("ST表.cpp",f"all-intervals-{case}",f"1\n{n} {len(qs)}\n"+" ".join(map(str,a))+"\n"+"\n".join(f"{l+1} {r+1}" for l,r in qs)," ".join(map(str,expected)))

# Exhaustive edge subsets, independent of both MST implementations.
for case in range(14):
    n=RNG.randint(1,6)
    edges=[(u,v,RNG.randint(-10,25)) for u in range(n) for v in range(u+1,n) if RNG.randrange(3)]
    best=None
    for subset in itertools.combinations(edges,max(0,n-1)):
        f=list(range(n))
        def root(x):
            while f[x]!=x:x=f[x]
            return x
        good=True
        for u,v,w in subset:
            x,y=root(u),root(v)
            if x==y:good=False;break
            f[x]=y
        if good and len({root(i) for i in range(n)})==1:
            total=sum(w for u,v,w in subset);best=total if best is None else min(best,total)
    data="\n".join(f"{u+1} {v+1} {w}" for u,v,w in edges)+"\n"
    check("prim MST.cpp",f"edge-subsets-{case}",f"{n} {len(edges)}\n"+data,best if best is not None else "No MST.")
    check("kruskal MST.cpp",f"edge-subsets-{case}",f"{n} {len(edges)} 1\n"+data,best if best is not None else "No Answer")
check("kruskal MST.cpp","target-two-components","4 4 2\n1 2 7\n2 3 1\n3 4 3\n1 4 20\n",4)
check("DSU.cpp","union-idempotence-and-reinit","","PASS")

for name,func in [("bfs.cpp","bfs"),("dfs.cpp","dfs")]:
    exe,compiled=extra_driver(name,"visit_"+func,
        f"int main(){{g[1]={{2}};g[2]={{1,3}};g[3]={{2}};g[4]={{5}};g[5]={{4}};{func}(1);for(int i=1;i<=5;i++)if(visited[i]!=(i<=3))return 1;std::cout<<\"PASS\\n\";}}\n")
    result=invoke([exe]);assert result["returncode"]==0 and result["stdout"].strip()=="PASS"
    by_name[name]["checks"].append({"label":"cycle-free-component-and-disconnected-vertices","result":"PASS","driver_compile":compiled})

check("read.cpp","signed-number"," -12345\n",-12345)
counterexample("read.cpp","EOF-never-terminates","","EOF handled",timeout=0.3)
counterexample("ksm.cpp","zero-exponent-mod-one","2 0 1\n","2^0 mod 1=0")
counterexample("matrix_ksm.cpp","zero-accumulator","4 1 1 1 1\n",1)
counterexample("matrix_ksm.cpp","n-below-three-negative-exponent","2 3 5 7 11\n",5,timeout=0.3)

# Only add a declaration in a temporary copy to expose the independent lazy bugs.
seg=read_source(ROOT/"Segment tree2.cpp")
seg=seg.replace("void pushdown(int node, int l, int r)","void apply(int,int,int,ll);\nvoid pushdown(int node, int l, int r)")
body='''int main(){
 a[1]=1;a[2]=2;build(1,1,2);modify(1,1,2,1,2,10);std::cout<<query(1,1,2,1,1)<<'\\n';
 update(1,1,2,1,7);std::cout<<query(1,1,2,1,2)<<'\\n';
 std::fill(lazy,lazy+N*4,0);a[1]=a[2]=a[3]=1000000000;build(1,1,3);std::cout<<query(1,1,3,1,3)<<'\\n';
}'''
exe,compiled=extra_driver("Segment tree2.cpp","segment_declaration_only",body,source_override=seg)
result=invoke([exe]);assert result["stdout"].split()!=["11","19","3000000000"]
by_name["Segment tree2.cpp"]["checks"].append({"label":"declaration-only-copy-exposes-query-point-set-overflow","expected":"11 19 3000000000","observed":result,"result":"KNOWN_FAILURE_REPRODUCED","driver_compile":compiled})

exe,compiled=extra_driver("Mobius_inversion.cpp","mobius_helper",'int main(){auto v=get_mu(10);std::cout<<v[4]<<" "<<v[6]<<"\\n";}\n')
result=invoke([exe]);assert result["stdout"].split()!=["0","1"]
by_name["Mobius_inversion.cpp"]["checks"].append({"label":"unused-helper-overwrites-composite-mu","expected":"0 1","observed":result,"result":"KNOWN_FAILURE_REPRODUCED"})
for case in range(10):
    a=RNG.randint(1,10);b=RNG.randint(a,20);c=RNG.randint(1,10);d=RNG.randint(c,20);k=RNG.randint(1,8)
    expected=sum(math.gcd(x,y)==k for x in range(a,b+1) for y in range(c,d+1))
    check("Mobius_inversion.cpp",f"active-main-gcd-enumeration-{case}",f"1\n{a} {b} {c} {d} {k}\n",expected)

# Store exact source integrity and diagnostic details; audited statuses live in
# notes/legacy_manifest.json so a rerun cannot accidentally upgrade a status.
report={"seed":20261024,"compiler":invoke([CXX,"--version"])["stdout"].splitlines()[0],
        "flags":FLAGS,"root_cpp_count":len(rows),"compile_pass":sum(r["compile"]["returncode"]==0 for r in rows),
        "compile_fail":sum(r["compile"]["returncode"]!=0 for r in rows),"files":rows}
(OUT/"legacy_results.json").write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding="utf-8")
print(json.dumps({k:v for k,v in report.items() if k!="files"},ensure_ascii=False))
for r in rows:
    print(r["file"],"COMPILE_PASS" if r["compile"]["returncode"]==0 else "COMPILE_FAIL",len(r["checks"]),"checks")
print("Behavioral checks and expected counterexamples reproduced; this is not a claim that every legacy file is correct.")
