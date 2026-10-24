# 成都 2024 / 2025：CF1500 铜牌训练与打印取舍

核验日期：2026-10-24（北京时间）。以下只研究公开的 2024、2025 赛题和 2026 赛前规则，不包含或预测 2026 赛题。难度来自电子科技大学出题组官方题解第 2 页，题名以 UCup 赛站题目列表为准；训练价值、优先级及打印取舍是面向 CF1500 选手的编辑判断，不是官方 CF rating 对照。

**本表的推荐组件不等于本次已实现组件。** 本轮尚未收录 gcd 聚合线段树、max-plus 矩阵、换根 DP、dsu on tree；当前提供的是求和线段树、普通矩阵和基本 Euler 序/树 DP。下文相关 P0/P1 项目是训练与改造方向：求和线段树不能直接回答 gcd 聚合，普通乘加矩阵也不能直接替代 max-plus 转移。实际收录以最终 `notes/INDEX` 索引和书目录为准。

## 先读结论

- P0：先练两届 Easy 与可直接切入的构造、模拟、位运算、搜索；核心打印优先放整数边界、排序去重、前缀和、BFS/DFS 路径恢复，gcd 点修改线段树另作改造方向。模板不能替代状态定义和构造证明。
- P1：在已理解且能现场改写的前提下带 max-plus 矩阵快速幂、LCA、换根 DP、子树启发式合并、单调队列、Dijkstra/多源 Dijkstra。2025 B 的矩阵是 `max` 与 `+`，不是普通乘加矩阵。
- P2：费用流、WQS、复杂几何与交互决策树仅留索引或课后题单；临赛不靠增加厚模板弥补未掌握的建模。

打印列中的“代码”只指可复用组件，“清单”指简短思维/易错点提示；不把整份题解或整题代码塞入主打印版。P1 组件须以当前已通过验证的打印目录为准。

## 2024 全 13 题

官方难度已同时核对 PDF 文本与第 2 页渲染图：Easy 为 J、L；Easy-Medium 为 A、B、G、I；Medium 为 E、K；Medium-Hard 为 C、D、F；Hard 为 H、M。

| 题号 / 题名 | 官方难度 | 主要知识点 / 突破口 | 模板 vs 思维 | CF1500 训练价值 | 建议打印取舍 | 题面 / 题解 / 榜单 |
|---|---|---|---|---|---|---|
| A · Arrow a Row | Easy-Medium | 覆盖构造；必要条件与操作顺序 | 思维为主 | P0，高：从条件到可验证构造 | 是：构造检查清单，不放整题代码 | [题面](https://contest.ucup.ac/contest/1821/problem/9535) · [题解 p.5-6][T24A] · [榜单][S24] |
| B · Athlete Welcome Ceremony | Easy-Medium | 字符计数 DP；多维前缀和 | 状态设计 + 基础组件 | P1，高：离线预处理回答多询问 | 是：DP/前缀和提示；不放题目专用大数组 | [题面](https://contest.ucup.ac/contest/1821/problem/9536) · [题解 p.7-8][T24B] · [榜单][S24] |
| C · Chinese Chess | Medium-Hard | 交互；BFS 预处理、迭代加深决策树 | 专项思维与搜索实现 | P2，低：临赛投入较大 | 否：只在拓展索引保留 | [题面](https://contest.ucup.ac/contest/1821/problem/9537) · [题解 p.27-28][T24C] · [榜单][S24] |
| D · Closest Derangement | Medium-Hard | 最优错排结构；RMQ 找首个差异 | 证明为主，RMQ 辅助 | P2，中：学会压缩候选和比较 | 仅复用通用 RMQ，不放此题构造 | [题面](https://contest.ucup.ac/contest/1821/problem/9538) · [题解 p.21-24][T24D] · [榜单][S24] |
| E · Disrupting Communications | Medium | 换根计数；树上路径容斥、LCA | 建模 + 树模板 | P1，高但需补树 DP | LCA 可复用；前后缀换根为待补方向，避免除零 | [题面](https://contest.ucup.ac/contest/1821/problem/9539) · [题解 p.15-17][T24E] · [榜单][S24] |
| F · Double 11 | Medium-Hard | 分段 DP；决策单调性、WQS | 高阶推导 | P2，低：先补基础 DP | 否 | [题面](https://contest.ucup.ac/contest/1821/problem/9540) · [题解 p.25-26][T24F] · [榜单][S24] |
| G · Expanding Array | Easy-Medium | 位运算闭包；相邻对候选去重 | 思维为主 | P0，高：按位不变量、闭包枚举 | 是：位运算/去重清单，不放专用代码 | [题面](https://contest.ucup.ac/contest/1821/problem/9541) · [题解 p.9-12][T24G] · [榜单][S24] |
| H · Friendship is Magic | Hard | 十进制拆分；格点计数、分类求和 | 数学推导为主 | P2，低：非临赛优先项 | 否 | [题面](https://contest.ucup.ac/contest/1821/problem/9542) · [题解 p.29-31][T24H] · [榜单][S24] |
| I · Good Partitions | Easy-Medium | 下降断点下标 gcd；因数计数 | 核心观察 + 线段树 | P0，最高：题意转局部维护 | 改造方向：gcd 聚合 + 因数预处理；本轮不是现成组件 | [题面](https://contest.ucup.ac/contest/1821/problem/9543) · [题解 p.13-14][T24I] · [榜单][S24] |
| J · Grand Prix of Ballance | Easy | 事件合法性、重置、排名模拟 | 实现与读题 | P0，高：防罚时训练 | 是：模拟清单、64 位与时间戳重置提醒 | [题面](https://contest.ucup.ac/contest/1821/problem/9544) · [题解 p.4][T24J] · [榜单][S24] |
| K · Magical Set | Medium | 因数匹配；最大费用最大流 | 建模 + 费用流模板 | P2，中：会费用流后再练 | 否：主版不为此题扩张；普通匹配不足 | [题面](https://contest.ucup.ac/contest/1821/problem/9545) · [题解 p.18-20][T24K] · [榜单][S24] |
| L · Recover Statistics | Easy | 按题目定义构造频数 | 思维与精确读定义 | P0，高：避免误套分位数概念 | 是：构造与定义检查清单 | [题面](https://contest.ucup.ac/contest/1821/problem/9546) · [题解 p.3][T24L] · [榜单][S24] |
| M · Two Convex Holes | Hard | 投影/仿射变换；移动凸多边形面积积分 | 专项几何推导 | P2，低：铜牌短期暂缓 | 否 | [题面](https://contest.ucup.ac/contest/1821/problem/9547) · [题解 p.32-39][T24M] · [榜单][S24] |

## 2025 全 13 题

官方难度已同时核对 PDF 文本与第 2 页渲染图：Easy 为 G、J；Easy-Medium 为 A、B、D、L；Medium 为 C、K、M；Medium-Hard 为 H、I；Hard 为 E、F。

| 题号 / 题名 | 官方难度 | 主要知识点 / 突破口 | 模板 vs 思维 | CF1500 训练价值 | 建议打印取舍 | 题面 / 题解 / 榜单 |
|---|---|---|---|---|---|---|
| A · A Lot of Paintings | Easy-Medium | 四舍五入区间；整数化构造 | 思维与边界 | P0，高：严格/非严格不等号 | 是：整数化、构造验证清单 | [题面](https://contest.ucup.ac/contest/2567/problem/14706) · [题解 p.7-9][T25A] · [榜单][S25] |
| B · Blood Memories | Easy-Medium | 小集合状压；max-plus 矩阵幂加速 | 状态建模 + 关键模板 | P1，最高：少状态、大轮数 | 建议补充 max-plus；本轮仅普通矩阵，不能直接套 | [题面](https://contest.ucup.ac/contest/2567/problem/14707) · [题解 p.10-11][T25B] · [榜单][S25] |
| C · Crossing River | Medium | 排序；倒序贪心排程 | 思维为主 | P1，高：正向难时逆向构造 | 是：逆向思考提示，不放专用代码 | [题面](https://contest.ucup.ac/contest/2567/problem/14708) · [题解 p.19-20][T25C] · [榜单][S25] |
| D · Deductive Snooker Scoring | Easy-Medium | 规则建状态图；搜索、前驱恢复 | 建模为主，BFS/DFS 支撑 | P0，高：复杂叙述变有限状态 | 是：搜索与路径恢复；有环须判重 | [题面](https://contest.ucup.ac/contest/2567/problem/14709) · [题解 p.12-15][T25D] · [榜单][S25] |
| E · Escaping from Trap | Hard | 交互几何；有向面积、定位与缩放 | 专项推导 | P2，低：精度与交互成本高 | 否 | [题面](https://contest.ucup.ac/contest/2567/problem/14710) · [题解 p.39-43][T25E] · [榜单][S25] |
| F · Following Arrows | Hard | 网格构造；独立翻转贡献、背包组合 | 非标准构造为主 | P2，低：已有背包仍不能直接套 | 否 | [题面](https://contest.ucup.ac/contest/2567/problem/14711) · [题解 p.44-46][T25F] · [榜单][S25] |
| G · GCD of Subsets | Easy | gcd 相邻数性质；修改收益分类 | 数学思维为主 | P0，高：上界与可达构造 | 是：gcd/64 位边界提示 | [题面](https://contest.ucup.ac/contest/2567/problem/14712) · [题解 p.5-6][T25G] · [榜单][S25] |
| H · Heuristic Knapsack | Medium-Hard | 枚举前缀；贪心构造、再次模拟验证 | 思维与细节为主 | P2，中：适合构造复盘 | 仅验证构造的通用清单，不放专用代码 | [题面](https://contest.ucup.ac/contest/2567/problem/14713) · [题解 p.33-38][T25H] · [榜单][S25] |
| I · Inside Triangle | Medium-Hard | 凸包切线；双指针、动态配对计数 | 专项几何 + 计数 | P2，中：几何熟练后再练 | 否：基础叉积可留，整题不留 | [题面](https://contest.ucup.ac/contest/2567/problem/14714) · [题解 p.29-32][T25I] · [榜单][S25] |
| J · Judging Papers | Easy | 可改善论文计数；有限次机会分配 | 模拟/贪心 | P0，高：快速稳定取分 | 是：读题检查清单，不放专用代码 | [题面](https://contest.ucup.ac/contest/2567/problem/14715) · [题解 p.3-4][T25J] · [榜单][S25] |
| K · K-Coverage | Medium | 覆盖差分；增减贡献、滑窗最大值 | 拆贡献 + 单调队列 | P1，高：区间贡献分解 | 是：差分/前缀和/单调队列 | [题面](https://contest.ucup.ac/contest/2567/problem/14716) · [题解 p.21-24][T25K] · [榜单][S25] |
| L · Label Matching | Easy-Medium | 含通配符多重集；子树频次合并 | 条件化简 + dsu on tree | P1，高：子树统计常见模型 | 建议补充子树启发式合并；本轮不是现成组件 | [题面](https://contest.ucup.ac/contest/2567/problem/14717) · [题解 p.16-18][T25L] · [榜单][S25] |
| M · Meeting for Meals | Medium | 会合时间；多源最短路及来源标签 | 建模 + Dijkstra | P1，高：多源扩展训练 | 是：Dijkstra、多源初始化与来源记录 | [题面](https://contest.ucup.ac/contest/2567/problem/14718) · [题解 p.25-28][T25M] · [榜单][S25] |

题名勘误：2025 官方题解 p.7 将 A 写为 `Arrow Painting`，p.29 将 I 写为 `Inside Polygon`。赛站题目列表与链接分别使用 `A Lot of Paintings`、`Inside Triangle`，上表按赛站命名；正文题意已核对一致。

## 从历史榜单能说到哪一步

奖项身份以 ICPC 北京总部正式获奖名单为准：[2024 PDF](https://icpc.pku.edu.cn/docs/20250403154317586746.pdf#page=6)、[2025 PDF](https://icpc.pku.edu.cn/docs/2026-09/af24206f3813422584a27b6a78065ed5.pdf#page=6)。解题数与罚时从 [2024 UCup 榜单][S24]、[2025 UCup 榜单][S25] 的现场导入记录交叉核对。页面也混有后续训练/虚拟记录，因此不能直接拿页面当前总排名当现场排名。

| 年份 | 官方获奖名单的铜牌末位 | 对应现场成绩 | 相邻荣誉队成绩 |
|---|---|---|---|
| 2024 | 并列第 186：大连理工大学盘锦校区“这技校真练技术”、东北师范大学“巴别塔” | 都为 5 题 / 953 分钟；通过 A、G、I、J、L | 第 188 乐山师范学院“漫云征途”：5 题 / 989 分钟 |
| 2025 | 第 192：吉林大学“夏日空调幻想时刻” | 4 题 / 558 分钟；通过 A、B、G、J | 第 193 西南科技大学“飘零之剑”：4 题 / 563 分钟 |

这两次结果支持“先稳基础题、降低罚时，再争下一道”的训练顺序，但不能推出 2026 的铜牌题数、罚时线或获奖保证。CF1500 是个人水平线索，ICPC 还取决于队友分工、读题、实现稳定性、题型和当天参赛队伍。

可复查的最小证据已存入 `notes/sources.json`：官方 PDF URL、页码、SHA-256、现场记录标识及边界成绩。第三方榜单可辅助浏览：[2024 RankLand](https://rl.algoux.cn/ranklist/icpc2024chengdu)、[2025 RankLand](https://rl.algoux.cn/ranklist/icpc2025chengdu)；上述铜牌身份没有仅凭第三方颜色推定。

## 最有用的易错点

1. 2024 I：修改 `a[p]` 只影响 `p-1`、`p` 两个断点；没有断点时 gcd 为 0，应按题意单独处理，不能查“0 的因数个数”。
2. 2024 E：换根乘积的某个因子可能模意义下为 0；删掉一个儿子贡献时优先用前后缀乘积，不能无条件取逆元。
3. 2025 B：不可达值用负无穷；相加前检查可达性；矩阵单位阵对角为 0、其他为负无穷。
4. 2025 D：失败动作可以使状态成环；先标记访问，再继续搜索，同时记录动作以恢复方案。
5. 2025 A/G：优先用整数化和宽整数处理边界；不要因为官方标 Easy 就省掉极限与相等情形。
6. 构造题在本地写小规模验证器或手动重放；输出满足条件与“看起来合理”是两回事。

[S24]: https://contest.ucup.ac/contest/1821/standings?v=1
[S25]: https://contest.ucup.ac/contest/2567/standings?v=1
[T24A]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=5
[T24B]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=7
[T24C]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=27
[T24D]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=21
[T24E]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=15
[T24F]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=25
[T24G]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=9
[T24H]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=29
[T24I]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=13
[T24J]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=4
[T24K]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=18
[T24L]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=3
[T24M]: https://contest.ucup.ac/download.php?id=1821&r=1&type=attachments#page=32
[T25A]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=7
[T25B]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=10
[T25C]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=19
[T25D]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=12
[T25E]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=39
[T25F]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=44
[T25G]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=5
[T25H]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=33
[T25I]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=29
[T25J]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=3
[T25K]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=21
[T25L]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=16
[T25M]: https://contest.ucup.ac/download.php?id=2567&r=1&type=attachments#page=25
