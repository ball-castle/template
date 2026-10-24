"""Regenerate the cookbook from tested canonical sources and model cards."""
from pathlib import Path
import json
import re

root = Path(__file__).resolve().parents[2]
load = lambda path: json.loads((root / path).read_text(encoding="utf-8"))
cards = load("notes/round02/cards/segment.json")
modules = {
    path.stem: json.loads(path.read_text(encoding="utf-8"))
    for path in (root / "notes/round02/modules").glob("PSD*.json")
}
manifest = load("notes/round02/segment_sources.json")
read = lambda path: (root / path).read_text(encoding="utf-8").rstrip()
lines = []
def add(text=""):
    lines.append(text)
def code(text, language="cpp"):
    fence = chr(96) * 3
    add(fence + language + "\n" + text.rstrip() + "\n" + fence)
def expand(path):
    path = Path(path)
    text = path.read_text(encoding="utf-8")
    out = []
    for line in text.splitlines():
        if line == "#pragma once":
            continue
        match = re.fullmatch(r'#include "([^"]+)"', line)
        if match:
            out.append(expand(path.parent / match.group(1)))
        else:
            out.append(line)
    return "\n".join(out).rstrip()
def link(path):
    return f"[{path}]({path.replace(' ', '%20')})"

add("# 线段树与 Fenwick 个人比赛 Cookbook")
add()
add("> 规范数据：notes/round02/cards/segment.json、notes/round02/modules/PSD*.json。"
    "本文件由 competition/data_structure/build_cookbook.py 从卡片与规范源码生成；"
    "修改代码后重新运行生成器，不手工维护另一份树。")
add()
add("正式代码优先沿用用户的全局 a/tree/lazy、1 下标和"
    "pushup/pushdown/build/modify/query。原文件与 ROUND01 的 DS01/DS02/DS03 "
    "均保留；新旧版本在相同随机操作、独立数组 oracle 下比较。"
    "用户已经使用验证的经验予以保留；没有核验的线上提交状态记为 UNKNOWN。")
add()
add("## 一页速查：Fenwick、ST 表、单调队列、线段树")
add()
add("| 题目限制 | 第一选择 | 必须检查 |")
add("|---|---|---|")
for row in [
    ("静态区间和", "前缀和", "区间由两个前缀相减"),
    ("只在最后看结果的区间加", "差分", "中间没有在线询问"),
    ("单点加、前缀/区间和", "PSD01 Fenwick", "操作可消去；不能减两个前缀min"),
    ("静态min/max/gcd", "ROUND01 DS05 ST表", "重叠查询需幂等；sum不行"),
    ("固定窗口min/max", "ROUND01 DS04 单调队列", "窗口移动规则固定，非任意区间"),
    ("动态任意区间或多个统计量", "PSD02单点树+Node变体", "merge结合律；有些merge不交换"),
    ("区间加、和/min/max", "PSD10", "sum加len*x；min/max各加x"),
    ("区间赋值或赋值+加", "PSD11", "has_set区分赋值0；先set后add下传"),
]:
    add("| " + " | ".join(row) + " |")
add()
add("| 要维护的答案 | 最小 Node / 查询信号 |")
add("|---|---|")
for row in [
    ("和 / min / max / gcd", "sum / mn / mx / g；四种都只需一个统计量"),
    ("最大值和次数", "mx,cnt；mx相同才相加cnt"),
    ("最大连续子段和", "sum,pref,suff,best；跨界候选suff左+pref右"),
    ("第k个", "非负频率sum；不足k就跳过左子树"),
    ("[L,R]第一个>=x", "mx<x整段排除；先左后右"),
    ("多个统计量", "每一项都能合并；可merge不等于可lazy"),
]:
    add("| " + " | ".join(row) + " |")
add()
add("边界先写清：modify是赋值还是加；[L,R]是闭区间还是半开；"
    "n=0不建本节全局树；全负最大子段默认非空；"
    "gcd禁止LLONG_MIN；第k个必须非负频率。")
add()
add("## 为什么不同题需要不同 Node")
add()
add("Node是一个区间对未来合并所需的摘要。先问：拼接左右两段时，"
    "答案是否只由两段答案决定？和、最小值可以；最大子段和不可以，"
    "因为最优子段可能跨分界，所以补充左右前缀、后缀和。"
    "再问：题目操作能否直接作用于摘要？区间加可直接更新sum/min/max；"
    "只存gcd不能直接更新，因为[2,4]与[2,6]原gcd同为2，整体+2后分别为2与4。")
add()
add("merge只需结合律，不要求交换律。最大子段的左右方向不能颠倒；"
    "原矩阵练习以列向量表示状态，先执行左段再执行右段，"
    "组合矩阵必须是right*left。该练习额外保存翻转后的rev，"
    "使整段A/B互换可通过交换mat/rev完成。")
add()
add("## 三棵完整树骨架与一个 Fenwick")
add()
add("每个个人版本独立用于一份题目，不要把多个含同名全局数组的版本一起粘贴。"
    "工程中的.hpp已组合好Node与公共骨架，可以直接include。"
    "纸面抄写时把对应Node、make_node、merge放在单点公共骨架前；"
    "下面PSD02已给出完整组合，其他点改模型只替换这三项。"
    "测试内部的namespace只是为了同进程对照，不是要求比赛使用cp::。")
add()
for mid in ["PSD01", "PSD02", "PSD10", "PSD11"]:
    m = modules[mid]
    add("### " + mid + " " + m["title"])
    add()
    add("来源与规范源码：" + link(m["source"]) + "。")
    add()
    add("契约：" + m["contract"])
    add()
    code(expand(root / m["source"]))
    add()
    if mid == "PSD01":
        add("接口仅统一名字：旧sum(x)改名query(x)，旧query(l,r)改名rangeSum(l,r)。"
            "原main中的rangeSum现在对应真实接口；add仍是增量。")
    if mid == "PSD02":
        add("单点树不需要lazy或pushdown。query仅进入相交儿子，"
            "所以max/min/非空子段不必造一个容易写错的空节点。"
            "所有树接口使用合法非空闭区间；a是建树缓冲，修改后当前值保存在tree。")
    if mid == "PSD10":
        add("若题目只需要sum，可以删掉Node的mn/mx及对应三处赋值；"
            "本手册和测试的正式源保留三个字段，避免并行维护三份相同lazy树。")
    if mid == "PSD11":
        add("Tag表示函数T(x)=(has_set ? set : x)+add。"
            "compose(old,new)意味着先old后new，即new(old(x))："
            "new若有赋值，结果直接变成new；否则只把new.add加到old.add。"
            "因此新赋值覆盖旧增量，新的加法接在旧赋值之后；pushdown固定先set再add。")
    add()
add("## 14 个维护模型：信号、推导、代码和真实调用")
add()
used_examples = set()
used_node = set()
for card in cards:
    if card["id"] in {"SEG-00", "SEG-15"}:
        continue
    mid = card["code_refs"][0]
    m = modules[mid]
    add("### " + card["id"] + " " + card["title"] + " · " + mid)
    add()
    for title, field in [
        ("识别信号", "signal"), ("先观察", "observe"), ("暴力", "brute"),
        ("重复计算", "redundancy"), ("必要限制", "conditions"),
        ("正确性", "proof"), ("复杂度", "complexity")
    ]:
        add("**" + title + "：** " + str(card[field]))
        add()
    add("**怎样推出：** " + card["derive"][0])
    add()
    add("| 维护项 | 具体规则 |")
    add("|---|---|")
    for title, field in [
        ("Node", "node"), ("pushup", "pushup"), ("是否lazy", "has_lazy"),
        ("apply", "apply"), ("pushdown", "pushdown"), ("查询", "query")
    ]:
        add("| " + title + " | " + card[field].replace("|", "\\|") + " |")
    add()
    add("**核心代码：** " + link(m["source"]) + "。"
        "它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。")
    add()
    if mid in {"PSD03", "PSD04", "PSD05", "PSD06", "PSD07", "PSD08", "PSD09"} and mid not in used_node:
        code(read(m["source_parts"][0]))
        add()
        used_node.add(mid)
    if card["id"] in {"SEG-12", "SEG-13"}:
        source = read(m["source"])
        tail = source.split('#include "segment_point_base.inc"', 1)[1].strip()
        code(tail)
        add()
    add("**调用定位：**")
    code(card["usage"])
    add()
    example_source = card["example_source"]
    if example_source not in used_examples:
        add("**完整可运行调用：** " + link(example_source) + "。"
            "工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。")
        add()
        code(read(example_source))
        add()
        add("输入格式：" + card["input_format"])
        add()
        add("输入：")
        code(card["sample_input"], "text")
        add("期望输出：")
        code(card["sample_output"], "text")
        used_examples.add(example_source)
    else:
        add("**完整调用及样例复用：** " + link(example_source) + "，"
            "见SEG-05的完整main与输入输出。本模型取该输出的对应统计列。")
    add()
    add("**最容易出错：** " + "；".join(card["pitfalls"]))
    add()
    add("**来源/练习：** " + card["exercise"])
    add()
card = next(c for c in cards if c["id"] == "SEG-15")
add("## Fenwick 完整调用")
add()
code(read(card["example_source"]))
add()
add("输入：")
code(card["sample_input"], "text")
add("期望输出：")
code(card["sample_output"], "text")
add()
add("## 原代码继承与最小改动")
add()
add("以下路径按cf仓库根目录定位。template/Segment tree.cpp不存在，"
    "实际旧单点树文件为template/segmen tree1.cpp。")
add()
for origin in manifest["origin_audit"]:
    add("### " + origin["path"])
    add()
    for label, key in [
        ("直接继承", "idea"), ("原问题/限制", "issue"), ("最小改动", "change"),
        ("接口变化", "interface_change"), ("在线证据", "online_status")
    ]:
        add("**" + label + "：** " + origin[key])
        add()
add("旧区间加树的最小反例是[0,0]全段+1后询问[1,1]："
    "若query不pushdown会读到0，正确值为1。"
    "旧最小值树使用4e18作为空段值，若所有值大于4e18则会污染部分查询；"
    "当前公共查询不访问不相交儿子，消除了此类哨兵需求。")
add()
add("## 验证与复现")
add()
add("所有正式变体都是真实编译代码；13个完整main覆盖14种维护模型，"
    "其中区间加的sum/min/max共用一个main。"
    "验证包括固定种子20261024随机差分、n=1/n=200000、全0/全负、"
    "重复最大值、负数gcd、频率非法k、搜索范围、set0、set-add、add-set、"
    "多层标记与重建。PSD01/PSD02/PSD10与ROUND01 DS01/DS02/DS03使用同一输入流，"
    "同时与独立暴力比较；最大子段oracle枚举所有端点。")
add()
add("执行目录为template仓库根目录：")
code("\n".join(manifest["commands"][:3]), "text")
add()
add("WSL Sanitizer：")
code(manifest["commands"][3], "text")
add()
add("当前结果：" + "；".join(manifest["verification"].values()) + "。")
add()
add("个人全局树不接收空树或空区间；只有Fenwick空前缀/空区间、"
    "first_ge空搜索范围与kth非法k有显式返回值。"
    "所有sum/标记/长度乘积及中间运算需处于long long范围。"
    "数学gcd可能需要2^63时应另用无符号或更宽表示，本节ll版本不接受LLONG_MIN。")
(root / "SEGMENT_TREE_COOKBOOK.md").write_text("\n".join(lines) + "\n", encoding="utf-8")
print("WROTE SEGMENT_TREE_COOKBOOK.md from canonical sources/cards")
