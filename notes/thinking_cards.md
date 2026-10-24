# 赛场思考卡片

这些卡片来自本仓库实际源码阅读；“有练习代码”不等于该题已 AC。每次先写出可检查的性质，再选工具。

## 1. 构造：先满足被迫位置，再分配自由度

- 先列必要条件：总量、奇偶、对称、边界；不要直接试填。
- 配对约束先传播。回文题中已知字符决定镜像，冲突立即无解。
- 扣掉已使用的 0/1；奇数剩余必须由唯一中点承担，其余每次消耗 2。
- 输出后独立检查：长度、计数、固定位置、回文。构造程序最好配一个判定器。
- 实际练习：[回文构造源码](https://github.com/ball-castle/cf/blob/main/construction/1.cpp)；复习任务：手写长度 1、偶数长度但剩余奇数、两端冲突三个例子。

## 2. 操作题：找不变量，再看能否达到

- 每次操作分别改变什么：总和、奇偶、gcd、相对顺序、连通块数？不变的是必要条件，充分性还要构造。
- 重复减小数可合并为商：一次 Euclid 转移替代 `a / b` 次减法；零要单独定义。
- 涉及括号/配对时，先证明栈中对象代表什么、最后为什么恰剩两个；不能把 `top()` 当成证明。
- 实际练习：[除法批处理](https://github.com/ball-castle/cf/blob/main/ICPC/IC_14_xian/K.cpp)、[栈与 DSU](https://github.com/ball-castle/cf/blob/main/xian/B.cpp)；复习任务：解释终止量和最后一步的合法性。

## 3. 贪心：写出一次交换

- 把目标变成“最早可放 / 最小可选 / 最晚截止”，明确目前维护的可行性。
- 假设某个最优解第一处不同；把它交换成自己的选择。检查总成本不增、后续限制不更紧。
- 有固定锚点时，先算相邻锚点间必须放多少个 A，再分配；必须检查数量落在可用槽位范围内。
- 实际练习：[分段填空源码](https://github.com/ball-castle/cf/blob/main/ICPC/Yokohama%20Regional%202020/B2.cpp) 的 `need[x]`；复习任务：说明“优先放小数”对应哪个目标，题意未核实前不要假定它总最优。

## 4. 差分与区间：先确定查询对象

- 区间加只影响差分的两个边界；查询原数组单点是差分前缀和。前缀和本身不支持任意在线修改。
- 相等余数条件可转成相邻差的 gcd：原区间 `[l,r]` 对应差分 `[l+1,r]`，单点答案单独处理。
- 写清 0/1 下标、闭区间/半开区间。CF EDU 的 `[l,r)` 转本模板闭区间是 `[l+1,r]`。
- 实际练习：[差分 gcd 的 ST 表](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/ST%E8%A1%A8.cpp)、[CF EDU 区间和](https://github.com/ball-castle/cf/blob/main/segment_tree_cf_edu/1.cpp)；复习任务：用长度 1、2 和右端点 n 手算映射。

## 5. 图论：边的含义比算法名先出现

- 先定义点、边、方向、权重。无权最短路用 BFS，非负权用 Dijkstra；连通与最短距离不是同一个问题。
- 二分图染色要遍历每个连通分量，并检查已染色邻点冲突；只给未染色点涂色不能判定二分图。
- DSU 合并一次成功才减少一个连通块；Kruskal 选边后检查目标边数，断图不能返回“部分 MST”。
- 实际练习：[DSU](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/DSU.cpp)、[Kruskal](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/kruskal%20MST.cpp)；反例练习：[染色草稿](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/graph_paint.cpp) 缺边输入和冲突检测。

## 6. DP：状态语义、可达性、转移范围

- 用一句话定义 `dp[i]`“恰好/至多什么”；先写最慢正确转移，再找重复查询。
- 若当前选择只依赖前一层中比它小的选择，排序后用双指针维护前缀最优，可避免两层笛卡尔积。
- 不可达状态不参加加法；01 背包容量倒序、完全背包正序；滚动数组每轮重新初始化。
- 实际练习：[子序列与前缀最大 DP](https://github.com/ball-castle/cf/blob/main/ICPC/JAKARTA_2018/J.cpp)、[01 背包](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/01bag.cpp)。
- 反例练习：[西安 C 草稿](https://github.com/ball-castle/cf/blob/main/xian/C.cpp) 的分号把费用项变成无效表达式；写优化前先用小规模暴力检查每一项费用。

## 卡住时的三分钟

1. 写 n≤6 暴力；枚举合法对象而不只是随机输入。
2. 记录猜想的最小反例，区分“性质不成立”与“代码未实现性质”。
3. 口头交接：状态/不变量、复杂度、仍未证明的一步、反例。不要只说“应该能贪心”。
