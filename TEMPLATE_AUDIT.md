# 旧模板逐份审计

审计日期：2026-10-24。旧源基线：`8c9f89adb51980acd6f16b8da3e2e9c344130423`。

## 审计范围与结论

根目录 **32份旧 `.cpp` 全部逐行阅读并保留原位**，没有迁移、改名、覆盖或删除。用 Windows MinGW GCC 15.2.0、`-std=gnu++20 -O2 -Wall -Wextra` 逐份编译，**28份成功、4份失败**；4个无main片段通过合理的附加驱动编译，而不是用“没有main”掩盖源码错误。`DSU.cpp`、`read.cpp`驱动调用了接口，`interaction.cpp`只附空main且明确无算法，`Segment tree2.cpp`保留原样编译仍失败。

完成 **175条行为检查记录**，包括独立穷举/朴素对照、边界样例和预期失败反例；编译成功不是正确性证明。旧源分类为 **14 VERIFIED、10 REFERENCE_ONLY、7 INCOMPLETE、1 UNVERIFIED**。没有字节重复文件，本次不滥用DUPLICATE。正式替代中另有5项FIXED，修复发生在新模块，**旧文件状态不因此升级**。

可复现入口：在template根目录执行 `python tests/legacy_audit.py`；使用系统GNU g++，或设置环境变量CXX为编译器路径。结果、临时驱动与可执行文件输出到已忽略的 `_audit/`，不污染正式模块。固定随机种子为20261024。审计JSON保存在 [legacy_manifest.json](notes/legacy_manifest.json)，其中保留完整编译诊断和反例输出。

## 状态的含义

- **VERIFIED**：完整阅读，并有本文指定输入契约下的本地行为证据；不表示任意输入正确、在线评测AC或已做完整形式证明。
- **FIXED**：明确缺陷已由正式替代模块处理，并有针对反例和独立测试；须同时保留旧源状态和修复范围。
- **REFERENCE_ONLY**：只供理解历史/语法，或已知有错；禁止直接作为比赛可用模板。
- **INCOMPLETE**：缺算法、接口、输入输出或未写完；能编译的空壳仍属于此类。
- **DUPLICATE**：确认重复且能指明权威版本；本次32份无此分类。
- **UNVERIFIED**：尚不足以核实题源、前提或正确性，不能因编译成功升级。

## 32份逐项结果

下表“通过”仅指编译；输入域与行为证据才决定状态。所有源链接固定在审计基线提交。

| 文件 | 原状态 | GNU++20编译 | 契约、证据与限制 |
|---|---|---|---|
| [01bag.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/01bag.cpp) | VERIFIED | 通过 | 01背包，容量与物品数均<1010，重量正数，int价值和不溢出。 16组子集穷举对照；包含零容量、物品放不下。 |
| [bfs.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/bfs.cpp) | VERIFIED | 通过 | 只遍历从1可达的分量；1<=n<100010。不是最短路输出程序。 驱动检查可达标记与未连通点；全图需外层遍历，多测需清空。 |
| [binary_lifting.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/binary_lifting.cpp) | INCOMPLETE | 失败 | 未完成的ST表草稿，不是可用倍增/LCA实现。 使用了未定义的log，原数组定义为logn；for条件/递增和表构造均未完成，编译失败。 |
| [binary_search_num.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/binary_search_num.cpp) | VERIFIED | 通过 | 升序数组、n<1000010；返回任意相等位置或-1，不保证第一个。 15组独立数组查询对照及重复值位置检查。 |
| [data_sturcture.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/data_sturcture.cpp) | REFERENCE_ONLY | 通过 | STL语法演示，不是通用算法模块。 编译成功但有未使用变量警告；unordered容器复杂度只能说平均O(1)。 |
| [dfs.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/dfs.cpp) | VERIFIED | 通过 | 递归访问单个可达分量；小图、递归深度安全、1<=n<100010。 驱动验证标记；长链可能栈溢出，不作大树可用性承诺；中文注释原编码显示异常。 |
| [DSU.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/DSU.cpp) | VERIFIED | 通过 | 1..n，n<100005，调用init后按秩合并与路径压缩。 合理main驱动检查传递连接、重复合并、重新初始化。注释“挂到秩小”与实现相反。 |
| [Fenwick_tree.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Fenwick_tree.cpp) | INCOMPLETE | 失败 | 当前文件不能编译。有效下标应为1..n；add(0)会死循环。 定义query却调用rangeSum，编译器已证实；原文件保持原状。 |
| [gcd.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/gcd.cpp) | VERIFIED | 通过 | 非负long long输入，支持gcd(0,0)=0。 5个边界与Python math.gcd对照；负数未规范化，不保证返回非负值。 |
| [graph_paint.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/graph_paint.cpp) | INCOMPLETE | 通过 | 二分染色草稿。 没有读边、没有检测同色冲突、没有遍历所有分量；编译成功不能当判二分图。 |
| [interaction.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/interaction.cpp) | INCOMPLETE | 通过 | 只有include和宏。 加空main能编译，但没有询问、flush、读回复或协议处理。 |
| [KMP.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/KMP.cpp) | VERIFIED | 通过 | 非空模式串；输出0-based全部匹配，允许重叠。 20组朴素子串对照和aaaaa/aa重叠反例；空模式接口没有契约保护。 |
| [kruskal MST.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/kruskal%20MST.cpp) | VERIFIED | 通过 | 求恰k个分量的最小生成森林；1<=k<=n<1010，m<10010，边权及总和在int范围。 14组边子集穷举对照（连通/断开/负边/n=1）及k=2；无法达目标打印No Answer。 |
| [ksm.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/ksm.cpp) | REFERENCE_ONLY | 通过 | 旧快速幂缺少完整模运算契约。 2 0 1输出1而非0；p=0非法，负指数不终止；未先规范化a，乘法可能溢出。 |
| [lambda.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/lambda.cpp) | REFERENCE_ONLY | 通过 | 捕获、排序和递归lambda语法演示。 编译成功且有未使用lambda警告；定义未调用，不能视作算法测试。 |
| [linklist.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/linklist.cpp) | REFERENCE_ONLY | 通过 | 带头结点的教学链表。 全源码审读；未做一般链表行为验证。CreateListT实际头插导致逆序；无销毁函数，中文字符串原编码异常。 |
| [lower_bound.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/lower_bound.cpp) | REFERENCE_ONLY | 通过 | 文件名/注释讲lower_bound，实际调用降序比较器的upper_bound固定查4。 要求数组降序；未检查end就解引用。不能复制作为常规升序lower_bound。 |
| [matrix_ksm.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/matrix_ksm.cpp) | REFERENCE_ONLY | 通过 | CF1182E专用指数矩阵推导，原题n>=4。 4 1 1 1 1期望1实得0；ans=0连乘。扩展到n<3时负指数不终止（原题域外，不作为该题合法反例）。 |
| [mex.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/mex.cpp) | INCOMPLETE | 失败 | 只有思路注释，没有mex实现。 uisng拼写错误导致编译失败；修正拼写也不构成算法。 |
| [Mobius_inversion.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Mobius_inversion.cpp) | REFERENCE_ONLY | 通过 | 活动main为P2522矩形gcd计数；另有错误的备用get_mu。 活动main10组gcd穷举通过；get_mu(10)返回mu[4]=-1、mu[6]=-1，正确为0、1。每轮无条件mu[x]=-1覆盖合数值。 |
| [prim MST.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/prim%20MST.cpp) | VERIFIED | 通过 | 1<=n<5050，m<=200000；边权<0x3f3f3f3f，权和int安全，单次调用。 14组边子集穷举对照含断图/负边/n=1；全局状态多测需重置；大权和会溢出。 |
| [read.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/read.cpp) | REFERENCE_ONLY | 通过 | 有输入保证且数值范围安全的教学读整数片段。 驱动读取-12345；空输入0.3秒超时。getchar返回值存char丢EOF语义；极值累积可能溢出。 |
| [segmen tree1.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/segmen%20tree1.cpp) | VERIFIED | 通过 | 非空数组n<200005、闭区间1-based；单点赋值/加，区间和long long不溢出。 12组每组70操作与独立vector对照；零长度未支持。 |
| [Segment tree2.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Segment%20tree2.cpp) | INCOMPLETE | 失败 | 当前片段不能编译，且存在独立逻辑/宽度缺陷。 apply调用在声明前。仅在临时副本补声明后：全加10查单点得1而非11；再赋值7查总和得9而非19；3个1e9求和得-1294967296。 |
| [sort.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/sort.cpp) | REFERENCE_ONLY | 通过 | 降序比较器语法示例。 编译成功；固定样例不是通用排序测试，参见比较器严格弱序约束。 |
| [ST表.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/ST%E8%A1%A8.cpp) | VERIFIED | 通过 | CF2050F差分gcd静态查询；1<=n<=200000，合法闭区间，差值不溢出。 12组数组所有区间与直接差分gcd对照；单点输出0。不是动态区间结构。 |
| [Ternary_Search.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Ternary_Search.cpp) | UNVERIFIED | 通过 | 特定几何表达式的三分求极小；未核实其原题与全域单峰性。 GNU++20编译成功。源码链接2227/B实际是括号题Party Monster，与代码不符；不能作已核验题源。 |
| [tree_dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/tree_dp.cpp) | INCOMPLETE | 通过 | 只有子树累加遍历骨架。 无输入、无状态初始化/含义、无答案输出；编译成功不等于树DP完整。 |
| [tree_storage.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/tree_storage.cpp) | REFERENCE_ONLY | 通过 | 读取n-1条无向边的邻接表骨架。 编译成功；不验证输入确为树，不含任何查询/遍历/输出，多测需清空。 |
| [区间dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/%E5%8C%BA%E9%97%B4dp.cpp) | VERIFIED | 通过 | 线性相邻石子合并，1<=n<310，非负权；前缀和/int代价低于INF。 12组小数组枚举全部相邻合并顺序独立对照；不是环形石子合并。 |
| [完全背包dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/%E5%AE%8C%E5%85%A8%E8%83%8C%E5%8C%85dp.cpp) | VERIFIED | 通过 | 正重量、非负价值，n,m<1010，int价值和安全。 16组容量转移独立对照，含零容量；重量0且价值正没有有限最优。 |
| [线性dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/%E7%BA%BF%E6%80%A7dp.cpp) | VERIFIED | 通过 | 严格上升LIS长度，0<=n<1010，O(n^2)。 16组所有子序列穷举；重复值不扩展；未恢复具体序列。 |

## 四个重点问题的独立复现

### Fenwick：定义与调用不一致

旧类定义 `query(l,r)`，main调用 `fen.rangeSum(l,r)`。原文件直接编译失败，诊断为无成员rangeSum。只修调用名尚不能解决`add(0)`无限循环的契约风险。正式替代 [data_structure/fenwick.hpp](data_structure/fenwick.hpp) 使用统一`range_sum`接口，明确1下标和空区间语义。

### Lazy线段树：四个独立问题

原文件无main，而且在`apply`声明前调用它；加正常空main仍编译失败。审计**只在临时副本中补前向声明**以继续观察行为，未修原文件。

| 操作 | 正确结果 | 旧临时副本结果 | 原因 |
|---|---:|---:|---|
| 初值[1,2]，全段+10，再查[1,1] | 11 | 1 | 部分区间查询前没有pushdown |
| 继续将第1项赋值7，再查全段 | 19 | 9 | 单点update下降前也没有pushdown |
| 重新建树，初值[10^9,10^9,10^9]，查总和 | 3000000000 | −1294967296 | tree和query用int；此结果只记录该构建的实测，带符号溢出本身属未定义行为 |

正式替代 [data_structure/lazy_segment_tree.hpp](data_structure/lazy_segment_tree.hpp) 的契约为**区间加、区间和**，使用long long，并在部分覆盖下降前下传。它没有继承旧的单点赋值接口；若题目要赋值标记，需要专门的组合规则，不能混用。

### 矩阵快速幂：乘法累积从0开始

合法输入 `4 1 1 1 1` 的递推答案为1，旧程序输出0，原因是`ll ans=0`后连续相乘。

输入 `2 3 5 7 11` 时`n-3`为负，旧程序未在0.3秒内终止；带符号右移后指数不能到0。必须区分：**CF1182E官方要求n≥4，因此n<3是复用接口风险，不能作为该题合法输入上的WA证据。** [官方题面](https://codeforces.com/problemset/problem/1182/E)

专用递推不能只改成通用矩阵幂后就宣称已修复；乘法单位元、初值返回、零底数和指数模phi的前提都要独立测试。正式M04 [product_recurrence.hpp](math/product_recurrence.hpp) 与通用矩阵幂分离，本轮全量测试包含初值、零底数、单位元和小规模直接递推对照，已确认FIXED。

### binary_lifting：实际上未完成ST表

`pre()`使用未定义的`log[i/2]`，原数组定义为`logn`；末尾`for(ll j=1;j )`没有完整循环、表构造和查询。GNU++20实编译失败。正式DS05 [sparse_table.hpp](data_structure/sparse_table.hpp)完成幂层预处理和静态区间查询，已确认FIXED。LCA属于另外的新模块；文件名不能证明旧文件已经实现LCA或祖先倍增。

## 另外发现的风险

1. **Mobius备用函数错误。** `get_mu(10)`实测`mu[4]=-1, mu[6]=-1`，正确应为0、1；每次外层迭代无条件`mu[x]=-1`覆盖已筛出的合数值。活动main走另一套`init_mu()`，10组小矩形gcd穷举通过，但这不能替错误备用接口消除风险。注释中反演方向/整除条件也应按所用公式重新推导，不抄混淆表达。
2. **快速幂模1边界。** `2 0 1`输出1，正确为0；还需明确p>0、指数非负和乘法宽度。正式 [math/modular.hpp](math/modular.hpp) 已有模1、负底数、极限乘法回归。
3. **读入EOF。** `read.cpp`驱动在空输入0.3秒超时；`getchar()`的int结果放进char，无法正确表达EOF。正常读入负数通过不代表EOF健壮性。
4. **三分题源错。** `Ternary_Search.cpp`写的 [2227/B](https://codeforces.com/contest/2227/problem/B) 实际是括号题Party Monster，和几何函数不符。没有核实真实题源和单峰性前，保留UNVERIFIED。
5. **数值和规模前提。** 旧Prim/Kruskal、背包、石子合并使用int，原DFS递归深度受栈限制，旧KMP不接收空模式。表中的VERIFIED均严格限于列明前提，不能直接扩到long long全域、大深树或空输入。
6. **示例不等于接口。** STL/lambda/sort是教学演示；lower_bound文件实际用降序upper_bound且不查end；tree_storage只读边；interaction只有头文件。它们不能纳入“已验证算法数量”。

## 已确认的正式替代

以下五个替代的源SHA与主任务已生成的 `build/release/results.json`、`build/sanitizers/results.json`完全一致，两个报告均为PASS；分别为Windows release与WSL ASan/UBSan。全量正式验证为每环境39/39检查，覆盖32个正式源码与3套测试文件的哈希。具体SHA与测试文件记录在manifest中。这里的FIXED只覆盖所列正式模块契约。

| 旧源 | 正式替代 | 状态与验证 |
|---|---|---|
| Fenwick_tree.cpp | [fenwick.hpp](data_structure/fenwick.hpp) | FIXED；接口回归、空树、负增量、随机数组对照、大规模测试 |
| Segment tree2.cpp | [lazy_segment_tree.hpp](data_structure/lazy_segment_tree.hpp) | FIXED；溢出/漏下传反例、随机区间操作、大规模测试；仅区间加/和 |
| ksm.cpp | [modular.hpp](math/modular.hpp) | FIXED；指数0模1、负底数、宽乘法与随机快速幂对照 |
| matrix_ksm.cpp | [product_recurrence.hpp](math/product_recurrence.hpp) | FIXED；初值、乘法单位元、零底数与小规模直接递推对照 |
| binary_lifting.cpp | [sparse_table.hpp](data_structure/sparse_table.hpp) | FIXED；完成原ST草稿对应的预处理/查询，随机gcd/min区间对照；不是LCA替代 |

其他`replacement_candidate`仅是导航，不代表已测或可以打印；主任务完成新模块验证后，应按新模块测试记录使用。旧32份源码始终保持上述原状态。

## 原文件完整性与编码

清单为每份文件保留三种SHA256：`sha256`是本次Windows工作区的原始字节（CRLF），`git_blob_sha256`是基线Git提交字节（通常LF），`sha256_lf_normalized`供跨平台核对。环境`core.autocrlf=true`导致前两者可能不同，不能误判为代码被改。

本次32份均满足：审计前后原始工作区SHA一致，且仅正规化行尾后与基线提交内容一致。少量旧中文注释/字符串的编码已有异常显示，未重新编码旧源。新正式文件采用统一编码。`legacy/`只提供[索引](legacy/README.md)，不再复制32份旧代码，避免产生第二套会漂移的旧源。

## 使用边界

比赛时从正式模块索引选择算法。旧目录用于追溯、反例教学和补充理解；即使标VERIFIED，也必须重新核对该题的规模、数值和下标契约。公开通过记录、当前本地源码、本地测试是三种独立证据，不互相替代。
