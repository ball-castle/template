# ROUND ACM-02 / 02A / 02B 基线覆盖审计

审计始于2026-10-24，2026-10-25完成个人源码补充记录。本文冻结本轮开始时的事实，不把并行新增但尚未实测的内容记作完成。

- template 基线：`c0792d4a09e34c636eaff03fce69e9b83d8c10a4`。
- cf 基线：`d8d0b4ed1b58c54540e45c26f0a872e309a9edcf`。
- 本轮重新完整读取 32 份正式源码、根目录 32 份旧源码、`notes/thinking_cards.md` 的 6 张卡；不是只根据文件名判断覆盖。
- 32 份正式源码 SHA-256 均与 `notes/validation/release.json`、`sanitizers.json` 的记录一致；两份既有验证记录均为 PASS。本审计复核证据绑定，没有把它说成本轮重新执行全部测试。
- 第一轮打印结构记录为急用版 24 页、658 行源码，完整版 41 页、1271 行源码；对应四份原 DOCX/PDF 保留。原记录不代表本轮新增主题已入稿。
- 用户说明自己的原代码经过验证使用。此项记为 `USER_REPORTED_USED`；线上提交状态未经查询仍为 `UNKNOWN`。两者互不否定，不能自动改写为 AC。

## 状态口径

| 状态 | 本文含义 |
|---|---|
| VERIFIED_CODE | 明确输入契约下已有正式可复用代码，且有与当前源码绑定的本地验证记录；不等于覆盖所有同名变体或线上 AC。 |
| MODEL_ONLY | 有可定位的性质、注释或模型卡，但没有对应完整规范代码。 |
| PARTIAL | 只有部分变体、题目特化片段或尚需补齐的接口/证明，不能冒充整个专题完成。 |
| LEGACY_ONLY | 旧文件已有具体代码，尚未进入正式主目录；代码是否可直接收录另列具体条件。 |
| MISSING | 基线正式源码、旧模板与卡片没有对应可交付内容；个人训练文件的候选不自动成为正式覆盖。 |
| OPTIONAL | 低频或超出稳定铜牌主线的扩展，保留已有成果，不占赛前补丁主线。 |

最新用户纠正优先于旧规格：`read.cpp` 的 `inline ll read()` 原样收录，保留 `ll/getchar()` 与接口；合法常规整数输入使用它，其他输入用 `cin`。不以 EOF、超出原用途的极值/字符串需求为由重构。旧代码先用，只有确切问题才最小修改；树存储也必须进入基础材料。

## 32 个正式模块逐项复核

表中状态仅针对“实际已有能力”这一列。全部 `online_status=UNKNOWN`。

| ID | 规范源码 | 状态 | 实际已有能力及不可外推的边界 |
|---|---|---|---|
| CO01 | `core/main.cpp` | VERIFIED_CODE | 单/多测主骨架；没有旧快读。 |
| CO02 | `core/basics.hpp` | VERIFIED_CODE | 一维前缀和、离线差分、离散化、i128 转字符串；没有二维版或最少区间操作模型。 |
| CO03 | `core/search_window.hpp` | VERIFIED_CODE | 整数 first/last true、排序两数和、非负和窗口；没有实数二分/三分、负数窗口通解。 |
| DP01 | `dp/knapsack.hpp` | VERIFIED_CODE | 01/完全背包，恰好/至多容量；没有多重/分组背包。 |
| DP02 | `dp/lis.hpp` | VERIFIED_CODE | LIS 长度的平方/二分算法及严格/非严格区别；没有 LCS、编辑距离或答案恢复。 |
| DP03 | `dp/hamilton_path.hpp` | VERIFIED_CODE | n≤18 的有向 Hamilton 路状压 DP；不是所有状压题通解。 |
| DP04 | `dp/interval_merge.hpp` | VERIFIED_CODE | 非负直线石子合并；不是环形区间 DP。 |
| DS01 | `data_structure/fenwick.hpp` | VERIFIED_CODE | 单点加、前缀和、区间和；没有第 k 个、区间加变体。 |
| DS02 | `data_structure/segment_tree.hpp` | VERIFIED_CODE | 单点赋值、区间和；没有 min/max/gcd/max-count/最大子段和。 |
| DS03 | `data_structure/lazy_segment_tree.hpp` | VERIFIED_CODE | 区间加、区间和；没有区间赋值、混合标记或值域第 k 个。 |
| DS04 | `data_structure/monotonic.hpp` | VERIFIED_CODE | 左侧严格更小位置、定长窗口最小值；没有所有单调结构题型的推导。 |
| DS05 | `data_structure/sparse_table.hpp` | VERIFIED_CODE | 结合且幂等的静态查询，已有 min/gcd 运算；不能用重叠块求和。 |
| G01 | `graph/dsu.hpp` | VERIFIED_CODE | 连通、合并、块大小、块数；接口与原全局 `fa/rk` 不同。 |
| G02 | `graph/bfs_path.hpp` | VERIFIED_CODE | 单源无权最短路及路径；没有网格、多源、0-1 BFS。 |
| G03 | `graph/dfs_forest.hpp` | VERIFIED_CODE | 迭代 DFS 全图连通分量、父亲、发现序；不是递归树模板或有向环恢复。 |
| G04 | `graph/topological_sort.hpp` | VERIFIED_CODE | Kahn 拓扑排序与有环判定；没有完整 DAG DP。 |
| G05 | `graph/unicyclic_graph.hpp` | VERIFIED_CODE | 连通简单无向基环图的环恢复；一般有向/无向图的环恢复不能算已覆盖。 |
| G06 | `graph/dijkstra.hpp` | VERIFIED_CODE | 非负权单/多源最短路、路径，INF 饱和；不是 Floyd/0-1 BFS/负边算法。 |
| G07 | `graph/kruskal.hpp` | VERIFIED_CODE | MST/最小生成森林，恢复选边；不连通以字段表示。 |
| G08 | `graph/prim.hpp` | VERIFIED_CODE | 堆 Prim 最小生成森林；不是稠密矩阵 Prim。 |
| G09 | `graph/lca.hpp` | VERIFIED_CODE | 树上倍增、跳祖先、距离；没有带权路径聚合。 |
| G10 | `graph/euler_subtree.hpp` | VERIFIED_CODE | Euler 子树区间、子树大小与子树和；只覆盖最简单树 DP。 |
| G11 | `advanced/scc.hpp` | VERIFIED_CODE | 迭代 Kosaraju SCC；保留进阶层，不需重写。 |
| G12 | `advanced/bridges.hpp` | VERIFIED_CODE | 迭代无向桥，考虑平行边；保留进阶层。 |
| M01 | `math/integer_bits.hpp` | VERIFIED_CODE | gcd/lcm、lowbit/popcount/最低位、子集枚举说明；不能算 XOR 模型完整。 |
| M02 | `math/modular.hpp` | VERIFIED_CODE | 规范取模、模加乘幂；费马逆元仅注释适用条件，没有 exgcd。 |
| M03 | `math/matrix_power.hpp` | VERIFIED_CODE | 普通模矩阵幂；不能替代 max-plus 转移矩阵。 |
| M04 | `math/product_recurrence.hpp` | VERIFIED_CODE | CF1182E 指数矩阵专例；属于特定模型，不代表组合数学完整。 |
| M05 | `math/combinations.hpp` | VERIFIED_CODE | 已知质数模、n<mod 的阶乘逆元组合数；不检测质数。 |
| S01 | `string/kmp_z.hpp` | VERIFIED_CODE | KMP/Z；不是回文最长子串算法。 |
| S02 | `string/double_hash.hpp` | VERIFIED_CODE | 双模子串哈希，碰撞风险明确；未提供正反串回文专题调用。 |
| S03 | `string/basic_string.hpp` | VERIFIED_CODE | 双指针回文判定、分割、ASCII 小写、连续段；不含回文计数/构造/LPS。 |

## 32 个旧文件逐项复核

原测试证据详见 `TEMPLATE_AUDIT.md` 与 `notes/legacy_manifest.json`。本轮重新核对实际代码，以下判断不覆盖或更改原文件。`VERIFIED_CODE` 表示相应能力已被正式模块覆盖，并非把有问题的旧文件标为通过。

| 旧路径（template/ 下） | 覆盖状态 | 直接复用或具体小问题 |
|---|---|---|
| `01bag.cpp` | VERIFIED_CODE | 全局 `w/v/dp`、倒序容量，DP01 已覆盖；原 int 容量/价值范围须说明。 |
| `bfs.cpp` | VERIFIED_CODE | `g[N]`、独立 bfs、入队即标记可直接作为遍历基础；G02 扩展了距离。 |
| `binary_lifting.cpp` | PARTIAL | 实际是未写完 ST 草稿：`log`/`logn` 混用，末尾 for 未完成；不能作 LCA 来源。 |
| `binary_search_num.cpp` | VERIFIED_CODE | 全局数组二分可直接复用；返回任一匹配，不是 lower_bound，CO03 是另一个答案二分模型。 |
| `data_sturcture.cpp` | LEGACY_ONLY | 有 STL 实际调用；哈希 O(1) 应标平均，`emplace_back` 不保证总更快；技巧大全尚缺。 |
| `dfs.cpp` | VERIFIED_CODE | `g[N]`、visited、独立递归 dfs 可直接收录基础；长链栈风险须注明，G03 提供迭代版。 |
| `DSU.cpp` | VERIFIED_CODE | `init/find/unite/same` 已正确实现按秩合并；仅“挂到秩小”注释与实现相反。个人主版本优先它。 |
| `Fenwick_tree.cpp` | VERIFIED_CODE | 原 struct/vector 就是用户源码；只需将 main 的 `rangeSum` 调用对齐已定义 `query`。不为全局化而重写。 |
| `gcd.cpp` | VERIFIED_CODE | 非负域递归 gcd 可直接复用；M01 扩展了全 ll 域。 |
| `graph_paint.cpp` | PARTIAL | 只写了染色传播，无读边、同色冲突检查和全分量循环；不能当二分图判定。 |
| `interaction.cpp` | OPTIONAL | 只有 include/宏，未实现交互协议；非本轮主线。 |
| `KMP.cpp` | VERIFIED_CODE | 原非空模式串 KMP 可直接复用；S01 补齐空模式与 Z。 |
| `kruskal MST.cpp` | VERIFIED_CODE | 求恰好 k 个分量的最小森林；原 int 和及容量有限，G07 的接口另有差异。 |
| `ksm.cpp` | VERIFIED_CODE | 普通正模/非负指数惯用写法；p=1、b=0 初值及乘法范围需具体说明，M02 已修正。 |
| `lambda.cpp` | LEGACY_ONLY | 递归 self lambda、排序 lambda 可引用，定义未调用不等于算法验证。 |
| `linklist.cpp` | OPTIONAL | 教学链表；`CreateListT` 实为头插，优先 STL 容器；不占铜牌主线。 |
| `lower_bound.cpp` | PARTIAL | 实际降序 upper_bound 固定查 4；须保证降序并判 end，不能按文件名认作升序 lower_bound。 |
| `matrix_ksm.cpp` | VERIFIED_CODE | 原指数矩阵观察值得保留；最终乘积初值 `ans=0` 使结果恒 0，M04 已修复。 |
| `mex.cpp` | MODEL_ONLY | 原注释已有静态 vis、动态零频 set 的完整方向；`uisng` 拼写错误且没有实现。 |
| `Mobius_inversion.cpp` | OPTIONAL | 主程序和备用 `get_mu` 要分开：备用函数无条件覆盖 mu[x]；反演进阶不阻塞基本筛法。 |
| `prim MST.cpp` | VERIFIED_CODE | 链式前向星+小根堆是用户真实习惯；原 int 总权与单测状态有限，G08 扩展为森林。 |
| `read.cpp` | LEGACY_ONLY | 原 6–11 行函数按最新用户要求原样采用。保证有下一个合法整数、绝对值累积可放 ll；特殊输入用 cin。 |
| `segmen tree1.cpp` | VERIFIED_CODE | 全局 a/tree 与 pushup/build/update/add/query 可直接复用，是个人版优先来源。 |
| `Segment tree2.cpp` | PARTIAL | `apply` 声明顺序；query/update 下行前未 pushdown；tree/query 是 int。逐点小修即可，不需换成陌生类。 |
| `sort.cpp` | LEGACY_ONLY | 有 cmp、lambda、struct 排序及“参数是值不是下标”注释；适合 STL 卡片。 |
| `ST表.cpp` | VERIFIED_CODE | 差分 gcd 静态查询完整，`[l,r]→[l+1,r]` 很有教学价值；DS05 已有通用静态表。 |
| `Ternary_Search.cpp` | LEGACY_ONLY | 有固定迭代次数的三分骨架；当前注释题链与公式题意未确认，单峰前提不能省略，也不能猜原题。 |
| `tree_dp.cpp` | PARTIAL | 子后父合并骨架可直接收录讲解；没有读边/初始化 dp，不能当完整树 DP 解题程序。 |
| `tree_storage.cpp` | LEGACY_ONLY | 6、15–19 行双向 vector 邻接表完整，应直接收录基础，不因短小或无查询而漏掉。 |
| `区间dp.cpp` | VERIFIED_CODE | 长度→左端点→分割点、对角线初值；DP04 已扩 ll 并明确非负直线契约。 |
| `完全背包dp.cpp` | VERIFIED_CODE | 正序容量与原中文原因注释可直接复用，DP01 已覆盖。 |
| `线性dp.cpp` | VERIFIED_CODE | `dp[i]` 以 i 结尾、初值 1，DP02 已覆盖。 |

## 6 张已有思维卡

| 卡片 | 状态 | 已有内容 | 仍缺的交付 |
|---|---|---|---|
| 构造：先满足被迫位置 | MODEL_ONLY | 回文传播、剩余奇偶与独立判定器，指向 `cf/construction/1.cpp` | 统一的信号→暴力→证明→可运行核心代码→反例；更多分组/两端/模构造。 |
| 操作题：不变量与可达性 | MODEL_ONLY | 总和/奇偶/gcd/连通不变量，Euclid 批处理，指向 `cf/xian/B.cpp` | 不变量充分性证明、区间单位操作最优公式及实际构造。 |
| 贪心：一次交换 | MODEL_ONLY | 交换论证与固定锚点分段 | 区间选择、堆维护、反向/两端贪心的独立适用条件和代码。 |
| 差分与区间 | PARTIAL | 边界差分、差分 gcd、CF EDU 半开转闭区间；有 CO02/DS05 | 二维、前后缀操作、正负单位操作、翻转及总变差。 |
| 图论：先定义边 | PARTIAL | BFS/Dijkstra/DSU/Kruskal 信号，有 G01–G10 | 网格、多源 BFS、0-1 BFS、染色、一般环及新模型证明。 |
| DP：语义、可达性、转移 | PARTIAL | 状态定义、前缀最优优化、背包遍历方向；有 DP01–04 | 完整 15 类 DP 状态表、逐类初值/答案/顺序/例题与代码。 |

## 新要求的逐主题差距

以下为本轮开始时的状态；`MISSING` 不表示用户从没写过相关练习，只表示没有已交付的完整正式覆盖。训练源码候选单独进入 `notes/round02/source_inventory.json`。

| 主题 | 状态 | 现有证据与需补齐范围 |
|---|---|---|
| 关闭同步/cin.tie、旧整数 read | PARTIAL | CO01 与旧 read 各自可用；主打印尚未收录 read 原函数、混用约定及适用域。 |
| i128 输出与整数溢出 | PARTIAL | CO02 有转字符串，M01/M02 有防溢出；i128 输入和小技巧调用未整合。 |
| 精度、舍入、反三角、方向角 | MISSING | 没有完整 fixed/precision/EPS/atan2/acos clamp 的规范模块与例子。 |
| 实数二分、实数三分 | LEGACY_ONLY | Ternary_Search 有特化三分；需要独立单调/单峰契约和通用调用。 |
| STL 技巧大全 | PARTIAL | 旧 data_sturcture/lambda/sort/lower_bound 分散；单个 multiset 删除、迭代器失效、负除法等未闭合。 |
| 一维前缀和/差分/离散化 | VERIFIED_CODE | CO02；需增模型推导，不重写正确代码。 |
| 二维前缀和/二维差分 | MISSING | CO02 仅一维。 |
| 非负全零→目标，任意区间 +1 最少次数 | MISSING | 没有正差分下界、达成构造与反例。 |
| 任意整数目标、区间 ±1 最少次数 | MISSING | 不能沿用只允许 +1 的公式；须独立证明。 |
| 区间 -1、前缀/后缀操作、区间加任意数 | MISSING | 须按允许操作和目标域分别分析。 |
| 区间翻转、相邻差分、指定位置增量、不变量 | MODEL_ONLY | 旧操作卡仅框架；缺具体必要/充分条件。 |
| 奇偶、对称、两端、分组、排列、排序、取模、循环移位构造 | MODEL_ONLY | 仅回文卡覆盖一角；必须逐类证明合法及不可行。 |
| 区间选择/最早结束/最小代价/反向/两端/堆/重排贪心 | MODEL_ONLY | 只有交换论证框架，没有逐类完整实现与反例。 |
| 整数答案二分/最小最大值 | PARTIAL | CO03 代码可靠；check 设计、单调性与应用模型不完整。 |
| 双指针与窗口 | PARTIAL | CO03/DS04 覆盖排序两数和、非负和、定长最小；不应声称所有窗口。 |
| 逆序对 | MISSING | Fenwick 可当工具，未给逆序对算法与离散化调用。 |
| 贡献法、枚举端点/中间点、前后缀合并 | MISSING | 没有规范模型卡与经测代码。 |
| 括号匹配、循环节与周期 | PARTIAL | 训练卡提及栈，KMP 可作工具；未形成完整模型。 |
| 全排列/子集/回溯/剪枝/折半/状态 BFS/记忆化 | PARTIAL | M01 仅子集枚举，DP03 仅集合 DP；其他搜索代码与模型缺口独立存在。 |
| Nim、SG、mex、独立游戏、必胜策略、反常规则 | MISSING | 原 6 卡无完整博弈专题；不能从位运算模块推断覆盖。 |
| 静态/动态/前缀/区间/构造 MEX | MODEL_ONLY | 旧 mex 只有 vis/set 思路；没有正式可复用实现。 |
| 双指针回文判定 | VERIFIED_CODE | S03，空区间与奇偶长度已覆盖。 |
| 回文数、排列可行性、回文构造、中心扩展与计数 | PARTIAL | 仅构造卡与 S03 判定；数字前导零、计数/最长子串未覆盖。 |
| 回文子序列/最长回文子串/反向哈希调用 | MISSING | S01/S02 不能代替对应模型；Manacher 可放长期次优先。 |
| 线性 DP / LIS | VERIFIED_CODE | DP02；需要识别与状态设计教学补全。 |
| 网格 DP | MISSING | 无正式网格路径计数/最小代价。 |
| 01/完全背包 | VERIFIED_CODE | DP01；保留原循环与中文说明。 |
| 多重/分组背包 | MISSING | 原 DP01 不能代表它们。 |
| 区间 DP | PARTIAL | DP04 只直线石子合并，缺状态选择与其他常见例子。 |
| 树形 DP | PARTIAL | G10 只子树和；旧 tree_dp 为合并骨架。 |
| DAG DP | MISSING | G04 有拓扑工具，但没有完整 DP 转移。 |
| 状压 DP | PARTIAL | DP03 单一 Hamilton 模型，缺集合状态识别。 |
| 数位 DP | MISSING | 正式缺失；`cf/date/8/1/2_digit_dp.cpp` 是优先个人候选。 |
| LCS、编辑距离、状态机 DP、计数 DP | MISSING | 不能以 LIS 存在替代。 |
| 概率/期望 DP | MISSING | 需要有限简单状态例子；不扩大到高级优化。 |
| 单点改：和 | VERIFIED_CODE | DS02 与旧 segmen tree1。 |
| 单点改：min/max/gcd | MISSING | ST 静态查询不能冒充动态线段树。 |
| 区间加：和 | VERIFIED_CODE | DS03，原 Segment tree2 可按具体缺陷最小修补。 |
| 区间加：min/max | MISSING | 和的 lazy apply 不能原样用在极值上。 |
| 区间赋值：和；赋值+加混合 | MISSING | 需要独立 has_set、赋值 0、先后复合测试。 |
| 极值及出现次数、最大连续子段和 | MISSING | 需要专门 Node 与结合律合并，不只是替换加号。 |
| 频率第 k 个、首个阈值位置 | MISSING | 需非负计数/节点剪枝条件和越界返回值。 |
| 多个统计量、矩阵信息合并 | LEGACY_ONLY | `cf/date/6/6.15/1_segment_tree.cpp` 是普通模矩阵乘法、A/B 翻转并维护原/反两套矩阵的候选，未成正式条目；不是 max-plus。 |
| Fenwick/ST/单调队列/线段树选择表 | MISSING | 工具均有，比较导航尚无独立页。 |
| 图 BFS/DFS/连通块 | VERIFIED_CODE | G02/G03，另保留原全局 g 基础写法。 |
| 树存储/遍历/子树大小 | PARTIAL | 旧 tree_storage 可直接收录；G10 有大小，主版基础教学不能缺。 |
| 网格 BFS、多源 BFS、0-1 BFS | MISSING | G02 的单源普通图 BFS 不算覆盖。 |
| Dijkstra、Kruskal、Prim、LCA | VERIFIED_CODE | G06–G09；个人接口优化不必推翻正确算法。 |
| Floyd | MISSING | 测试 oracle 中使用过不等于正式收录。 |
| 拓扑 | VERIFIED_CODE | G04；DAG DP 另计。 |
| 二分图染色 | PARTIAL | 旧 graph_paint 草稿缺读边和冲突检测。 |
| 一般图环检测/恢复 | PARTIAL | G05 只简单连通基环图。 |
| 树直径 | MISSING | 现有 BFS 可做基础，但无正式两遍搜索调用。 |
| SCC/桥基本识别 | VERIFIED_CODE | G11/G12 已可靠存在，保持长期进阶层。 |
| gcd/lcm/快速幂/质数模组合数 | VERIFIED_CODE | M01/M02/M05。 |
| exgcd/同余与可达性/整数上下取整 | MISSING | 负数除法尤其不能仅加 b-1。 |
| 质数判定/筛/分解/约数 | PARTIAL | 旧 Mobius 主程序包含线性筛，但不是独立基础模块。 |
| 常用求和、计数、奇偶与模不变量 | MODEL_ONLY | 旧操作卡仅方向，缺独立闭合例子。 |
| 二维向量/点积叉积/方向/线段相交 | MISSING | 三分几何表达式不等于几何基础模板。 |
| XOR 性质/恢复未知量/最高位/奇偶不变量 | MISSING | M01 只位 API，缺专题模型。 |
| 前缀 XOR、子数组 XOR=k 计数、XOR 差分 | MISSING | 需独立代码与更新前提，训练注释不是实现。 |
| XOR/AND/OR 按位贡献、最高位分类 | MISSING | 需按位计数和不同运算的区别。 |
| XOR 构造/自由位/排列约束 | MISSING | 需限制条件、合法性判定与失败反例。 |
| 最大 XOR 数对/子数组、二进制 Trie | MISSING | 双循环训练候选不能算已满足大数据需求。 |
| 图 XOR 约束/矛盾/连通块自由度 | MISSING | 普通 DSU/DFS 不自动实现带权异或约束。 |
| 线性基 | MISSING | 用户明确长期进阶基础需求；补丁不优先。 |
| FWT、Beats、李超、可持久化、斜率优化、复杂树背包 | OPTIONAL | 无本轮高频需求依据，不占主线。 |
| 小数据暴力、固定种子随机差分 | PARTIAL | 原测试体系已有；缺面向使用者的现场调试与模型验证导航。 |

## 本轮行动边界

先做原 read、精度、STL、博弈/MEX、构造操作、DP/线段树速查与调试补丁。长期版再补齐图、数学、搜索、完整 DP 和 XOR 专题。正确旧源优先直接收录；原 32 正式模块、32 旧源和四份打印成品均保留。最终完成度由新增规范源码、实测证据、模型卡和真实打印页码共同决定，本文的基线状态不能被误读为本轮最终验收。

## 个人训练源码补充：已有候选不应被重新发明

上面的主题差距表考察正式32模块、旧32模板和6卡的交付覆盖。进一步扫描cf全部215个跟踪cpp，并全文深读50份（49份不同内容）后，下列能力在**个人训练侧**已有可用候选，应标为LEGACY_ONLY或PARTIAL，而非声称用户从没写过。它们尚未因此变成正式VERIFIED_CODE：需要按各自题域验证、补齐调用和打印绑定。

| 训练侧能力 | 训练侧状态 | 首选来源与具体边界 |
|---|---|---|
| 单点赋值、区间min | LEGACY_ONLY | `cf/segment_tree_cf_edu/2.cpp:12–59`，与已有和版同骨架；保留大数单位元契约。 |
| 区间加、最小值及左侧位置 | LEGACY_ONLY | `cf/date/6/6.9/1_special_segment_tree.cpp:30–102`，mn/pos/lazy完整；题目删除策略与数据结构分开。 |
| A/B翻转、普通模矩阵合并 | LEGACY_ONLY | `cf/date/6/6.15/1_segment_tree.cpp:7–123`，mat/rev、right×left；不是max-plus。 |
| 数位DP | LEGACY_ONLY | `cf/date/8/1/2_digit_dp.cpp:15–48`，统计非零位≤3，包含0；直接沿用pos/cnt/limit。 |
| 两阶段状态机DP | LEGACY_ONLY | `cf/date/7/7.9/1_interesting_dp.cpp:22–51`，未左转/已左转，滚动数组分层。 |
| 值域/余数/前缀最优DP | LEGACY_ONLY | `cf/date/6/6.7/1_leaner_value_dp.cpp`、`cf/date/6/6.9/3_leaner_dp.cpp`、`cf/ICPC/JAKARTA_2018/J.cpp`，有完整状态压缩和重复转移消除的原注释。 |
| 集合计数DP | PARTIAL | `cf/luogu/5.cpp:20–38` 有明确mask递推；题目价值定义尚未核验，不当成通用Hamilton来源。 |
| 树DP与树存储/遍历 | LEGACY_ONLY | `cf/hdu/B.cpp:19–30`、`cf/3/1096/H_!!dp_tree.cpp:32–64`；保留g[N]、dfs(u,fa)、儿子先合并。 |
| 拓扑+DAG DP | PARTIAL | `cf/1704E.cpp:58–95`有Kahn与逆拓扑DP；单汇点和DAG保证属于题目契约，不能原样泛化。 |
| 二分图染色 | PARTIAL | `cf/date/6/6.16/2_bi_paint_.cpp:36–54`已遍历所有分量；通用化只补同色冲突检查。 |
| 字符串Trie | LEGACY_ONLY | `cf/ICPC/2026ICPCWeb1/L.cpp:5–37`，tr[][26]/cnt/dep/tot；前缀插入可直接提取，题目best/ans分开。 |
| 回文配额构造 | LEGACY_ONLY | `cf/construction/1.cpp:13–64`，先镜像传播、再扣配额、处理中点；优先原代码。 |
| 最长回文前/后缀与首尾拼接 | LEGACY_ONLY | `cf/date/6/6.14/1.cpp:4–79`，KMP拼反串；不是任意最长回文子串。与前一天kmp文件内容重复。 |
| MEX+中心扩展 | PARTIAL | `cf/3/1096/D_mex_set.cpp:20–42`，set候选删除；依赖两个0等原题条件，不是动态重数MEX。 |
| 答案二分与选最便宜合格项 | LEGACY_ONLY | `cf/other/Hello_2018/D.cpp:5–77`，原中文推导已闭合到check、单调性和恢复。 |
| 贪心+lower_bound | LEGACY_ONLY | `cf/date/7/7.25/1.cpp:17–37`，每步取最小可行保留后续空间。 |
| 组合数/模逆 | LEGACY_ONLY | `cf/date/5/5.19/3.cpp:19–46`，fac/ifac/ksm/ncr/init完整固定质数版。 |
| gcd不变量/贡献计数/Euclid批处理 | LEGACY_ONLY | `cf/2/873/B.cpp`、`cf/date/7/7.28/1.cpp`、`cf/ICPC/IC_14_xian/K.cpp`，只引用可明确陈述的局部模型，不猜整题。 |
| XOR回溯/逆操作 | LEGACY_ONLY | `cf/1/122/B.cpp:13–32`，XOR两次恢复、逆排列恢复状态。 |
| XOR最高位构造、暴力最大数对 | PARTIAL | `cf/date/7/7.9/2_and.cpp`、`cf/4/1090/E_xor_find_pattern.cpp`；后者O(n²)，不是现成Trie。 |
| 前缀XOR、Fenwick选型、容斥 | MODEL_ONLY | `cf/1/1124/C.cpp`、`cf/3/1096/G_Fenwicktree.cpp`、`cf/ICPC/IC_14_xian/3.cpp`有思路但关键实现为空/未完。 |

全部个人来源路径、SHA-256、行号、原注释观察、复用片段及具体小问题保存在 `notes/round02/source_inventory.json`。统计与逐份语义审读范围严格分开，线上状态均为UNKNOWN。
