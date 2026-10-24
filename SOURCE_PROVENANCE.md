# SOURCE_PROVENANCE — 模块源码来源与复用边界

审计日期：2026-10-25。范围是 `notes/modules/*.json` 的 32 个保留模块与 `notes/round02/modules/*.json` 的 68 个本轮目录条目，共 100 条。逐项核对规范源路径、原思路、实际改动和接口；不把文件名相似、题型相近或 origins 出现某路径等同于直接代码继承。

本表的 `template/...` 相对当前 cf 工作区的 template 子模块，`cf/...` 相对当前 cf 仓库根目录。专题来源记录保留自己的哈希、具体题意限制和本地测试证据。当前在线提交状态全部为 **UNKNOWN**；用户说明原代码已经本人验证使用，这项经验与在线 AC、题面身份核对、本地测试分别记录。

## 证据和分类口径

- **原样引用**：本轮直接引用已有规范源，文件不改。**完整复制**：原程序与新路径逐字比较，仅允许 CRLF/LF 差异。
- **直接提取/最小适配/小优化**：旧核心循环或转移保留，但函数化、宽度、输入输出或名字有明确变化，不宣称字节相同。
- **扩展/混合**：旧核心与新功能并存；表中分别指出新增部分。**新写**：只有旧注释、性质、基础 API 或建模背景，完整实现由本轮增加。
- 下列“改动”描述新规范源与来源的差异；没有修改用户旧源。算法 PASS 引自模块、专题与最终统一验证报告；本来源审计核对报告及哈希，没有自行重跑算法测试。

## 保留基线与完整性

冻结基线：cf `d8d0b4ed1b58c54540e45c26f0a872e309a9edcf`，template `c0792d4a09e34c636eaff03fce69e9b83d8c10a4`。32 个旧根目录 `.cpp`、32 个 ROUND01 正式源码均与 [provenance_baseline.json](notes/round02/provenance_baseline.json) 的 SHA-256 一致；四份原 DOCX/PDF 亦保持原字节。原 32 正式源码的既有 release/sanitizers 记录与这些源码哈希匹配；这项基线哈希核验本身不等于重跑测试；本轮已完成的统一全目录回归另见文末报告。

个人仓库 215 份已跟踪 `.cpp` 的全量词法风格统计、50 份代表源码的深入阅读和精确行号，见 [source_inventory.json](notes/round02/source_inventory.json)、[PERSONAL_CODE_STYLE.md](PERSONAL_CODE_STYLE.md)。215 份统计不冒充 215 份语义审计。原文件的缺陷/前提逐项见 [CURRENT_COVERAGE_AUDIT.md](CURRENT_COVERAGE_AUDIT.md)；本表按后续专题实证更新复用关系，不扩大此前审计结论。

目录的 8 处 replaces 只选用本轮比赛接口，原模块及源码继续保留：DS01 → PSD01；DS02 → PSD02；DS03 → PSD10；G01 → PG02；G02 → PG03；G03 → PG04；G06 → PG05；G09 → PG06。

## ROUND01 保留模块：32 项

以下每项均直接引用冻结的 ROUND01 正式文件，本轮最小修改为“无”，接口为“原接口不变”。“个人相关源”是已读的同类原稿/复用候选，不能据此倒推 ROUND01 必定逐行复制了它；无候选时明确留空。本轮新增个人接口在后表单独说明。

| 模块 | 直接引用的规范源码 | 原思路/适用能力 | 个人相关源及关系边界 | 本轮改动/接口 |
|---|---|---|---|---|
| CO01 主程序骨架 Main | `template/core/main.cpp` | 每场比赛新建文件；单测、多测都从这份骨架开始。 | `cf/segment_tree_cf_edu/1.cpp:91-99` | 无；原样引用，原接口不变 |
| CO02 前缀、差分、离散化 Basics | `template/core/basics.hpp` | 静态区间和用前缀；离线区间加用差分；值域太大先离散化。 | `cf/3/1096/E_suffix.cpp:23-34`<br>`cf/3/1096/F_counting.cpp:21-27`<br>个人片段仅模型背景。 | 无；原样引用，原接口不变 |
| CO03 答案二分、双指针、窗口 Search | `template/core/search_window.hpp` | 答案满足单调判断时二分；有序数组找二元和用双指针；非负数组求最长受限和区间用窗口。 | `template/binary_search_num.cpp:9-19`<br>`cf/other/Hello_2018/D.cpp:31-77`<br>数组值二分与答案二分语义不同。 | 无；原样引用，原接口不变 |
| DP01 0/1 与完全背包 Knapsack | `template/dp/knapsack.hpp` | 按物品选择、总重量受容量限制，价值最大化；能区分每件一次与无限次。 | `template/01bag.cpp:4-18`<br>`template/完全背包dp.cpp:4-18` | 无；原样引用，原接口不变 |
| DP02 线性 DP 与最长递增子序列 LIS | `template/dp/lis.hpp` | 寻找保持原顺序的最长严格递增或非递减子序列；先用朴素状态理解，再用二分优化。 | `template/线性dp.cpp:4-19` | 无；原样引用，原接口不变 |
| DP03 小规模状压最短路径 Hamilton DP | `template/dp/hamilton_path.hpp` | n很小，需要每个点恰好访问一次；用mask记录已选集合，末尾点作为第二维。 | `cf/luogu/5.cpp:20-38`<br>`cf/ICPC/JAKARTA_2018/J.cpp:18-35`<br>相关子集枚举不等于 Hamilton 路径实现。 | 无；原样引用，原接口不变 |
| DP04 石子合并区间 DP Interval Merge | `template/dp/interval_merge.hpp` | 只能合并相邻部分，最后一次操作把区间分成左右两段，总代价可以由子区间转移。 | `template/区间dp.cpp:4-25` | 无；原样引用，原接口不变 |
| DS01 树状数组 Fenwick | `template/data_structure/fenwick.hpp` | 单点加、前缀和、区间和；频次累计、逆序对可先离散化。 | `template/Fenwick_tree.cpp:8-31`<br>旧稿本身已有 struct/vector；本轮个人版为 PSD01。 | 无；原样引用，原接口不变；主目录另选 PSD01 |
| DS02 单点线段树 Segment Tree | `template/data_structure/segment_tree.hpp` | 单点赋值、区间求和；需要以后扩展节点信息时使用。 | `template/segmen tree1.cpp:18-89`<br>`cf/segment_tree_cf_edu/1.cpp:5-55` | 无；原样引用，原接口不变；主目录另选 PSD02 |
| DS03 懒标记线段树 Lazy Segment Tree | `template/data_structure/lazy_segment_tree.hpp` | 在线区间加、区间和；更新整段后仍需查询任意子段。 | `template/Segment tree2.cpp:11-88` | 无；原样引用，原接口不变；主目录另选 PSD10 |
| DS04 单调栈与单调队列 Monotonic | `template/data_structure/monotonic.hpp` | 找每个位置左侧最近更小元素；固定窗口查询最小值。 | 未核实对应个人完整实现；不补造来源 | 无；原样引用，原接口不变 |
| DS05 稀疏表 Sparse Table | `template/data_structure/sparse_table.hpp` | 数组不修改，需要大量区间min/max/gcd查询。 | `template/ST表.cpp:15-37` | 无；原样引用，原接口不变 |
| G01 并查集 Disjoint Set Union | `template/graph/dsu.hpp` | 只增加连边、判断同集合、维护连通块大小；Kruskal 的基础。 | `template/DSU.cpp:13-36`<br>`cf/xian/B.cpp:18-41` | 无；原样引用，原接口不变；主目录另选 PG02 |
| G02 BFS 最短路与路径恢复 BFS Path | `template/graph/bfs_path.hpp` | 无权图最短距离、最少操作次数、输出一条最短路径。 | `template/bfs.cpp:4-25` | 无；原样引用，原接口不变；主目录另选 PG03 |
| G03 迭代 DFS 连通块与父亲 Iterative DFS Forest | `template/graph/dfs_forest.hpp` | 无向图连通块标号、父亲树、需要避开递归栈溢出的深图遍历。 | `template/dfs.cpp:4-14`<br>`template/tree_storage.cpp:6-19` | 无；原样引用，原接口不变；主目录另选 PG04 |
| G04 拓扑排序 Topological Sort | `template/graph/topological_sort.hpp` | DAG 处理顺序、依赖任务、DAG DP、有向环检测。 | `cf/1704E.cpp:58-73` | 无；原样引用，原接口不变 |
| G05 简单无向基环图环恢复 Unicyclic Graph | `template/graph/unicyclic_graph.hpp` | 连通简单无向图恰有一个环，找环点、按环顺序做双指针或 DP。 | 未核实对应个人完整实现；不补造来源 | 无；原样引用，原接口不变 |
| G06 单源与多源最短路 Dijkstra | `template/graph/dijkstra.hpp` | 非负边权最短路；多个起点同时出发时求离最近源点的距离；恢复一条最短路径。 | 未核实对应个人完整实现；不补造来源<br>本轮 PG05 来自此已验证正式版。 | 无；原样引用，原接口不变；主目录另选 PG05 |
| G07 Kruskal 最小生成树与森林 Kruskal MST | `template/graph/kruskal.hpp` | 无向带权图求最小生成树；图可能不连通时求最小生成森林。 | `template/kruskal MST.cpp:9-70` | 无；原样引用，原接口不变 |
| G08 堆优化 Prim 最小生成森林 Prim MST | `template/graph/prim.hpp` | 已有带权无向邻接表，需要最小生成树或所有分量的最小生成森林。 | `template/prim MST.cpp:5-58` | 无；原样引用，原接口不变 |
| G09 树上倍增最近公共祖先 Binary Lifting LCA | `template/graph/lca.hpp` | 静态树上多次 LCA、两点边数距离、向上跳 k 级祖先查询。 | 未核实对应个人完整实现；不补造来源<br>binary_lifting.cpp 不是 LCA 源；PG06 来源为本文件。 | 无；原样引用，原接口不变；主目录另选 PG06 |
| G10 Euler 子树区间与迭代树 DP Euler Subtree | `template/graph/euler_subtree.hpp` | 将树上子树映射为数组闭区间，配合前缀和/Fenwick/线段树；逆序处理简单树形 DP。 | `template/tree_storage.cpp:6-19`<br>`template/tree_dp.cpp:8-19`<br>`cf/hdu/B.cpp:19-30` | 无；原样引用，原接口不变 |
| G11 强连通分量 Iterative Kosaraju SCC | `template/advanced/scc.hpp` | 有向图中两两互相可达的分量；缩点成 DAG 的第一步。 | 未核实对应个人完整实现；不补造来源 | 无；原样引用，原接口不变 |
| G12 无向图找桥 Iterative Bridges | `template/advanced/bridges.hpp` | 找删除后连通块数增加的边；边双连通分量与桥树的基础。 | 未核实对应个人完整实现；不补造来源 | 无；原样引用，原接口不变 |
| M01 整数与位运算 Integer and Bits | `template/math/integer_bits.hpp` | 最大公约数、最小公倍数、二进制计数、取最低位、枚举mask的所有子集。 | `template/gcd.cpp:6-8`<br>`cf/2/873/B.cpp:6-18` | 无；原样引用，原接口不变 |
| M02 安全模运算与快速幂 Modular Power | `template/math/modular.hpp` | 大整数模乘、非负整数快速幂、负数归一化，以及质数条件成立时求逆元。 | `template/ksm.cpp:7-15`<br>`cf/date/5/5.19/3.cpp:22-33` | 无；原样引用，原接口不变 |
| M03 普通模矩阵快速幂 Matrix Power | `template/math/matrix_power.hpp` | 线性递推阶数很小但n很大，能把一次转移写成固定矩阵乘状态列向量。 | `template/matrix_ksm.cpp:16-40`<br>普通模矩阵乘法，不是 max-plus。 | 无；原样引用，原接口不变 |
| M04 乘积递推修正版 Product Recurrence | `template/math/product_recurrence.hpp` | 原1182E乘积递推的指数转移示例；用矩阵求各底数指数，再做模幂。理解M02、M03后再读。 | `template/matrix_ksm.cpp:54-91` | 无；原样引用，原接口不变 |
| M05 质数模数组合数 Prime Combinations | `template/math/combinations.hpp` | 大量询问C(n,k)，所有n有可承受的预处理上限，模数已知为质数且大于该上限。 | `cf/date/5/5.19/3.cpp:19-46` | 无；原样引用，原接口不变 |
| S01 KMP 与 Z 函数 Prefix Matching | `template/string/kmp_z.hpp` | 线性时间寻找全部模式串出现位置、分析前后缀相等长度，或比较字符串每个后缀与全串前缀。 | `template/KMP.cpp:21-60`<br>`cf/date/6/6.14/1.cpp:4-42` | 无；原样引用，原接口不变 |
| S02 双模字符串哈希 Double Hash | `template/string/double_hash.hpp` | 大量查询子串指纹、子串去重或快速筛选相等候选；可以接受碰撞风险时使用。 | 未核实对应个人完整实现；不补造来源 | 无；原样引用，原接口不变 |
| S03 基础字符串处理 String Basics | `template/string/basic_string.hpp` | 回文检查、单字符分割、ASCII大小写、相邻相同字符分组，以及含空格输入。 | `cf/construction/1.cpp:13-64`<br>回文构造与基础回文判断能力不同。 | 无；原样引用，原接口不变 |

## ROUND02 — 运行、STL、mex、博弈与字符串

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| IO01 原版 ll/getchar 快读<br>`template/read.cpp` | `template/read.cpp` | **原样引用**：原 inline ll read() 的 getchar、符号与十进制累积。 | read.cpp 字节不动；不加入 EOF 包装、不改 fread；常规合法整数使用原版，其他输入用 cin。 | inline ll read() 原接口。 |
| IO02 cin、字符串字符与定点输出<br>`template/core/main.cpp` | `template/core/main.cpp`<br>`template/core/basics.hpp` | **原样引用**：ROUND01 solve/T/cin 主程序骨架；basics.hpp 提供宽整数输出。 | 直接引用 CO01 与 CO02；没有另写一份主程序或重复宽整数工具。 | main/solve 保留；是否读 T 由题目决定。 |
| IO03 cin字符串转i128与原输出函数<br>`template/competition/runtime/int128_input.hpp` | `template/core/basics.hpp` | **新写输入辅助，原样引用输出**：ROUND01 已有 to_string_i128；没有个人旧字符串转 i128 实现。 | 新增合法十进制字符串解析，使用无符号 128 位累积并以 -(value-1)-1 处理负极值；cin 调用示例为本轮新增。原 read.cpp、CO02 输出函数均不改；这不是快读替代实现。 | 新增 parse_i128(const string&)；先 cin 读 string，再转换。输出依赖 CO02；普通 ll 仍用原 read()。 |
| FP01 浮点误差、距离与反三角<br>`template/competition/runtime/floating.hpp` | `template/Ternary_Search.cpp` | **新写**：旧 Ternary_Search.cpp 只提供实数运算、sqrt 与精度情境。 | 比较、钳制、角度/几何精度工具为本轮新增；旧三分文件不是这些函数的完整实现来源。 | 新增独立 long double 工具接口，契约见 FP01 元数据。 |
| TRS01 实数二分与三分<br>`template/competition/runtime/real_search.hpp` | `template/Ternary_Search.cpp` | **提取并扩展**：旧 Ternary_Search.cpp 的固定 200 次三分迭代。 | 提取三分循环，去掉题目专用目标式；实数二分为新增能力。 | 目标函数/判断器作为参数；返回 long double 搜索点。 |
| STL01 可执行 STL 容器与算法短例<br>`template/competition/stl/recipes.hpp` | `template/data_sturcture.cpp` | **扩展示例**：data_sturcture.cpp 的容器调用习惯。 | 沿用原有调用并补全缺失容器、算法与字符串示例；新增示例不冒充原文件已有内容。 | stl_container_recipes、stl_algorithm_recipes、stl_string_recipes。 |
| STL02 堆与有序集合的典型题型<br>`template/competition/stl/heaps_sets.hpp` | `template/data_sturcture.cpp` | **新写**：data_sturcture.cpp 的 priority_queue/set/multiset 基础 API。 | Huffman、Top-K、双堆中位数、版本懒删除、严格邻居、资源匹配和滑窗为新增模型；API 示例不是完整算法来源。 | huffman_cost、top_k_largest/smallest、median_*、lazy_*、strict_neighbors、assign_at_least、window_*。 |
| MEX01 静态与前缀 MEX<br>`template/competition/game/mex.hpp` | `template/mex.cpp`<br>`cf/3/1096/D_mex_set.cpp` | **新写**：旧 mex.cpp 的 vis 标记思路；D_mex_set.cpp 是特定回文中心模型的背景。 | 实现通用静态 mex 与前缀 mex；原评论/题内局部集合不等于已有通用函数。 | static_mex、prefix_mex；与 MEX02 共用同一规范源码。 |
| MEX02 频率与缺失 set 动态 MEX<br>`template/competition/game/mex.hpp` | `template/mex.cpp` | **新写**：旧 mex.cpp 的计数与零计数集合思路。 | 补成支持插入、删除与多次初始化的动态 mex；原稿以说明为主。 | 全局 cnt/missing 与 mex_init/mex_add/mex_erase/mex_query；与 MEX01 共源。 |
| GAME01 正常规则、Nim 胜着与有限 DAG SG<br>`template/competition/game/normal_play.hpp` | `template/mex.cpp` | **新写**：旧 mex.cpp 的 mex/SG 文字观察。 | Nim 胜步、拓扑 DAG 的 SG 和 misere Nim 为新增可执行实现；旧稿没有这些完整程序。 | 新增 Nim、SG 与 misere 判定函数。 |
| PAL01 回文判定、中心扩展与构造<br>`template/competition/string/palindrome.hpp` | `template/string/basic_string.hpp`<br>`cf/construction/1.cpp`<br>`cf/3/1096/D_mex_set.cpp` | **混合**：S03 基础字符串；construction/1.cpp 镜像补全；D_mex_set.cpp 中心扩展。 | S03 直接引用；二进制回文保留原成对传播，补计数与失败返回；中心扩展提成局部函数。多重集回文排列、十进制回文判断为新增。 | binary_palindrome_fill 返回 optional；其余独立函数，不沿用原 solve 输出接口。 |
| TRS02 个人数组字符串 Trie：前缀频次与整词频次<br>`template/competition/string/trie.hpp` | `cf/ICPC/2026ICPCWeb1/L.cpp` | **提取并扩展**：ICPCWeb1/L.cpp 的 tr[][26]、cnt、dep、tot 与逐字符插入。 | 保留字母 Trie 插入骨架；新增 word_end 精确词频、查询及已用节点清理；原 best/ans 题意逻辑不搬入。 | trie_init/trie_insert/trie_node/trie_prefix/trie_count；容量契约单独写明。 |

## ROUND02 — 基础建模与原完整程序

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| BM01 区间单位操作与总变差<br>`template/competition/models/interval_count.hpp` | `cf/date/6/6.9/1_special_segment_tree.cpp` | **新写**：特殊线段树练习展示把题意转成维护量的建模习惯。 | 区间单位增减的差分总变差计数为新增推导与实现，不继承该练习的 d[x]、mn/pos 算法。 | unit_intervals、prefix_units、suffix_units、binary_interval_flips、adjacent_moves。 |
| BM02 最少区间操作的压缩构造与变体<br>`template/competition/models/interval_construct.hpp` | `template/competition/models/interval_count.hpp` | **新写**：本轮 BM01 的差分计数结论。 | 补可执行压缩操作构造，以及非零差分个数至多 20 的精确子集 DP；来源是本轮新模块，不能反向当成用户旧代码。 | construct_unit_intervals、arbitrary_intervals_small 及压缩区间构造。 |
| BM03 二维前缀和与矩形差分<br>`template/competition/models/prefix_grid.hpp` | `template/core/basics.hpp` | **新写**：CO02 的一维前缀与差分概念。 | 新增二维容斥前缀、矩形四角差分及恢复；不是直接复制一维实现。 | 全局 a/s/d；init/build_prefix/query/add_rect/restore_difference。 |
| BM04 逆序对与折半枚举<br>`template/competition/models/enumeration.hpp` | `template/core/basics.hpp` | **新写**：CO02 的排序、离散化等基本操作。 | 归并逆序对与折半子集和计数是新增实现；原 basics.hpp 没有这两套算法。 | merge_count、subset_sum_count。 |
| BM05 原解直接收录 二分选题 CF913D<br>`template/competition/models/choose_tasks.cpp` | `cf/other/Hello_2018/D.cpp` | **完整复制**：Hello_2018/D.cpp 原中文二分推导、check、选题输出。 | 规范 .cpp 与原程序 LF 标准化后全文一致；无算法或接口修改。 | 原 check/main 与输入输出均保留。 |
| BM06 原解直接收录 选最小可行值 CF2065C2<br>`template/competition/models/ordered_choice.cpp` | `cf/date/7/7.25/1.cpp` | **完整复制**：date/7/7.25/1.cpp 选最小可行候选、lower_bound 贪心。 | 规范 .cpp 与原程序 LF 标准化后全文一致；无算法或接口修改。 | 原 solve/main 与输入输出均保留。 |
| BM07 区间选择、首尾配对与括号判定<br>`template/competition/models/greedy_construct.hpp` | `template/competition/models/greedy_construct.hpp` | **新写**：通用区间调度、排序配对和括号构造。 | 全部本轮新写；origins 自指仅定位新规范文件，不是历史来源证明。 | interval_schedule、minmax_pair_sum、regular_brackets。 |
| BM08 整数状态BFS与非负子集回溯剪枝<br>`template/competition/models/state_search.hpp` | `template/competition/stl/recipes.hpp`<br>`template/competition/graph/bfs.hpp` | **新写并参考本轮骨架**：STL01 的选/不选递归示例、PG03 的队列与前驱结构。两项来源也是本轮规范文件，不是用户原练习。 | 新增有限边界内 +1/-1/乘2 的状态转移及操作路径；新增非负子集目标、后缀和上界与已选下标回溯。旧基础示例没有这些完整模型；不声称任意截断边界保证无界问题最优。 | integer_state_bfs 返回 IntegerPath{states,operations}；subset_target_nonnegative 返回 `optional<vector<int>>` 原下标；重复值按位置区分。 |

## ROUND02 — 动态规划

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| PDP01 线性与网格：最大子段、路径计数、最小代价<br>`template/competition/dp/linear_grid.hpp` | `cf/date/6/6.9/3_leaner_dp.cpp`<br>`cf/date/5/5.22/2.cpp` | **混合**：收费最大连续段的余数状态；date/5/5.22/2.cpp 的障碍网格右下路径。 | 普通最大子段为无收费模型新实现，不能删原题余数后称等价；网格计数提取局部递推并滚动存储；最小路径和新增。 | max_subarray、grid_paths、grid_min_path；独立返回值，模数/网格作为参数。 |
| PDP02 多重背包与分组背包<br>`template/competition/dp/knapsack_variants.hpp` | `template/dp/knapsack.hpp` | **引用并扩展**：DP01 已验证 0/1 与完全背包。 | 数量二进制拆分后直接调用现有 knapsack01；分组背包新增层快照。DP01 不重写。 | 新增 knapsack_bounded/knapsack_grouped，保留 DP01 的既有契约。 |
| PDP03 序列前缀与个人状态机、候选分层 DP<br>`template/competition/dp/sequence_state.hpp` | `cf/date/7/7.9/1_interesting_dp.cpp`<br>`cf/ICPC/JAKARTA_2018/J.cpp` | **混合**：方向状态机 dp0/dp1；JAKARTA/J 子序列排序去重与双指针前缀 max。 | 这两段递推保留后提函数；补空目标/空链与精确 NEG。LCS、编辑距离为本轮新写，不能归到两个旧文件。 | right_then_left_match 返回 bool；lex_subsequence_chain 返回 ll、对象从 1 开始；新增 lcs_length/edit_distance。 |
| PDP04 个人数位 DP 与基础有限状态计数<br>`template/competition/dp/digit_count.hpp` | `cf/date/8/1/2_digit_dp.cpp` | **混合**：原数位 DP 的 s、dp[pos][cnt] 与 dfs(pos,cnt,limit)，原统计包含 0。 | 保留至多三个非零十进制位递推；仅提函数、调整遮蔽命名与索引类型。binary_no_adjacent 是另行新增的二进制有限状态计数。 | digit_calc、digit_range；新增 binary_no_adjacent，不改变旧数位模型含 0 的语义。 |
| PDP05 树形 DP：最大权独立集<br>`template/competition/dp/tree_independent_set.hpp` | `template/tree_dp.cpp`<br>`cf/3/1096/H_!!dp_tree.cpp` | **新写**：tree_dp.cpp 的先子后父骨架；个人叶子奇偶树 DP 的子树压缩状态观察。 | 新增最大权独立集 dp[不选/选] 及逆序合并；旧叶子奇偶公式不是独立集公式。 | 独立树最大权独立集接口；迭代顺序避免深链递归。 |
| PDP06 独立试验概率与阶段等待期望<br>`template/competition/dp/probability_expectation.hpp` | 无个人旧实现；本轮新写 | **新写**：标准概率分布与线性期望递推。 | 本轮新增硬币正面概率、独立阶段成功次数期望；origins 为空是没有个人旧实现，不能补造出处。 | coin_heads_probability、expected_stage_attempts。 |

## ROUND02 — 图与树

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| PG01 树的存储、多测与根化<br>`template/competition/graph/tree_storage.hpp` | `template/tree_storage.cpp`<br>`cf/3/1096/H_!!dp_tree.cpp` | **保留骨架并扩展**：旧 tree_storage.cpp 的 g[N]、n-1 边双向 push；树 DP 的父边与后序顺序。 | 保留存储/读边思路，新增清理、根化、fa/dep/sz/order 与迭代遍历；原树基础代码仍原样保留。 | init、read_tree(istream)、root_tree 返回 bool；全局 g/fa/dep/sz/order。 |
| PG02 个人原版并查集 fa/rk<br>`template/competition/graph/dsu.hpp` | `template/DSU.cpp`<br>`cf/xian/B.cpp`<br>`cf/3/1109/3.cpp` | **最小适配**：DSU.cpp 与 xian/B.cpp 的 fa/rk/init/find/unite/same。 | 保留按秩合并与路径压缩；修反向文字注释、容量与头文件形式。3/1109/3 是调用场景，不是替换算法。 | unite 仍为 void；init/find/unite/same 保留；替代主目录 G01，不删除 G01。 |
| PG03 原队列BFS与最短路径恢复<br>`template/competition/graph/bfs.hpp` | `template/bfs.cpp`<br>`template/graph/bfs_path.hpp` | **提取并扩展**：旧 bfs.cpp 的 queue 和入队标记；ROUND01 BFS 路径实现。 | 保留遍历核心，补 dist/fa/path 和多测清理；距离与恢复路径为追加能力。 | bfs(start)、init、path；全局数组接口，替代主目录 G02。 |
| PG04 显式栈DFS与连通块<br>`template/competition/graph/dfs.hpp` | `template/dfs.cpp`<br>`template/graph/dfs_forest.hpp` | **接口与遍历适配**：旧 dfs.cpp 递归逐邻边；ROUND01 迭代 DFS 森林。 | 用显式帧栈保留 DFS 访问顺序并支持深链、全组件；不是字节级复制旧递归。 | dfs(start)、dfs_forest、init 与 comp/order；替代主目录 G03。 |
| PG05 全局数组Dijkstra<br>`template/competition/graph/dijkstra.hpp` | `template/graph/dijkstra.hpp` | **接口适配**：ROUND01 graph/dijkstra.hpp 已验证堆 Dijkstra；无已核实个人旧 Dijkstra 完整实现。 | 保留最小堆、过期条目判断、非负权/INF 约束，转为全局 dist/fa；不虚构个人算法来源。 | dijkstra 的起点/多源版本返回 bool，独立 path/init；替代主目录 G06。 |
| PG06 个人树上倍增LCA<br>`template/competition/graph/lca.hpp` | `template/graph/lca.hpp`<br>`template/tree_storage.cpp` | **接口适配**：ROUND01 graph/lca.hpp 倍增；个人 g[N] 树存储。 | 保留可靠倍增算法，改为 PG01 全局存储与独立函数；旧 binary_lifting.cpp 是未完成 ST，不能作为 LCA 实现来源。 | build_lca、jump、lca、distance；依赖 PG01，替代主目录 G09。 |
| PG07 四邻接网格BFS<br>`template/competition/graph/grid_bfs.hpp` | `template/bfs.cpp` | **新写**：旧 bfs.cpp 的无权队列分层。 | 新增二维坐标、边界和障碍处理；旧图 BFS 没有完整网格接口。 | bfs(sx,sy) 内清理 dist；全局 n/m/g/dist，网格按 1 下标契约。 |
| PG08 多源BFS<br>`template/competition/graph/multisource_bfs.hpp` | `template/bfs.cpp` | **扩展**：旧 bfs.cpp 入队标记；实际复用 PG03 的规范状态。 | 新增多源同时入队、起点去重和清理。 | bfs(vector 起点)；依赖 PG03。 |
| PG09 0-1 BFS<br>`template/competition/graph/zero_one_bfs.hpp` | `template/graph/dijkstra.hpp` | **新写**：ROUND01 Dijkstra 的距离与松弛概念。 | 新增 0/1 权双端队列，0 权入前、1 权入后；不是从旧代码直接提取的完整算法。 | init、bfs01；边权只能为 0 或 1。 |
| PG10 Floyd全源最短路<br>`template/competition/graph/floyd.hpp` | `template/graph/dijkstra.hpp` | **新写**：ROUND01 Dijkstra 的不可达 INF 约定。 | 新增 Floyd 三重松弛、重边取小值与不可达保护；旧单源算法不含 Floyd。 | init、add_edge、floyd 返回 bool。 |
| PG11 二分图染色与冲突检测<br>`template/competition/graph/bipartite.hpp` | `template/graph_paint.cpp`<br>`cf/date/6/6.16/2_bi_paint_.cpp` | **保留骨架并补全**：graph_paint.cpp 的 3-col[u]；个人实战的全组件 BFS 染色。 | 保留染色循环，补通用同色边冲突检测与初始化；不能把特殊题图保证当通用条件。 | init、bipartite 返回 bool；col 为全局颜色。 |
| PG12 有向/无向环恢复<br>`template/competition/graph/cycle.hpp` | `template/graph/dfs_forest.hpp`<br>`template/advanced/bridges.hpp` | **新写**：ROUND01 DFS 显式帧、桥算法的无向边 id。 | 新增有向 0/1/2 着色与无向 parent-edge 排除、完整环恢复；平行边不按父点误跳过。 | init、find_cycle(bool 有向标记)，返回环。 |
| PG13 无权树直径与路径<br>`template/competition/graph/tree_diameter.hpp` | `template/tree_storage.cpp`<br>`template/bfs.cpp` | **新写并复用存储**：旧 tree_storage.cpp 邻接表和 bfs.cpp 分层。 | 复用 PG01，新增树上两次最远点搜索、直径恢复；旧文件未提供这套完整接口。 | farthest、tree_diameter；依赖 PG01，仅树契约。 |
| PG15 DAG路径计数与最长路<br>`template/competition/graph/dag_dp.hpp` | `cf/1704E.cpp`<br>`template/graph/topological_sort.hpp` | **提取拓扑骨架并新写模型**：1704E.cpp 的 Kahn 与逆拓扑沿边累加；ROUND01 拓扑排序。 | 保留拓扑依赖思想；去掉原模拟与特定汇点，新写通用起点路径数/最长路 DP。 | init、dag_dp(s,mod)；环检测和不可达语义另定。 |

## ROUND02 — 数学与整数几何

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| PM01 扩展欧几里得与一般模逆元<br>`template/competition/math/exgcd.hpp` | `template/gcd.cpp`<br>`template/ksm.cpp` | **新写**：旧 gcd 的 Euclid 递归；ksm 注释中的质数逆元。 | 新增 Bézout 系数回代及一般模逆；旧 gcd 无系数、Fermat 逆元不是复合模算法。 | exgcd(a,b,x,y)、inverse_mod；新 API，非“接口无变化”。 |
| PM02 线性筛、判素、分解与约数<br>`template/competition/math/primes.hpp` | `template/Mobius_inversion.cpp` | **提取并扩展**：Mobius_inversion.cpp 中可靠 init_mu 线性筛循环。 | 只取该可靠筛骨架，不使用存在覆盖赋值问题的 get_mu；补 lp、质性、分解与约数工具。 | 新 sieve/质性/分解/约数接口，详细名与契约见 PM02。 |
| PM03 负数向下/向上整除<br>`template/competition/math/integer_division.hpp` | `cf/ICPC/JAKARTA_2018/H.cpp` | **新写**：未完成 JAKARTA/H 的非负取整表达式只是需求背景。 | 新增有符号 floor/ceil 除法，覆盖异号与余数；不声称补完原题。 | floor_div、ceil_div；明确分母和溢出契约。 |
| PM04 基础求和与成对差贡献<br>`template/competition/math/sums.hpp` | `cf/date/7/7.28/1.cpp` | **混合**：date/7/7.28/1.cpp 排序后 a[i]*(2*i-n+1) 成对差贡献。 | pair_abs_sum 保留贡献系数，I/O 分离并扩 i128 中间值；sum1、sum2、arithmetic_sum 为本轮新写公式函数。 | `pair_abs_sum(vector<ll>)` 与三个求和函数，宽整数返回。 |
| PM05 奇偶、模不变量与环上可达<br>`template/competition/math/invariants.hpp` | `cf/2/873/B.cpp`<br>`cf/2/750/A.cpp`<br>`cf/3/1109/3.cpp` | **新写**：位移 gcd、总和奇偶、有限区间 DSU 连通的局部观察。 | 新增模归一/同余/模环可达工具；有限区间 DSU 不能直接替换为模环 gcd 结论。 | norm_mod、congruent、reachable_mod；适用对象由契约限制。 |
| PM06 整数点积、叉积与线段相交<br>`template/competition/math/integer_geometry.hpp` | `template/math/integer_bits.hpp` | **新写**：ROUND01 integer_bits.hpp 只提供宽整数处理背景。 | Point、点积、叉积、方向、在线段上与线段相交为新增整数几何；不是原文件已有几何代码。 | Point、dot/cross/orientation/on_segment/segments_intersect。 |

## ROUND02 — Fenwick 与线段树

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| PSD01 Fenwick：个人add/query/rangeSum<br>`template/competition/data_structure/fenwick.hpp` | `template/Fenwick_tree.cpp` | **保留核心并改名**：原 Fenwick 就是 struct/vector、n/tr、add 与 lowbit 循环。 | 保留结构；原 main 调 rangeSum 而只定义 query(l,r)。实际版本把 sum(x) 改 query(x)、query(l,r) 改 rangeSum(l,r)，并加断言、避免 size 遮蔽；改名多于只修调用。 | add 不变；query(x) 前缀、rangeSum(l,r) 区间，无 query(l,r) 重载；替代 DS01。 |
| PSD02 单点树公共骨架与区间和<br>`template/competition/data_structure/segment_point_sum.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **保留骨架并适配**：segmen tree1.cpp、EDU/1.cpp 全局数组单点赋值与求和。 | 保留 build/pushup/递归修改，update 改 modify；聚合改 Node；只递归相交儿子。 | query 返回 Node.sum；正式输入 1 下标闭区间，非 EDU 的 0 下标半开输入；替代 DS02。 |
| PSD03 单点树：区间最小值<br>`template/competition/data_structure/segment_point_min.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp`<br>`cf/segment_tree_cf_edu/2.cpp` | **保留骨架并适配**：单点树骨架与 EDU/2.cpp 的 min 合并。 | 保留 min 逻辑；为完整 ll 契约取消有限 4e18 空段哨兵，只访问相交儿子；不否定原题常规值域实现。 | modify 单点赋值；query 返回 Node.mn。 |
| PSD04 单点树：区间最大值与阈值搜索<br>`template/competition/data_structure/segment_point_max.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **骨架复用，聚合新写**：原求和树的全局递归骨架。 | 本轮新增 max 节点与区间内首个 >= 阈值的搜索；原 sum 文件并无 first_ge。 | query 返回 Node.mx；新增 first_ge。 |
| PSD05 单点树：区间gcd<br>`template/competition/data_structure/segment_point_gcd.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **骨架复用，聚合新写**：原求和树的全局递归骨架。 | 本轮新增 gcd 节点、非负 gcd 约定；禁止 LLONG_MIN 是本模块契约。 | query 返回 Node.g；modify 单点赋值。 |
| PSD06 单点树：最大值及出现次数<br>`template/competition/data_structure/segment_max_count.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **骨架复用，聚合新写**：原求和树的全局递归骨架。 | 本轮新增最大值与出现次数合并；不是原求和代码直接已有的能力。 | query 返回 Node.mx/.cnt。 |
| PSD07 单点树：最大非空连续子段和<br>`template/competition/data_structure/segment_max_subarray.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **骨架复用，聚合新写**：原求和树的全局递归骨架。 | 本轮新增 sum/pref/suff/best 非空最大子段合并；全负保留最大负数。 | query 返回 Node.best；其余字段支持合并。 |
| PSD08 频率线段树：查第k个<br>`template/competition/data_structure/segment_frequency.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **骨架复用，搜索新写**：原求和树的全局递归骨架。 | 新增非负频率语义与按左子树和找第 k 个；非法 k 返回 -1。 | query 返回 Node.sum；新增 kth，modify 是把频率赋值。 |
| PSD09 单点树：多个可合并统计量<br>`template/competition/data_structure/segment_point_stats.hpp` | `template/segmen tree1.cpp`<br>`cf/segment_tree_cf_edu/1.cpp` | **骨架复用，聚合新写**：原求和树的全局递归骨架。 | 本轮新增同时 sum/min/max/gcd 的节点及合并，禁止 LLONG_MIN。 | query 返回 Node.sum/.mn/.mx/.g。 |
| PSD10 区间加：和、最小值、最大值<br>`template/competition/data_structure/segment_range_add.hpp` | `template/Segment tree2.cpp`<br>`cf/date/6/6.9/1_special_segment_tree.cpp` | **最小修补并扩展**：Segment tree2.cpp 区间加；特殊树的 apply/pushdown 与最小值。 | 修声明顺序、ll 区间和、查询下传、多测建树清 lazy；新增 sum/min/max 联合 Node，原题 pos 不搬入。 | modify 区间加；query 返回 Node.sum/.mn/.mx；替代 DS03。 |
| PSD11 区间赋值与区间加：和<br>`template/competition/data_structure/segment_set_add.hpp` | `template/Segment tree2.cpp`<br>`cf/date/6/6.9/1_special_segment_tree.cpp` | **保留递归，混合标记新写**：原区间加树的递归层次与 build 清标记。 | 保留 ll 求和；修共同声明/下传/宽度问题；本轮新增 Tag{has_set,set,add}，新 set 覆盖旧操作、下传先 set 后 add，支持赋值 0。 | modify(...,val,is_set)：true 覆盖、false 增加；query 直接返回 ll，非 Node，未继承原题 mn/pos。 |

## ROUND02 — XOR 与位运算

| 模块与规范源 | 原始文件/背景路径 | 复用关系与原思路 | 实际最小改动或新增部分 | 接口变化 |
|---|---|---|---|---|
| X01 前缀 XOR、子数组计数与离线区间异或<br>`template/competition/xor/prefix_xor.hpp` | `cf/1/1124/C.cpp` | **新写**：1/1124/C.cpp 原注释的区间前缀 XOR 观察；solve 为空。 | 新增前缀/子数组 XOR 计数及离线 XOR 差分工具；不是从空 solve 抽取代码，也不等于该题完整解。 | xor_prefix、range_xor、count_subarray_xor、xor_offline_update。 |
| X02 按位贡献：数对与所有子数组<br>`template/competition/xor/bit_contribution.hpp` | `cf/4/1090/E_xor_find_pattern.cpp` | **新写**：最大数对 XOR 原练习只作问题对比。 | 新增数对 XOR/OR/AND 的加法总和、子数组 XOR 值的加法总和及再 XOR；原二次枚举并未实现这些函数。 | pair_bit_sums、subarray_xor_sum、all_subarray_xor、to_string_u128；宽整数计数。 |
| X03 XOR 构造、未知元素与最高不同位<br>`template/competition/xor/xor_construct.hpp` | `cf/2/1129/B.cpp` | **混合**：2/1129/B.cpp 的 mx^mn 低位全 1 掩码。 | interval_or 保留原循环并扩无符号 64 位；xor_missing、xor_1_to_n、xor_distinct 为本轮新增，不能统称原程序提取。 | interval_or 及三个新增独立 XOR 工具；原 solve I/O 分离。 |
| X04 数组节点二进制 Trie：插入、删除、最大/最小 XOR<br>`template/competition/xor/binary_trie.hpp` | `cf/4/1090/E_xor_find_pattern.cpp` | **新写**：最大数对 XOR 的原二次枚举作为对照与独立基线。 | 新增二进制 Trie 插入/删除/极值 XOR 与最大子数组 XOR；原题总 n<=3105 的二次法可用，不以臆测 TLE 为重写理由。 | trie_init/insert/erase、最小/最大 XOR 查询及数对/子数组工具。 |
| X05 图上的 XOR 约束<br>`template/competition/xor/graph_xor.hpp` | `cf/1/1124/B.cpp` | **新写**：popcount 原注释里的 XOR 抵消只作性质背景。 | 新增图上 XOR 等式赋值与冲突检测；原库存计数程序不是图约束实现。 | 独立图 XOR 约束求解接口。 |
| X06 XOR 线性基：插入与最大子集 XOR<br>`template/competition/xor/linear_basis.hpp` | `cf/4/1090/E_xor_find_pattern.cpp` | **新写**：原最大数对 XOR 仅用来区分“选两数”与“任意子集”。 | 新增 64 位线性基 b/rk、插入和最大 XOR；绝不把数对暴力说成线性基旧实现。 | basis_init、basis_insert、basis_max；P2 模块。 |
| X07 我的偶数 popcount 计数模型（2268B）<br>`template/competition/xor/popcount_inventory.hpp` | `cf/1/1124/B.cpp` | **直接提取**：1/1124/B.cpp 的 good(int) 与点修改计数。 | 原 good(int) 及更新逻辑保留，无算法修复。新增证明说明为何两个保留位置足够；[5,0] 反驳的是旧文字论证，不是原答案。 | I/O 分离为函数；原值域 0..15 不扩大成虚构缺陷。 |
| X08 我的最大数对双循环（2218E）<br>`template/competition/xor/max_pair_quadratic.hpp` | `cf/4/1090/E_xor_find_pattern.cpp` | **直接提取**：4/1090/E_xor_find_pattern.cpp 枚举所有数对。 | 保留双循环，只提函数并扩 ull 值域；原题规模允许二次算法。 | max_xor_pair_quadratic 返回数对最大 XOR；与 Trie 共用对照输入。 |
| X09 我的 XOR-gun：最高位分类与前缀枚举<br>`template/competition/xor/xor_gun.hpp` | `cf/date/5.15/3.cpp` | **小优化提取**：date/5.15/3.cpp 两相邻段 XOR 比较、n>60 阈值与操作代价。 | 保留正确原算法；只用前缀 XOR 替换每次扫段 ca，真实复杂度由 O(n^4) 降 O(n^3)。正数、非降序、上界前提不删除。 | xor_gun 返回答案；原 main I/O 分离。 |
| X10 我的 AND 排列：补码块与同最高位环<br>`template/competition/xor/and_permutations.hpp` | `cf/date/7/7.9/2_and.cpp` | **直接提取**：2_and.cpp 按位分块构造零 AND/非零 AND 无不动点排列。 | 保留原两段构造，合并 6/7 基础环的等价初始化；补双射与存在性证明。 | 两个 bool 构造函数及输出排列；原程序输出形式改为返回状态。 |
| X11 我的 Energy Intervals 补全（2268C / 2269E）<br>`template/competition/xor/energy_intervals.hpp` | `cf/1/1124/C.cpp` | **新写完整解**：1/1124/C.cpp 仅有前缀 XOR 与维护最大值的注释，solve 为空。 | 本轮新增按位可行性、最大笛卡尔树与小并大前缀频次；这是补完实现，不是最小修原算法。 | energy_max 与 energy_check 对应已核对题意的完整接口；一般前缀工具另在 X01。 |

## 共享实现与原文件名解析

68 个 ROUND02 条目引用 67 个不同入口源码：MEX01/MEX02 共用 `competition/game/mex.hpp`；IO01/IO02 直接指向已有 `read.cpp`、`core/main.cpp`，所以只有 65 个新入口源码路径。此数不包括规范辅助 `.inc`、独立示例或测试快照，不代表所有新增文件数。

PSD02–PSD09 的 8 个 `.hpp` 是具体节点与公共骨架的组合入口。规范递归仅在 `template/competition/data_structure/segment_point_base.inc` 保持一份；节点分别为 `nodes/sum.inc`、`min.inc`、`max.inc`、`gcd.inc`、`max_count.inc`、`max_subarray.inc`、`frequency.inc`、`stats.inc`（均位于同一 data_structure 目录）。公共骨架保留原树递归，节点中新增的统计量/合并逻辑已逐项标明；打印展开不会创造另一份手工维护算法。

用户提到的 `template/Segment tree.cpp` 在仓库不存在，实际单点树是 `template/segmen tree1.cpp`，区间加是 `template/Segment tree2.cpp`。`cf/date/6/6.15/1_segment_tree.cpp` 原样保留：普通模 2×2 矩阵乘法，mat/rev 与翻转标记；列向量先左后右使父矩阵为 right*left。它只用作非交换合并的来源讲解，不算新增通用矩阵模块，也不是 max-plus。

## 四份专题来源记录的合并结果

| 来源记录 | 记录数 | 整合方式与证据边界 |
|---|---:|---|
| [dp_sources.json](notes/round02/dp_sources.json) | 19 | 14 份 cf + 5 份旧模板，记录局部模型、状态、行数、LF 哈希与代码引用；保留题意未核验的边界，不把练习背景说成完整题解。记录含 12482 检查、原数位/方向/Jakarta 同 oracle 与 15 份卡片驱动验证。 |
| [graph_math_sources.json](notes/round02/graph_math_sources.json) | 24 | 16 份 cf + 8 份旧模板；原代码经验与 UNKNOWN 线上状态分开。图接口适配来自既有正式版时明确写 ROUND01；其余新算法不套用“接口无变化”的笼统说法。 |
| [segment_sources.json](notes/round02/segment_sources.json) | 7 | 记录旧源精确问题、公共实现、8 个节点组合及 13 个示例；同输入对照 DS01/DS02/DS03。PSD11 已明确 ll query 与新 set/add 标签，不混用 PSD10 的 Node 接口。 |
| [xor_sources.json](notes/round02/xor_sources.json) | 6 | 记录题面身份、原快照、原逻辑/证明缺口、真实操作 oracle；题面 CONFIRMED_OFFICIAL 不等于在线 AC。X01/X11 对应空 solve 的新增代码，其余保留正确原核心。 |

合计 56 条专题记录、55 个不同来源路径。下列逐条摘录仅汇总各记录已经写明的局部模型与复用用途；专题作者的测试声明沿用原证据范围，来源审计没有独立复验所有原题。

### dp_sources.json

| 原始路径 | 原思路/局部模型 | 复用结论或用途 |
|---|---|---|
| `cf/date/8/1/2_digit_dp.cpp` | 统计整数区间内非零十进制数位不超过3的数，原calc把0计入。 | 最小提取PDP04；全局s/dp与递归lambda保留。 |
| `cf/ICPC/JAKARTA_2018/J.cpp` | 每个字符串选非空子序列字符串，选出的字符串严格字典序递增，最大化总长度。 | 保留get排序去重与双指针转移，提取PDP03；对象索引改1-indexed。 |
| `cf/date/7/7.9/1_interesting_dp.cpp` | 从s任意位置开始读t，先向右、后向左，至多一次转向。 | 原状态与转移直接保留为PDP03独立函数。 |
| `cf/date/6/6.9/3_leaner_dp.cpp` | 从代码可确认：最大化连续段元素和减ceil(长度/m)*k，且允许空段得0。 | DPC01由此讲解普通最大子段为何可只存末尾；有收费时不得删余数。 |
| `cf/date/6/6.7/1_leaner_value_dp.cpp` | 已出现正整数数值由小值向其倍数传播链长度；重复输入只标记同一数值。 | DPC09作为隐式DAG实例，不能把其解释成保持原数组顺序的LIS。 |
| `cf/3/1096/H_!!dp_tree.cpp` | ct统计子树叶子，mx按儿子ct奇偶给边+1/-1并保存向下最大增益；根叶子另行调整。 | DPC08讲子树对父亲只需小量信息；不将该特定叶子公式冒充最大独立集。 |
| `cf/2/1057/D.cpp` | 局部d(l,r)把区间切成长度2/3的段，代价分别为两数差绝对值与三数极差。 | DPC01提供另一个前缀划分练习；外层环形枚举依赖题意，未作完整正确性声明。 |
| `cf/2/1078/E.cpp` | dpS由左上到格点的最大含点权路径，dpT由格点到右下，均只右下移动。 | DPC02只引用这两个局部状态，不验证后面的规避路径/改点外层结论。 |
| `cf/date/5/5.22/2.cpp` | solve(x1,y1,x2,y2)在障碍网格计数右下路径；外层是四个端点组合的乘积差。 | 提取单路径计数至PDP01；外层组合恒等式未核验题意，不收作本卡整题解释。 |
| `cf/date/5/5.21/2.cpp` | 按两块合并四种(u,l)布尔状态，并枚举比赛结果分支增加局部分数。 | 保留来源作为有限状态边界信息的练习，不推广题目特定转移；原注释含CF1310/B但本轮未联网核验。 |
| `cf/luogu/3.cpp` | 按天处理物品，以幸福值为维度保存最小花费，并限制支出不超过此前已获得的收入。 | DPC03说明容量大、价值和小时交换维度；资金到达时刻是额外前缀可行条件。 |
| `cf/luogu/4.cpp` | 保留左右端点，枚举中间k，合并费用含a[l]*a[k]*a[r]。 | DPC07对比端点型区间与石子合并；不把费用项混用，不猜原题名称。 |
| `cf/luogu/5.cpp` | mask表示当前保留位置，从选中序列的每个相同字符连续段只尝试一个删除位置。 | DPC10/DPC14借此提醒集合语义与计数去重；完整对象计数题意未确认。 |
| `cf/other/Bayan 2015 Contest Warm Up/C.cpp` | 局部check按候选矩形大小，只有对应矩形全X的左上角才可走，布尔dp由上或左到达。 | DPC02作为网格判定的局部拓展；外层面积差候选和最优结论未核验。 |
| `template/01bag.cpp` | 1-indexed全局w/v/dp，容量倒序，求容量不超过m时最大价值。 | 旧转移保留，正式DP01直接复用；PDP02补数量与分组。 |
| `template/完全背包dp.cpp` | 容量正序，允许当前种物品继续使用。 | 直接引用正式DP01，不重新实现。 |
| `template/区间dp.cpp` | 按长度、左端点、分割点求最小合并费用。 | 直接引用修复过数值范围的DP04；原稿保持不动。 |
| `template/线性dp.cpp` | 以末尾i定义最长严格递增子序列。 | 直接引用DP02平方版及已验证二分版。 |
| `template/tree_dp.cpp` | 先递归儿子再把dp[v]加入dp[u]；未定义输入或具体目标。 | 保留原文件；PDP05新增具体最大权独立集状态与可运行驱动。 |

### graph_math_sources.json

| 原始路径 | 原思路/局部模型 | 复用结论或用途 |
|---|---|---|
| `template/tree_storage.cpp` | vector<ll> g[200010]；读n-1条边、两次push。 | PG01直接收录骨架；补多测和根化。 |
| `template/DSU.cpp` | fa/rk/init/find/unite/same；按秩合并实现正确。 | PG02保留；只修注释方向。 |
| `template/bfs.cpp` | queue；入队标记。 | PG03/07/08思想来源。 |
| `template/dfs.cpp` | visited；逐邻边递归。 | PG04仅深链栈替换。 |
| `template/gcd.cpp` | return b?gcd(b,a%b):a。 | M01已有健壮版，PM01从递归扩系数。 |
| `template/ksm.cpp` | 逐位平方；注释写费马逆元。 | M02直接引用；PM01补复合模。 |
| `template/Mobius_inversion.cpp` | init_mu可靠线性筛骨架；另get_mu存在覆盖合数mu的赋值。 | PM02只提取init_mu循环，不修改旧源。 |
| `template/graph_paint.cpp` | col=3-col[u]；只处理一个源且无冲突分支。 | PG11补组件枚举和冲突。 |
| `cf/xian/B.cpp` | 对2n位置成对合并，栈中用same判断。 | DSU段可直接收录；不对题内栈末尾两元素逻辑另作猜测。 |
| `cf/3/1109/3.cpp` | i与i+x/i+y存在时连接；检查i,p[i]组件。 | PG02例子；有限区间不能盲改gcd模环。 |
| `cf/3/1096/H_!!dp_tree.cpp` | g.clear；dfs(u,fa)；ct子树叶计数，mx自底向上。 | PG01保留存储/父边/后序；题内ct奇偶与mx不泛化。 |
| `cf/4/1090/F_tree_constrcution.cpp` | 用一段链和一个星形连接剩余点。 | 链、星形可作为直径/树统计局部练习。 |
| `cf/date/6/6.16/2_bi_paint_.cpp` | 输入配对边与相邻配对边；枚举全部组件染色。 | PG11保留结构；补通用同色边检测。 |
| `cf/2/873/B.cpp` | ans=gcd(ans,abs(a[i]-i))。 | CM07差与共同整除模型，不虚构原题完整证明。 |
| `cf/date/7/7.28/1.cpp` | 排序后sum2+=a[i]*(2*i-n+1)，最后gcd约分。 | PM04直接保留成对差系数，增加宽中间值。 |
| `cf/date/6/6.1/3.cpp` | b[i]=abs(a[i]-a[i-1])，ST表查询b[l+1..r]。 | CM07同余转差分；l=r空差特例。 |
| `cf/2/750/A.cpp` | sum=a+2*b+3*c，输出sum%2。 | 只引用总和模2局部，充分性依原题。 |
| `cf/3/1096/C.cpp` | 按6倍数、仅2倍数、仅3倍数、两者均非分类。 | CM09互斥分类与证明需求。 |
| `cf/date/6/6.8/1.cpp` | 排序求总和，偶总和时删最小奇数。 | 非负值条件下最大奇和的局部练习；不把负数范围猜入原题。 |
| `cf/3/1103/4_game_theory.cpp` | ct[i]奇偶，lose集合中查后继。 | CM09只提取计数奇偶与压缩状态，未复述未核验博弈规则。 |
| `cf/2/1129/C.cpp` | ct0/ct1统计并判断几个特例。 | CM09提醒需原题规则才能证明这些特例。 |
| `cf/ICPC/IC_14_xian/K.cpp` | ans+=a/b，随后(a,b)=(b,a%b)，单独处理b=0。 | 补充欧几里得每步商的含义，不新建重复gcd。 |
| `cf/ICPC/JAKARTA_2018/H.cpp` | 局部len=r-l+1，need=(c+len+1)/2；整体solve未完成。 | PM03只提取取整需求，明确不是完成题解。 |
| `cf/1704E.cpp` | 模拟之后Kahn生成tp，逆序沿出边累加dp。 | PG15抽拓扑递推；不复制题内模拟/汇点选择。 |

### segment_sources.json

| 原始路径 | 原思路/局部模型 | 复用结论或用途 |
|---|---|---|
| `template/Fenwick_tree.cpp` | 直接保留Fenwick结构、n、tr、add和lowbit循环。 | 只统一接口：sum(x)改名query(x)，旧query(l,r)改名rangeSum(l,r)；size参数避免-Wshadow；加合法调用断言。 |
| `template/segmen tree1.cpp` | 全局a/tree、1下标和pushup/build/递归单点修改结构直接继承。 | 保留单点递归骨架并将update改名modify；PSD02保持sum，PSD03–09分别替换具体Node/merge，阈值搜索、gcd、次数、最大子段和、第k个等为本轮在骨架上的扩展。查询只进入相交儿子。 |
| `cf/segment_tree_cf_edu/1.cpp` | 沿用用户的node/l/r/mid、左右儿子编号和单点赋值递归。 | 输入0下标[l,r)的转换只保留在来源说明，正式接口统一用户熟悉的1下标闭区间。 |
| `cf/segment_tree_cf_edu/2.cpp` | 直接继承min合并；来源为用户的区间最小值练习。 | 为共用无空节点的Node骨架，查询只进入相交儿子，不再依赖有限INF；不否定旧代码在原约束下的使用验证。 |
| `template/Segment tree2.cpp` | 全局a/tree/lazy；apply、pushdown、modify按原递归结构保留。 | 公共修复为apply声明顺序、ll和/答案、查询下传、build清标记。PSD10扩展Node{sum,mn,mx}；PSD11保留ll区间和，把lazy扩展为Tag{has_set,set,add}，新set覆盖旧标记，下传先set再add；test_set_add验证赋值0、操作先后、多测和随机差分。 |
| `cf/date/6/6.9/1_special_segment_tree.cpp` | 直接继承区间加、维护最小值的apply/pushdown/build/modify写法。 | PSD10沿用区间加与min维护并加入sum/max。PSD11只沿用加法递归和build清标记写法，聚合为ll区间和；has_set与set/add组合是本轮新增，原题mn/pos不搬入。 |
| `cf/date/6/6.15/1_segment_tree.cpp` | 读取完整原代码；Node保存mat和A/B互换后的rev，区间翻转swap二者、lazy异或；列向量先左后右使父矩阵为right*left。 | 原代码保持原样。仅作为多信息维护与非交换merge的来源讲解，不冒充本轮新增通用矩阵模板。 |

### xor_sources.json

| 原始路径 | 原思路/局部模型 | 复用结论或用途 |
|---|---|---|
| `cf/1/1124/B.cpp` | “这些数字二进制 都是偶数个1 所以popcount 奇偶性不变”；“对于中间都操作x 实现头尾x”。 | good(int)与更新计数逻辑直接收录，没有算法修复。仅补可达性证明。 |
| `cf/1/1124/C.cpp` | “先化简 异或是有结合率的 先全部异或 l r pr p(l - 1) 然后维护最大值”。 | 补全按位可行性检查、最大笛卡尔树与小并大前缀频次；新增P2完整解。另把一般前缀模型抽成X01。 |
| `cf/4/1090/E_xor_find_pattern.cpp` | “异或有交换律和结合律”“每次之和最后一次的xor有关”。 | 双循环原算法直接收录；sum n<=3105，所以不能判TLE。Trie只作为更大规模扩展。 |
| `cf/date/5.15/3.cpp` | “两个连续区间，左边异或大于右边”“三个最高位都相同，直接结束”“61个数”。 | 原解合法域正确且60封顶可用；仅用前缀替换ca循环，保留阈值、分割枚举与成本公式。 |
| `cf/date/7/7.9/2_and.cpp` | “分块处理 和按位处理是位运算很重要的感觉”。 | 两段原构造直接抽函数；只合并6/7基础环的等价初始化。 |
| `cf/2/1129/B.cpp` | “从两个我们去操作一下”“最大可以变多少”“取的是或”。 | 保留mx^mn生成低位全1mask的循环，抽成interval_or；完整题意推导在个人复盘中。 |

## XOR 原题身份与原代码评价

以下身份与规模来自 xor_sources.json 中的官方题面核对记录，来源审计未重新联网核验。所有在线提交 verdict 仍为 UNKNOWN。

| 原文件 | 记录中的题目 | 对复用有影响的结论 |
|---|---|---|
| `cf/1/1124/B.cpp` | CF2268B | 原 good(int) 在 0..15 域正确；补的是充分性证明，不能称原算法修复。 |
| `cf/1/1124/C.cpp` | CF2268C / CF2269E | 表达式为区间 XOR 与区间最大值按位 AND，且 l<r、ai<2^18；原 solve 为空。 |
| `cf/4/1090/E_xor_find_pattern.cpp` | CF2218E | 总 n<=3105，原 O(n²) 枚举允许；Trie 是规模扩展。 |
| `cf/date/5.15/3.cpp` | CF1457D | 正数、非降序、ai<=1e9 支撑 n>60 阈值；原可用，优化扫描后复杂度变化另记。 |
| `cf/date/7/7.9/2_and.cpp` | CF909F | 两个排列目标还要求无不动点；原构造完整，补证明并提函数。 |
| `cf/2/1129/B.cpp` | CF2261B | 目录 1129 不能当比赛编号；原 mx^mn 掩码结论保留，并补相邻操作可达性证明。 |

## 本次来源核验结果与元数据边界

- 32 + 68 个模块 ID 唯一，source、test、origins 路径均存在，dependencies/replaces 均能解析。没有缺失正式模块元数据或失效来源路径；BM07 自指、IO01/IO02 原样自引用、PDP06 空 origins 已按实际角色解释。
- 56 条专题记录中可用的 49 个来源哈希均与当前原文件匹配（按各记录规定使用字节或 LF 标准化）；DP 验证记录的 7 个代码/测试哈希亦匹配。两个完整程序副本 BM05/BM06 与来源 LF 全文相同。
- 基线 32 旧源 + 32 正式源、4 份原打印成品重新比较 SHA-256 均保持一致。逐文件值已存在 provenance_baseline.json，本表不重复抄写哈希。
- PSD11 曾复用 PSD10 的 Node 描述，现元数据与 segment_sources.json 已修正为 ll 求和及 Tag{has_set,set,add}。没有剩余的这项接口冲突。
- origins 是模块级来源摘要，不能覆盖同文件所有新增函数。PDP03 的 LCS/编辑距离、PDP04 的二进制计数、X03 的前三个 XOR 工具、PM04 的三个基本求和函数在本表明确标为新写。PSD04–PSD09 的新 Node/搜索与旧递归骨架也已拆开说明。
- BM01–BM06 与 IO03 均已纳入实际执行的 `scripts/run_round02_tests.py` 统一 release 和 ASan/UBSan 测试，两套报告均为 complete / PASS；此前待整合状态已由实测替代。两套均覆盖原 32 模块回归与本轮 68 个目录条目，共 100 个模块、16 个独立测试套件，每套 188 条编译/运行记录。


## 最终统一验证证据

来源审计读取以下已完成报告，并重新核对所有 100 个模块的展开代码 SHA-256。每份报告的 119 个源码映射、16 个测试源映射、132 个测试 include 依赖映射均与当前 LF 标准化内容相符；这些映射存在重叠，不相加为不同文件总数。全部记录退出码为 0。这里报告已执行测试的证据，没有把来源审计描述为测试执行者。

| 报告 | 实际编译配置与状态 | 报告文件 SHA-256 |
|---|---|---|
| [release.json](notes/round02/validation/release.json) | Windows g++ 15.2，GNU++20 -O2 -Wall -Wextra -Wshadow；complete / PASS | `52644e6c1990cf7d5247058bb3e4855c0d3ce1f5da97e6093fdc2b25e592efae` |
| [sanitizers.json](notes/round02/validation/sanitizers.json) | WSL Ubuntu g++ 13.3，GNU++20 ASan+UBSan；complete / PASS | `0d0d6fc5b63dec1b93243f76dc2eb3e6b54f060440e70de0cf5831fbac14d59a` |

本审计仅新增本来源说明；未修改任何旧算法、新规范算法、打印脚本、既有成品，也未提交或推送。
