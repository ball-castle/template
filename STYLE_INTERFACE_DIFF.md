# ROUND01 与个人比赛写法：风格及接口差异

这是 2026-10-24 基线的逐模块评估，2026-10-25 完成记录。本审计没有修改算法源码。表内“建议”是源选择与最小改动建议，**不表示对应比赛版本已经实现或通过测试**；实际落地须看本轮模块元数据及验证报告。

用户要求优先直接收录自己验证使用过的代码，只有确切问题才修。因而“全局数组、1下标、独立函数”是默认方向，不是把原本正确的 vector/struct 一律重写的指令。所有新版本必须保留原文件、原32模块和四份打印成品。

## 影响现场调用的实际差异

| 能力 | 用户原接口/组织 | ROUND01 正式接口/组织 | 影响和最小选择 |
|---|---|---|---|
| DSU | `fa/rk`，`init(n)`，`find/unite/same` 独立函数 | `cp::DSU d(n)`，内部 `p/sz`，unite返回bool，并有size/components | 用户原函数足够时直接用原版；需要块大小/成功合并状态时明确补字段或返回语义，不静默改接口。 |
| Fenwick | 原本就是 `Fenwick fen(n)`，`tr`，`add/sum/query` | `Fenwick`，`bit`，`add/sum/range_sum` | 原文件只需改 main 的不存在调用 `rangeSum`→`query`。原struct/vector已经是个人源码，优先保留。 |
| 单点线段树 | 全局 `a/tree`，`build(1,1,n)`，`update/add/query` | `SegmentTree st(a)`，`st.set/st.sum`，私有pull/update/query | 直接引用原函数比新套一层更熟悉。`set`是赋值，`add`是增量，不能仅凭改名混同。 |
| Lazy线段树 | 全局 `a/tree/lazy`，`pushup/pushdown/apply/modify/query` | `LazySegmentTree`，`tr/tag`，私有pull/push | 只修旧源码具体声明、下传、宽度问题；区间赋值属于新增功能，需要独立标记顺序证明。 |
| BFS/DFS | `g[N]`、visited，独立bfs/dfs | `cp::bfs(g,s)`返回BFSResult；`dfs_forest`返回parent/comp/order | 原遍历基础直接收录；距离/路径/全图/深链能力明确追加，不能用“接口统一”隐去返回信息。 |
| LCA | 旧 `binary_lifting.cpp` 实际不是完成的LCA | `cp::LCA lca(g,root)`，up[level][u]，ok/jump/lca/distance | 没有个人完整LCA来源就如实写；若改全局fa[u][k]，来源仍是正式版，须对照同一oracle。 |
| DP | 全局 `dp`、就地循环，也有局部vector/递归lambda | 多数独立函数接vector返回结果 | 01/完全/LIS/区间原循环先用；数位DP和状态机直接用实战源，不强行函数工厂或泛型。 |
| 数组下标 | 数组/节点常1下标，字符串/矩阵/位号常0下标 | 大部分1下标，但字符串半开区间与矩阵0下标 | 保留真正自然的下标；特别写明外部CF EDU [l,r)→内部[l+1,r]。 |
| 快读 | `inline ll read()`，getchar，返回ll | ROUND01没有正式快读 | 本轮原样收录，不换返回类型、不加fread、不加安全包装；特殊输入用cin。 |

## 32 个正式模块的逐项决策

“保留”表示继续引用已验证规范源码；“原源优先”表示有具体可复用个人来源，只有必要的能力差异或已知缺陷才允许修改。不把每个建议都转化成第二份功能相同的实现。

| ID | 基线组织和接口 | 个人来源/实际差异 | 建议及兼容性重点 |
|---|---|---|---|
| CO01 | solve/main，无cp，cin.tie(nullptr) | cf大量使用cin.tie(0)，功能一样 | 保留骨架；注释的默认局部vector可改成个人偏好说明，不必改可执行代码。 |
| CO02 | PrefixSum/Difference/Compression三个struct | 原实战多直接pre/s等数组；没有同能力完整旧源 | 保留已有实现，教学例用个人循环；二维等新能力单独增加，避免复制一维代码。 |
| CO03 | 泛型check，optional返回first/last_true | `binary_search_num`是找值；Hello_2018/D有独立check答案二分 | 保留通用正式函数；主模型优先原check与while，明确无解约定。 |
| DP01 | vector函数，ll，exact开关和NEG | 旧01/完全背包全局int dp、至多容量 | 原循环优先；需要ll和恰好装满时最小增加，不能丢掉不可达处理。 |
| DP02 | 平方与nlogn LIS函数，strict开关 | 旧线性dp有全局平方LIS | 平方原源优先；nlogn是独立优化而非陌生封装，保留其有效用途。 |
| DP03 | 扁平vector Hamilton DP | 已深读个人mask代码是其他递推 | 保留；不能把相关mask语法当直接Hamilton来源。 |
| DP04 | ll vector二维区间dp | 原`区间dp.cpp`循环完整，int易受总成本范围限制 | 原结构优先，只扩宽实际需要的值域；继续注明直线/非负。 |
| DS01 | Fenwick struct/vector，range_sum | 原Fenwick也是struct/vector且方法名query | 原源优先；修错调用，保留原形式，不机械全局化。 |
| DS02 | class式struct、set/sum、private pull | 原全局tree/pushup/build/update/add/query已成熟 | 原源优先；初始化、点赋值和点加分别标明。 |
| DS03 | struct tr/tag、pull/push、add/sum | 原全局tree/lazy、pushup/pushdown/modify/query | 原源最小修复优先；原update若与lazy混用必须先下传。 |
| DS04 | 独立函数返回vector/deque | 已深读源无等价完整单调结构 | 保留，不为了风格造第二份。 |
| DS05 | 泛型SparseTable，st[level][i] | 旧差分gcd用st[i][level] | 保留可靠通用表；个人差分gcd可直接引用原题片段。两种维度次序不能混抄。 |
| G01 | cp::DSU，按大小，路径减半 | 原fa/rk，递归压缩，按秩 | 原源优先；仅修“挂到秩小”注释。若新增size/块数，须独立说明。 |
| G02 | cp::BFSResult，有dist/parent/path | 旧bfs是g[N]遍历，仅visited | 原遍历基础必须收录；距离与恢复是新增用途，不能假称旧源已具备。 |
| G03 | cp::DFSForest显式栈，全分量 | 旧dfs独立递归，只起点可达分量 | 保留迭代正式版处理深链，原基础直接展示；职责和递归前提区分。 |
| G04 | cp::topological_sort(g)返回vector | cf/1704E是全局in/e/tp+queue | 可直接提取原Kahn循环；通用化需检查tp.size()==n。 |
| G05 | cp::Unicycle，先检查简单连通基环图 | 未找到已深读个人同能力源 | 保留；不要为去cp而重写一套环恢复。 |
| G06 | cp::DijkstraResult，多源、路径、INF饱和 | 没有已深读个人完整Dijkstra来源 | 保留算法；若主打印适配独立函数，注明来源正式版，仅改必要接口并对照。 |
| G07 | cp::KruskalResult返回森林信息 | 原edge数组/fa/cmp，求恰k分量 | 原结构可直接用于对应k森林模型；不静默用它替代所有正式返回语义。 |
| G08 | cp::PrimResult，vector图、多分量 | 原链式前向星/Prim/int总权/单次连通检查 | 原图存储值得收录；ll宽度和多分量扩展是实际差异，不是纯命名变更。 |
| G09 | cp::LCA结构体，构造时BFS倍增 | 旧binary_lifting是未完成ST | 不虚构原LCA；适配只从已验证正式代码来，保留根哨兵和超根行为。 |
| G10 | cp::EulerTree，tin/tout/order，逆序子树和 | tree_storage/tree_dp/hdu/B有g[N]和子后父思想 | 原基础直接用；Euler区间与深链迭代仍引用正式版，不混称一个模板。 |
| G11 | cp::SCCResult，迭代Kosaraju | 未找到个人等价源 | 保留P2进阶层，不阻塞补丁。 |
| G12 | cp::bridges，边id处理平行边 | 未找到个人等价源 | 保留P2进阶层；不能为简短丢掉父边id。 |
| M01 | gcd_abs/checked lcm，unsigned与optional | 原gcd为非负递归；cf位置偏差gcd | 原简单域可直接用；更强完整ll域保护保留供需要时引用。 |
| M02 | mod_norm/add/mul/pow，多模数与i128 | 原ksm在固定质数、安全乘法域短小 | 固定模模型优先原ksm；mod=1等扩展注明，不把域外输入当重写理由。 |
| M03 | Matrix=vector<vector<ll>>，mul/pow | 原matrix_ksm本身也用vector矩阵 | 保留，vector并不违背用户习惯；是普通模乘，不是max-plus。 |
| M04 | 特定product_recurrence函数，依赖M02/03 | 旧同公式有ans=0具体错误 | 正式已有修复和验证，直接引用；不再制作第三份乘积递推。 |
| M05 | PrimeComb类式struct，动态prime/n | cf/date/5/5.19/3有全局fac/ifac/ksm/ncr/init | 个人固定模版可直接提取；泛型质数参数不是现场必须，选单一规范主源。 |
| S01 | prefix_function/kmp_find_all/Z，0下标 | 原get_pi/kmp已熟悉，CF回文前后缀也用0下标 | 保留正式增强接口；原非空模式版可直接展示用途，别机械改1下标。 |
| S02 | DoubleHash struct，半开区间 | 无已深读个人等价哈希 | 保留；新增回文调用时明确反串映射与碰撞限制。 |
| S03 | 独立基础字符串函数 | 原construction/1是配额回文构造 | 两种不同能力都保留；构造直接收原源，不重写判定函数。 |

## 有证据的小修与只有条件差异的情况

可以定位的源码问题包括：旧 Fenwick 第61行调用名错误；旧 Segment tree2 的 apply 未先声明、部分下行缺下传、求和类型窄；旧 matrix_ksm 第85行乘积初值为0；`cf/xian/C.cpp:30–31` 分号导致费用项成为无效表达式。修复应围绕这些具体行和反例，不演变为整体重设计。

需要先明确题域的情况不同：`cf/1/1124/B.cpp` 的int popcount只有当输入确实超32位时才需扩宽；`cf/3/1096/D_mex_set.cpp` 的第二个0可能由题目保证；单测全局容器没有clear不是原单测错误；`cf/xian/B.cpp` 最后取栈中两个元素可能由原构造不变量保证。审计将这些记为条件或待证步骤，没有自动否定用户原代码。

## 版本与验证约定

优先让主打印版引用一个规范源码。只引用原版本而未改变逻辑时，验证实际合法边界与调用即可；不要为了测试所谓“安全输入”改变原read契约。

确需个人比赛版与ROUND01版并存时，二者必须在同一批输入下分别与独立oracle比较，并记录结果一致性。只有互相比较不足以排除共同错误。点赋值与点加、树遍历与路径恢复、k森林与完整MST等能力不同，必须先限定共有契约再比较，不能把接口语义不等价的问题伪装成回归失败。

原源路径、哈希、行号和本轮建议见 [provenance_baseline.json](notes/round02/provenance_baseline.json)；50份个人源码逐项观察见 [source_inventory.json](notes/round02/source_inventory.json)。这些是审计证据，最终测试状态由实际测试运行更新。
