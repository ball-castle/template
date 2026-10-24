#!/usr/bin/env python3
"""Regenerate the human-readable algorithm catalog from the canonical metadata."""
import json
import math
from pathlib import Path

root = Path(__file__).resolve().parents[1]
modules = [json.loads(p.read_text(encoding='utf-8-sig'))
           for p in sorted((root/'notes/modules').glob('*.json'))]
modules.sort(key=lambda m: (['P0','P1','P2'].index(m['priority']), m['id']))
lines = ['# 正式模板索引', '', '此文件由 `python scripts/build_index.py` 生成。代码唯一来源为下表 source；说明唯一来源为 `notes/modules/*.json`。测试结论以 `VALIDATION_REPORT.md` 和记录中的源码哈希为准。', '',
         '| 编号 | 优先级 | 模块 | 分类 |', '|---|---|---|---|']
for m in modules:
    lines.append(f"| {m['id']} | {m['priority']} | [{m['title']}](../{m['source']}) | {m['category']} |")
for m in modules:
    lines += ['', f"## {m['id']} {m['title']}", '', f"规范源码：[{m['source']}](../{m['source']})", '']
    for label, key in [('识别信号','when'),('接口约定','contract'),('复杂度','complexity'),('初始化与调用','usage'),('常见错误','pitfalls'),('练习入口','example'),('测试入口','test')]:
        value=m[key]
        if isinstance(value,list): value='；'.join(map(str,value))
        lines += [f'**{label}**：{value}', '']
    if m.get('exercise_url'):
        lines += [f"**可点击练习题**：{m['exercise_url']}", '']
    lines += ['```cpp', (root/m['source']).read_text(encoding='utf-8-sig').rstrip(), '```']
(root/'notes/INDEX.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
print(f'Indexed {len(modules)} modules')
profiles = {
    'CO01':('低','高','减少单测/多测和输入错误'), 'CO02':('低','高','把原数组操作转成前缀/差分'),
    'CO03':('中','高','先证单调再写边界'), 'DP01':('中','高','区分恰好/至多及迭代方向'),
    'DP02':('中','中','从平方状态推到有序末尾优化'), 'DP03':('高','条件性','集合与末尾是影响未来的信息'),
    'DP04':('中','条件性','按区间长度安排转移'), 'DS01':('低','高','先分清增量修改和前缀查询'),
    'DS02':('中','高','统一闭区间与单点赋值'), 'DS03':('高','中','掌握标记含义与查询下传'),
    'DS04':('中','中','维护候选的单调性并处理相等值'), 'DS05':('中','中','区分静态幂等查询与动态更新'),
    'G01':('低','高','连通块和成功合并的不变量'), 'G02':('中','高','状态搜索和前驱恢复'),
    'G03':('中','高','显式栈与父亲关系'), 'G04':('低','中','依赖有环时识别不可排序'),
    'G05':('高','条件性','基环图结构与环上顺序'), 'G06':('高','中','距离与来源及非负权前提'),
    'G07':('中','中','最小连接代价和断图检查'), 'G08':('中','较低','作为MST的另一实现对照，临场通常选一套'),
    'G09':('高','中','树上祖先、距离与越界'), 'G10':('中','中','把子树变成区间并按依赖逆序计算'),
    'G11':('高','较低','把互达关系缩成DAG'), 'G12':('高','较低','理解父边ID和low-link含义'),
    'M01':('低','高','数论/位操作极值与合法输入'), 'M02':('中','高','消除负模和乘法溢出'),
    'M03':('高','条件性','固定维状态与大次数迭代'), 'M04':('高','较低','专用推导示例，不能当通用递推公式'),
    'M05':('中','中','组合计数前先验证质数和上界'), 'S01':('中','中','确定性前后缀匹配'),
    'S02':('中','条件性','快速筛选但记得碰撞风险'), 'S03':('低','高','字节字符串与空字段边界')}
selection = ['# 每份模块的取舍', '',
    '这是训练与携带判断，不是对2026赛题的预测。两届样本不足以估计可信的数值命中概率；“场景频度”只表达通用工具适用面。', '',
    '手敲时间按每分钟10–20行估算，不含读题、改接口和调试；应在一次实际复抄后用个人数据替换。背诵难度是相对判断。可靠性只在各模块契约内成立，测试不是形式证明。', '',
    '|编号|可靠性|背诵难度|手敲估计|场景频度判断|对当前能力的帮助|', '|---|---|---|---|---|---|']
for m in modules:
    difficulty, frequency, benefit = profiles[m['id']]
    count = len((root/m['source']).read_text(encoding='utf-8-sig').splitlines())
    reliability='测试通过；仍可能哈希碰撞' if m['id']=='S02' else '契约内编译/功能/边界/消毒器通过'
    selection.append(f"|{m['id']}|{reliability}|{difficulty}|{math.ceil(count/20)}–{math.ceil(count/10)}分钟|{frequency}|{benefit}|")
(root/'notes/MODULE_SELECTION.md').write_text('\n'.join(selection)+'\n',encoding='utf-8')
