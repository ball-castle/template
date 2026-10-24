# 验证报告：成都铜牌模板 2026-10-24

本报告区分正式模块、历史代码和打印成品。通过表示在写明的输入契约及已执行测试下通过，不表示在线评测 AC、任意输入正确或获奖保证。

## 正式模块验收

共 **32 个唯一源码模块：17 P0、13 P1、2 P2**。完整说明和调用方式见 [索引](notes/INDEX.md)，个人使用成本见 [模块取舍](notes/MODULE_SELECTION.md)。P2 仅选 SCC 与桥。

| 配置 | 实际环境 | 结果 |
|---|---|---|
| Release | Windows 11 / MinGW GNU g++ 15.2.0；`-std=gnu++20 -O2 -Wall -Wextra -Wshadow -g` | 32 个独立模块编译、1 次主程序 smoke、3 个功能套件编译及运行，共 39 项检查通过 |
| Sanitizers | WSL Ubuntu / GNU g++ 13.3.0；GNU++20，`-O1 -g -Wall -Wextra -Wshadow -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie` | 同样 39 项通过；未出现 ASan、UBSan 或泄漏诊断 |

两套最终报告均无编译警告。测试未定义 `NDEBUG`，断言实际执行。固定随机种子为 `20261024`。每个头文件以只包含该头文件的合法独立驱动编译；功能套件实际调用接口，不能把空 main 编译当功能正确证据。

可复现命令（在 template 根目录）：

```sh
python3 scripts/run_tests.py
python3 scripts/run_tests.py --sanitize
```

Windows 可跑第一条，第二条使用支持消毒器的 Linux/WSL。`--p0` 可只选择急用模块。当前完整机器记录保存于 [release.json](notes/validation/release.json) 和 [sanitizers.json](notes/validation/sanitizers.json)，包含平台、编译器、命令、退出码、输出、正式源码与测试源码 SHA256。日志中的本机绝对路径仅作为本次证据；复现脚本自动使用新克隆路径。

## 功能、边界与独立对照

| 测试入口 | 独立对照及关键边界 |
|---|---|
| [test_core_data.cpp](tests/test_core_data.cpp) | 暴力数组、枚举所有子区间对照；前缀/差分、离散化、全 ll 范围二分、双指针、非负滑窗；Fenwick、普通/Lazy 线段树、单调栈/队列、ST；空区间、零元素、负值、30 亿增量、多对象重建；20 万元素与 20 万次后缀更新 |
| [test_graph.cpp](tests/test_graph.cpp) | BFS/Floyd 对 DSU、遍历与 Dijkstra；枚举边子集对 MST；逐级父亲对 LCA；朴素集合对子树；双向可达对 SCC；逐条删边对桥/基环成员；单点、断图、零权、负权适用范围、重复边、自环、空源、路径恢复、无效结构拒绝；20 万深链/大环 |
| [test_dp_math_string.cpp](tests/test_dp_math_string.cpp) | 600 组背包数量枚举、350 组 LIS 子序列枚举、250 组 Hamilton 全排列、180 组相邻合并穷举、1500 组模幂朴素连乘、200 组矩阵重复乘、200 组乘积递推、Pascal 三角、1200 组字符串朴素匹配/LCP；LLONG_MIN/MAX、模 1、零指数/零底数、空串、重叠匹配 |

深树相关正式实现均采用迭代遍历；Lazy 线段树递归深度为对数级。规模测试验证的是列出的规模，不承诺任何题目的时间/内存限制。Hamilton 的契约为 n<=18，测试包含小规模穷举；不存在用大规模随机对照替代穷举正确性判断的说法。

重点限制：Dijkstra 要求非负权且真实有限距离小于 INF；组合数要求质数模数及预处理上界小于模数；普通矩阵不是 max-plus；哈希存在碰撞；子树和、区间和及累计标记仍必须落在 ll 范围。各模块完整前提见 JSON/索引，调用者不能省略这些条件。

## 历史审计与修复反例

原提交 `8c9f89adb51980acd6f16b8da3e2e9c344130423` 的 **32 份根目录 cpp 全部逐份审读并原样保留**。28 份编译成功、4 份失败；175 条行为记录包含预期失败。旧状态为 14 VERIFIED、10 REFERENCE_ONLY、7 INCOMPLETE、1 UNVERIFIED，不把旧代码批量标成 READY。

正式替代中的五项 FIXED 分别处理 Fenwick 调用名、Lazy 声明/下传/溢出、快速幂模运算、乘积递推初值与边界、残缺 ST 草稿。旧文件状态维持不变。其他已复现风险包括 KMP 空模式、区间 DP 的过小 INF、旧 Möbius 备用函数等，具体范围和合法输入/扩展输入的区别见 [完整审计](TEMPLATE_AUDIT.md)。

- [核心与数据结构最小反例](tests/regressions_core_data.md)
- [图论与深树回归](tests/regressions_graph.md)
- [DP/数学/字符串反例](tests/regressions_dp_math_string.md)
- [32 份旧源码哈希、编译诊断与行为证据](notes/legacy_manifest.json)
- 复现：`python3 tests/legacy_audit.py`；临时输出到忽略的 `_audit/`。

## 打印成品与源码对应

| 成品 | 实际页数 | 模块 | 逐行一致的源码行数 |
|---|---:|---:|---:|
| 成都站_铜牌冲刺板子_2026.docx / .pdf | 24 | 17 P0 | 658 |
| ACM_ICPC_铜牌算法板子_完整版.docx / .pdf | 41 | 32 | 1271 |

DOCX 为可编辑 OOXML，PDF 由本机 Microsoft Word 实际导出。环境没有 LibreOffice，标准 render_docx 入口因此不可用；采用 Word 导出后 Poppler 130 DPI 全页渲染，未把缺失工具当作渲染通过。A4、13 mm 页边距、9.25 pt Consolas 代码、中文字体和黑白标题；含算法编号、索引、页眉、页码和前三页思维/排错材料。

生成器只读取 `notes/modules/*.json` 的唯一正式源码，检查两套测试 PASS 以及源码和测试 SHA256。两份 `.manifest.json` 保存模块与起始页；`check_print.py` 核验 A4、页数、非空页、模块位置、源文件哈希以及 DOCX 代码逐行一致。导出同时记录 DOCX/PDF 配对哈希，避免重新生成 Word 后误用旧 PDF。

`tests/test_print_gate.py` 已复现“旧 PDF 与新 Word 页数相同却误通过”的门禁缺陷；加入配对后该反例被拒绝，回归测试通过。结构检查不能代替视觉检查。**全部 24+41 页已实际渲染并逐页查看**；末次排版修改后的 12 张变化页重新查看，其余 53 张与已检查版本像素一致。未发现缺字、裁切、乱码、空白页或错误页码。少数续页留白较多，以便保持函数和逻辑块完整。详见 [打印 QA](notes/validation/PRINT_QA.md)。

```sh
python3 tests/test_print_gate.py
python3 scripts/check_print.py
```

PDF 与 Word 哈希、结构检查结果保存在 [急用检查](notes/validation/emergency_print.json) 和 [完整检查](notes/validation/full_print.json)。生成环境变化可能改变分页，应重新导出、运行门禁并逐页检查。

## 未解决、未实现和未验证

- 本次正式模块没有遗留的已知测试失败。原根目录的已知失败/残缺文件作为历史证据保留，不进入打印版。
- P2 其余候选（割点、树链剖分、DSU on Tree、Dinic、二分图匹配、计算几何、Trie 等）未实施；max-plus 与换根 DP 也未实现，不用占位代码充数。
- 本届纸质资料数量、页数、字号与装订规格没有取得完整官方可确认条文，标记 **待赛站确认**。不套用 World Finals 25 页规则。
- QOJ CruiseY 公开查询遇到登录页，无法判定提交状态；不据此声称 AC。Codeforces 数据及仓库文件证据分开记录，未逐份审计 cf 全部 215 份训练代码。
- 新模板未向线上评测系统提交，本地测试通过不等同于线上 AC。旧两份外部 Word 手册未作为未经核验的来源。
- 远端发布和新环境递归克隆的具体提交、PR 及结果记录在 cf 仓库的交付记录中。GitHub Actions 的状态以远端实际运行结果为准。
