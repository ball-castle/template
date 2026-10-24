# 线段树与 Fenwick 个人比赛 Cookbook

> 规范数据：notes/round02/cards/segment.json、notes/round02/modules/PSD*.json。本文件由 competition/data_structure/build_cookbook.py 从卡片与规范源码生成；修改代码后重新运行生成器，不手工维护另一份树。

正式代码优先沿用用户的全局 a/tree/lazy、1 下标和pushup/pushdown/build/modify/query。原文件与 ROUND01 的 DS01/DS02/DS03 均保留；新旧版本在相同随机操作、独立数组 oracle 下比较。用户已经使用验证的经验予以保留；没有核验的线上提交状态记为 UNKNOWN。

## 一页速查：Fenwick、ST 表、单调队列、线段树

| 题目限制 | 第一选择 | 必须检查 |
|---|---|---|
| 静态区间和 | 前缀和 | 区间由两个前缀相减 |
| 只在最后看结果的区间加 | 差分 | 中间没有在线询问 |
| 单点加、前缀/区间和 | PSD01 Fenwick | 操作可消去；不能减两个前缀min |
| 静态min/max/gcd | ROUND01 DS05 ST表 | 重叠查询需幂等；sum不行 |
| 固定窗口min/max | ROUND01 DS04 单调队列 | 窗口移动规则固定，非任意区间 |
| 动态任意区间或多个统计量 | PSD02单点树+Node变体 | merge结合律；有些merge不交换 |
| 区间加、和/min/max | PSD10 | sum加len*x；min/max各加x |
| 区间赋值或赋值+加 | PSD11 | has_set区分赋值0；先set后add下传 |

| 要维护的答案 | 最小 Node / 查询信号 |
|---|---|
| 和 / min / max / gcd | sum / mn / mx / g；四种都只需一个统计量 |
| 最大值和次数 | mx,cnt；mx相同才相加cnt |
| 最大连续子段和 | sum,pref,suff,best；跨界候选suff左+pref右 |
| 第k个 | 非负频率sum；不足k就跳过左子树 |
| [L,R]第一个>=x | mx<x整段排除；先左后右 |
| 多个统计量 | 每一项都能合并；可merge不等于可lazy |

边界先写清：modify是赋值还是加；[L,R]是闭区间还是半开；n=0不建本节全局树；全负最大子段默认非空；gcd禁止LLONG_MIN；第k个必须非负频率。

## 为什么不同题需要不同 Node

Node是一个区间对未来合并所需的摘要。先问：拼接左右两段时，答案是否只由两段答案决定？和、最小值可以；最大子段和不可以，因为最优子段可能跨分界，所以补充左右前缀、后缀和。再问：题目操作能否直接作用于摘要？区间加可直接更新sum/min/max；只存gcd不能直接更新，因为[2,4]与[2,6]原gcd同为2，整体+2后分别为2与4。

merge只需结合律，不要求交换律。最大子段的左右方向不能颠倒；原矩阵练习以列向量表示状态，先执行左段再执行右段，组合矩阵必须是right*left。该练习额外保存翻转后的rev，使整段A/B互换可通过交换mat/rev完成。

## 三棵完整树骨架与一个 Fenwick

每个个人版本独立用于一份题目，不要把多个含同名全局数组的版本一起粘贴。工程中的.hpp已组合好Node与公共骨架，可以直接include。纸面抄写时把对应Node、make_node、merge放在单点公共骨架前；下面PSD02已给出完整组合，其他点改模型只替换这三项。测试内部的namespace只是为了同进程对照，不是要求比赛使用cp::。

### PSD01 Fenwick：个人add/query/rangeSum

来源与规范源码：[competition/data_structure/fenwick.hpp](competition/data_structure/fenwick.hpp)。

契约：0<=n<2^30且内存足够；1下标，add只能1..n；query(0)=0；空rangeSum返回0；所有中间量在ll内。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 保留旧 Fenwick 的 n、tr、add 和 lowbit 循环。
// 1 下标；add 是增量，query(x) 是前缀，rangeSum(l,r) 是闭区间。
// 0<=n<2^30 且内存足够；所有节点、前缀及中间值须在 ll 范围。
struct Fenwick {
    int n;
    vector<ll> tr;
    explicit Fenwick(int size) : n(size), tr(size + 1) {
        assert(size >= 0);
    }
    void add(int x, ll val) {
        assert(1 <= x && x <= n);
        for (; x <= n; x += x & -x) tr[x] += val;
    }
    ll query(int x) const {
        assert(0 <= x && x <= n);
        ll res = 0;
        for (; x; x -= x & -x) res += tr[x];
        return res;
    }
    ll rangeSum(int l, int r) const {
        if (l > r) return 0;
        assert(1 <= l && r <= n);
        return query(r) - query(l - 1);
    }
};
```

接口仅统一名字：旧sum(x)改名query(x)，旧query(l,r)改名rangeSum(l,r)。原main中的rangeSum现在对应真实接口；add仍是增量。

### PSD02 单点树公共骨架与区间和

来源与规范源码：[competition/data_structure/segment_point_sum.hpp](competition/data_structure/segment_point_sum.hpp)。

契约：1<=n<N；1下标闭区间；modify是赋值；a仅是建树输入；所有中间和须在ll内。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
struct Node { ll sum; };
Node make_node(ll val) { return {val}; }
Node merge(const Node& x, const Node& y) { return {x.sum + y.sum}; }
// 公共单点树骨架：先放一个 Node / make_node / merge，再接本文件。
// 每个 segment_point_*.hpp 均已准确包含所需节点与本骨架，可直接 include。
// 保留旧代码 a/tree/node/l/r/mid 和递归结构；update 改名 modify。
// 非空树，1<=n<N；1<=pos<=n；查询必须 1<=L<=R<=n。
// a 仅为建树输入缓冲；modify 后当前值以 tree 为准。
const int N = 200000 + 5;
ll a[N];
Node tree[N * 4];

void pushup(int node) {
    tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
}
void build(int node, int l, int r) {
    if (l == r) {
        tree[node] = make_node(a[l]);
        return;
    }
    int mid = (l + r) >> 1;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node);
}
void apply(int node, ll val) {
    tree[node] = make_node(val);
}
void modify(int node, int l, int r, int pos, ll val) {
    if (l == r) {
        apply(node, val);
        return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) modify(node * 2, l, mid, pos, val);
    else modify(node * 2 + 1, mid + 1, r, pos, val);
    pushup(node);
}
Node query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    int mid = (l + r) >> 1;
    if (R <= mid) return query(node * 2, l, mid, L, R);
    if (L > mid) return query(node * 2 + 1, mid + 1, r, L, R);
    return merge(query(node * 2, l, mid, L, R),
                 query(node * 2 + 1, mid + 1, r, L, R));
}
```

单点树不需要lazy或pushdown。query仅进入相交儿子，所以max/min/非空子段不必造一个容易写错的空节点。所有树接口使用合法非空闭区间；a是建树缓冲，修改后当前值保存在tree。

### PSD10 区间加：和、最小值、最大值

来源与规范源码：[competition/data_structure/segment_range_add.hpp](competition/data_structure/segment_range_add.hpp)。

契约：1<=n<N；修改和查询是合法非空闭区间；a为初始输入；和、标记、长度*x及中间量在ll内。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 继承旧 Segment tree2 / 6.9 的全局树与递归写法。
// 非空树，1<=n<N；修改/查询须为 [1,n] 内非空闭区间。
// 所有和、标记、长度*增量及中间值须在 ll 范围。
const int N = 200000 + 5;
struct Node { ll sum, mn, mx; };
ll a[N], lazy[N * 4];
Node tree[N * 4];
Node merge(const Node& x, const Node& y) {
    return {x.sum + y.sum, min(x.mn, y.mn), max(x.mx, y.mx)};
}
void pushup(int node) {
    tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
}
void apply(int node, int l, int r, ll val) {
    tree[node].sum += (r - l + 1LL) * val;
    tree[node].mn += val;
    tree[node].mx += val;
    lazy[node] += val;
}
void pushdown(int node, int l, int r) {
    if (lazy[node] == 0 || l == r) return;
    int mid = (l + r) >> 1;
    apply(node * 2, l, mid, lazy[node]);
    apply(node * 2 + 1, mid + 1, r, lazy[node]);
    lazy[node] = 0;
}
void build(int node, int l, int r) {
    lazy[node] = 0;
    if (l == r) {
        tree[node] = {a[l], a[l], a[l]};
        return;
    }
    int mid = (l + r) >> 1;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node);
}
void modify(int node, int l, int r, int L, int R, ll val) {
    if (L <= l && r <= R) {
        apply(node, l, r, val);
        return;
    }
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    if (L <= mid) modify(node * 2, l, mid, L, R, val);
    if (R > mid) modify(node * 2 + 1, mid + 1, r, L, R, val);
    pushup(node);
}
Node query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    if (R <= mid) return query(node * 2, l, mid, L, R);
    if (L > mid) return query(node * 2 + 1, mid + 1, r, L, R);
    return merge(query(node * 2, l, mid, L, R),
                 query(node * 2 + 1, mid + 1, r, L, R));
}
```

若题目只需要sum，可以删掉Node的mn/mx及对应三处赋值；本手册和测试的正式源保留三个字段，避免并行维护三份相同lazy树。

### PSD11 区间赋值与区间加：和

来源与规范源码：[competition/data_structure/segment_set_add.hpp](competition/data_structure/segment_set_add.hpp)。

契约：1<=n<N；合法非空闭区间；true表示赋值、false表示加；所有中间量在ll内；允许赋值0。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 在旧区间加树上扩展赋值；true=赋值，false=加。
// 非空树，1<=n<N；区间须合法；所有中间值须在 ll 范围。
const int N = 200000 + 5;
struct Tag { bool has_set; ll set, add; };
ll a[N], tree[N * 4];
Tag lazy[N * 4];
void pushup(int node) {
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}
void apply(int node, int l, int r, ll val, bool is_set) {
    if (is_set) {
        tree[node] = (r - l + 1LL) * val;
        lazy[node] = {true, val, 0}; // 新赋值覆盖之前的赋值和增量。
    } else {
        tree[node] += (r - l + 1LL) * val;
        lazy[node].add += val;
    }
}
void pushdown(int node, int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (lazy[node].has_set) { // 下传必须先 set，再 add。
        apply(node * 2, l, mid, lazy[node].set, true);
        apply(node * 2 + 1, mid + 1, r, lazy[node].set, true);
    }
    if (lazy[node].add != 0) {
        apply(node * 2, l, mid, lazy[node].add, false);
        apply(node * 2 + 1, mid + 1, r, lazy[node].add, false);
    }
    lazy[node] = {};
}
void build(int node, int l, int r) {
    lazy[node] = {};
    if (l == r) { tree[node] = a[l]; return; }
    int mid = (l + r) >> 1;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node);
}
void modify(int node, int l, int r, int L, int R, ll val, bool is_set) {
    if (L <= l && r <= R) {
        apply(node, l, r, val, is_set);
        return;
    }
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    if (L <= mid) modify(node * 2, l, mid, L, R, val, is_set);
    if (R > mid) modify(node * 2 + 1, mid + 1, r, L, R, val, is_set);
    pushup(node);
}
ll query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    ll ans = 0;
    if (L <= mid) ans += query(node * 2, l, mid, L, R);
    if (R > mid) ans += query(node * 2 + 1, mid + 1, r, L, R);
    return ans;
}
```

Tag表示函数T(x)=(has_set ? set : x)+add。compose(old,new)意味着先old后new，即new(old(x))：new若有赋值，结果直接变成new；否则只把new.add加到old.add。因此新赋值覆盖旧增量，新的加法接在旧赋值之后；pushdown固定先set再add。

## 14 个维护模型：信号、推导、代码和真实调用

### SEG-01 单点改值、区间求和 · PSD02

**识别信号：** 每次只改一个位置，却反复问一段总量。

**先观察：** 一次改值影响哪些区间；输入下标是闭区间还是[l,r)。

**暴力：** 每次查询逐个相加，O(n)。

**重复计算：** 同一段会被反复求和，且sum(左+右)=sum(左)+sum(右)。

**必要限制：** 必要：只作单点赋值，所问答案确为元素和。充分：父段sum可由左右sum相加得到。 边界契约：1<=n<N；1下标闭区间；modify是赋值；a仅是建树输入；所有中间和须在ll内。

**正确性：** 每次修改重算该叶到根路径；查询按从左到右的互不相交段合并。

**复杂度：** 建树O(n)，修改/查询O(log n)，空间O(n)。

**怎样推出：** 把数组二分成固定区间，修改只重算包含该点的O(log n)层；若只需加法可先选Fenwick。

| 维护项 | 具体规则 |
|---|---|
| Node | sum：区间元素和。 |
| pushup | sum=x.sum+y.sum。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_point_sum.hpp](competition/data_structure/segment_point_sum.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); cout<<query(1,1,n,L,R).sum;
```

**完整可运行调用：** [competition/data_structure/examples/point_sum.cpp](competition/data_structure/examples/point_sum.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_point_sum.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.sum << '\n';
        }
    }
}
```

输入格式：2 1 3表示查询；1 pos val表示赋值。

输入：
```text
3 3
3 -2 4
2 1 3
1 2 5
2 1 3
```
期望输出：
```text
5
12
```

**最容易出错：** modify为赋值；加delta需用当前值+delta。；n=0不能build；查询不接受L>R。；a仅供建树，修改后请读tree或另行同步a。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/point_sum.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-02 单点改值、区间最小值 · PSD03

**识别信号：** 在线改变点值，并问任意区间的最低值。

**先观察：** 有修改就不能直接沿用静态ST表；min没有前缀相减法。

**暴力：** 遍历[L,R]取min。

**重复计算：** 左右最小值足够确定父区间最小值，原始元素不必反复扫描。

**必要限制：** 必要：只有单点改值；静态时可用ST替代。充分：左右mn的min决定父段mn。 边界契约：沿用PSD02边界；允许负数及LLONG_MIN；无空区间查询。

**正确性：** 每个元素恰属一个查询子段，min满足结合律。

**复杂度：** 建树O(n)，修改/查询O(log n)，空间O(n)。

**怎样推出：** 把Node从sum换成mn，叶子就是当前点值，其余单点骨架完全复用。

| 维护项 | 具体规则 |
|---|---|
| Node | mn：区间最小值。 |
| pushup | mn=min(x.mn,y.mn)。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_point_min.hpp](competition/data_structure/segment_point_min.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
struct Node { ll mn; };
Node make_node(ll val) { return {val}; }
Node merge(const Node& x, const Node& y) { return {min(x.mn, y.mn)}; }
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); cout<<query(1,1,n,L,R).mn;
```

**完整可运行调用：** [competition/data_structure/examples/point_min.cpp](competition/data_structure/examples/point_min.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_point_min.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.mn << '\n';
        }
    }
}
```

输入格式：2 l r查询；1 pos val赋值。

输入：
```text
3 3
5 2 7
2 1 3
1 2 8
2 1 3
```
期望输出：
```text
2
5
```

**最容易出错：** 最小值不能把不相交区间返回0。；旧4e18有限哨兵不足以覆盖所有ll；本版用分支查询避免哨兵。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/2.cpp 的点改/最小值模型。本轮可运行模型：competition/data_structure/examples/point_min.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-03 单点改值、区间最大值 · PSD04

**识别信号：** 每次改一个点，并问一段当前的最大高度/价值。

**先观察：** 最大值可能全为负数；不能用0冒充空段。

**暴力：** 逐个枚举区间元素取max。

**重复计算：** 父区间最大值只依赖两个儿子的最大值。

**必要限制：** 必要：只有单点改值；负数不能当无贡献。充分：左右mx的max决定父段mx。 边界契约：沿用PSD02边界；first_ge允许L>R并返回-1；值可为任意ll。

**正确性：** 每个区间元素位于且只位于一个儿子；两侧最大值的较大者就是整个区间最大值。

**复杂度：** 建树O(n)，修改/区间查询/first_ge均O(log n)，空间O(n)。

**怎样推出：** Node换成mx；查询骨架只进入相交儿子，因此无需人为负无穷哨兵。

| 维护项 | 具体规则 |
|---|---|
| Node | mx：区间最大值。 |
| pushup | mx=max(x.mx,y.mx)。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_point_max.hpp](competition/data_structure/segment_point_max.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
struct Node { ll mx; };
Node make_node(ll val) { return {val}; }
Node merge(const Node& x, const Node& y) { return {max(x.mx, y.mx)}; }
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); cout<<query(1,1,n,L,R).mx;
```

**完整可运行调用：** [competition/data_structure/examples/point_max.cpp](competition/data_structure/examples/point_max.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_point_max.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.mx << '\n';
        }
    }
}
```

输入格式：2 l r查询；1 pos val赋值。

输入：
```text
3 3
5 2 7
2 1 3
1 2 8
2 1 3
```
期望输出：
```text
7
8
```

**最容易出错：** max不能用0代表空段，全负区间会错误。；first_ge只查给定[L,R]；必须先左后右才是第一个。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/point_max.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-04 单点改值、区间gcd · PSD05

**识别信号：** 一段数反复求最大公约数，中间会改一个数。

**先观察：** 先看是否允许负数；gcd结果统一非负，0与其他数的gcd等于后者绝对值。

**暴力：** 逐个用Euclid累计gcd。

**重复计算：** gcd有结合律，左右各自gcd足够；但区间加不能只由旧gcd算出新gcd。

**必要限制：** 必要：点改；所有绝对值能用ll表示。充分：gcd满足结合律，叶子统一非负。 边界契约：沿用PSD02边界；负数先取绝对值，gcd(0,0)=0；LLONG_MIN不在契约内。

**正确性：** gcd结合律使分段方式无关；取绝对值不改变共同正因数。

**复杂度：** 建树O(n log V)，修改/查询O(log n log V)，空间O(n)。

**怎样推出：** 叶子先取绝对值；merge取gcd，继续复用单点骨架。

| 维护项 | 具体规则 |
|---|---|
| Node | g：区间所有元素绝对值的gcd。 |
| pushup | g=gcd(x.g,y.g)。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_point_gcd.hpp](competition/data_structure/segment_point_gcd.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
// gcd 统一非负；禁止 LLONG_MIN，因为其绝对值不能用 ll 表示。
struct Node { ll g; };
Node make_node(ll val) {
    assert(val != LLONG_MIN);
    return {val < 0 ? -val : val};
}
Node merge(const Node& x, const Node& y) { return {gcd(x.g, y.g)}; }
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); cout<<query(1,1,n,L,R).g;
```

**完整可运行调用：** [competition/data_structure/examples/point_gcd.cpp](competition/data_structure/examples/point_gcd.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_point_gcd.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.g << '\n';
        }
    }
}
```

输入格式：2 l r查询；1 pos val赋值。

输入：
```text
3 3
-6 10 0
2 1 3
1 2 -9
2 1 2
```
期望输出：
```text
2
3
```

**最容易出错：** std::gcd返回非负；LLONG_MIN的绝对值不能用ll表示，不支持。；区间加无法只用旧gcd推出新gcd；[2,4]与[2,6]原gcd同为2，加2后分别为2和4。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/point_gcd.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-05 区间加、区间和 · PSD10

**识别信号：** 每次对整段加同一个数，而且修改间还要在线查询任意子段和。

**先观察：** 若只在最后看数组用差分；若更新后立刻查询，需可延后下传的信息。

**暴力：** 每次区间修改逐元素加，每次查询逐元素加总。

**重复计算：** 整段长度已知，整体加x使和恰好增加len*x，无须访问每个叶子。

**必要限制：** 必要：每次覆盖段内所有元素增加同一val。充分：len已知，sum可直接增加len*val，标记可相加。 边界契约：1<=n<N；修改和查询是合法非空闭区间；a为初始输入；和、标记、长度*x及中间量在ll内。

**正确性：** 整段平移保持大小顺序，标记相加表示时间上的连续平移；下传后子树与父树状态一致。

**复杂度：** 建树O(n)，区间修改/查询O(log n)，空间O(n)。

**怎样推出：** 完全覆盖时改当前sum并记lazy；部分覆盖前pushdown，返回后pushup。

| 维护项 | 具体规则 |
|---|---|
| Node | sum,mn,mx；本题读取sum。 |
| pushup | sum=x.sum+y.sum。 |
| 是否lazy | 是；lazy[node]为本节点已处理、尚未传子的累计增量。 |
| apply | sum+=(r-l+1)*val；mn+=val；mx+=val；lazy+=val。 |
| pushdown | 先apply左右儿子，再lazy[node]=0；部分修改和部分查询都先下传。 |
| 查询 | 完整覆盖返回Node；部分覆盖先pushdown，再按左右顺序merge。 |

**核心代码：** [competition/data_structure/segment_range_add.hpp](competition/data_structure/segment_range_add.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,L,R,val); cout<<query(1,1,n,L,R).sum;
```

**完整可运行调用：** [competition/data_structure/examples/range_add.cpp](competition/data_structure/examples/range_add.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_range_add.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            ll val; cin >> val;
            modify(1, 1, n, l, r, val);
        } else {
            auto ans = query(1, 1, n, l, r);
            cout << ans.sum << ' ' << ans.mn << ' ' << ans.mx << '\n';
        }
    }
}
```

输入格式：1 l r delta区间加；2 l r同时输出sum min max。

输入：
```text
3 4
3 -2 4
2 1 3
1 1 2 5
2 1 3
2 2 2
```
期望输出：
```text
5 -2 4
15 3 8
3 3 3
```

**最容易出错：** 查询部分覆盖也必须pushdown。；sum加len*x，mn/mx只加x。；build清lazy后才可多测复用；不要直接调用旧update跳过标记。

**来源/练习：** 本地练习：cf/date/6/6.9/1_special_segment_tree.cpp 的区间加/最小值局部模型。本轮可运行模型：competition/data_structure/examples/range_add.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-06 区间加、区间最小值 · PSD10

**识别信号：** 给一段整体平移，同时问最低值或某个前缀约束是否被破坏。

**先观察：** 整段统一加法是否保持元素相对顺序；原6.9练习维护d[x]=x-count(<=x)。

**暴力：** 逐个加、逐个扫描最小值。

**重复计算：** 共同加x不改变谁最小，min只增加x。

**必要限制：** 必要：每次整段加同一val而非按位置加不同量。充分：共同平移保持相对大小，mn直接增加val。 边界契约：1<=n<N；修改和查询是合法非空闭区间；a为初始输入；和、标记、长度*x及中间量在ll内。

**正确性：** 整段平移保持大小顺序，标记相加表示时间上的连续平移；下传后子树与父树状态一致。

**复杂度：** 建树O(n)，区间修改/查询O(log n)，空间O(n)。

**怎样推出：** 复用区间加骨架中的mn字段；需要最小位置时可在Node额外记录并处理相等取左。

| 维护项 | 具体规则 |
|---|---|
| Node | mn（本实现同时保留sum,mx）。 |
| pushup | mn=min(x.mn,y.mn)。 |
| 是否lazy | 是；lazy[node]为本节点已处理、尚未传子的累计增量。 |
| apply | sum+=(r-l+1)*val；mn+=val；mx+=val；lazy+=val。 |
| pushdown | 先apply左右儿子，再lazy[node]=0；部分修改和部分查询都先下传。 |
| 查询 | 完整覆盖返回Node；部分覆盖先pushdown，再按左右顺序merge。 |

**核心代码：** [competition/data_structure/segment_range_add.hpp](competition/data_structure/segment_range_add.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,L,R,val); cout<<query(1,1,n,L,R).mn;
```

**完整调用及样例复用：** [competition/data_structure/examples/range_add.cpp](competition/data_structure/examples/range_add.cpp)，见SEG-05的完整main与输入输出。本模型取该输出的对应统计列。

**最容易出错：** 查询部分覆盖也必须pushdown。；sum加len*x，mn/mx只加x。；build清lazy后才可多测复用；不要直接调用旧update跳过标记。

**来源/练习：** 本地练习：cf/date/6/6.9/1_special_segment_tree.cpp 的区间加/最小值局部模型。本轮可运行模型：competition/data_structure/examples/range_add.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-07 区间加、区间最大值 · PSD10

**识别信号：** 整段增减后反复询问最大负载/最大高度。

**先观察：** 整体平移不改变大小关系；单独的最大值足以快速apply。

**暴力：** 逐个改、再扫描max。

**重复计算：** max(a_i+x)=max(a_i)+x。

**必要限制：** 必要：每次整段加同一val。充分：max(a_i+val)=max(a_i)+val。 边界契约：1<=n<N；修改和查询是合法非空闭区间；a为初始输入；和、标记、长度*x及中间量在ll内。

**正确性：** 整段平移保持大小顺序，标记相加表示时间上的连续平移；下传后子树与父树状态一致。

**复杂度：** 建树O(n)，区间修改/查询O(log n)，空间O(n)。

**怎样推出：** 复用区间加的mx；与sum不同，mx只加x，不能乘区间长度。

| 维护项 | 具体规则 |
|---|---|
| Node | mx（本实现同时保留sum,mn）。 |
| pushup | mx=max(x.mx,y.mx)。 |
| 是否lazy | 是；lazy[node]为本节点已处理、尚未传子的累计增量。 |
| apply | sum+=(r-l+1)*val；mn+=val；mx+=val；lazy+=val。 |
| pushdown | 先apply左右儿子，再lazy[node]=0；部分修改和部分查询都先下传。 |
| 查询 | 完整覆盖返回Node；部分覆盖先pushdown，再按左右顺序merge。 |

**核心代码：** [competition/data_structure/segment_range_add.hpp](competition/data_structure/segment_range_add.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,L,R,val); cout<<query(1,1,n,L,R).mx;
```

**完整调用及样例复用：** [competition/data_structure/examples/range_add.cpp](competition/data_structure/examples/range_add.cpp)，见SEG-05的完整main与输入输出。本模型取该输出的对应统计列。

**最容易出错：** 查询部分覆盖也必须pushdown。；sum加len*x，mn/mx只加x。；build清lazy后才可多测复用；不要直接调用旧update跳过标记。

**来源/练习：** 本地练习：cf/date/6/6.9/1_special_segment_tree.cpp 的区间加/最小值局部模型。本轮可运行模型：competition/data_structure/examples/range_add.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-08 区间赋值、区间和 · PSD11

**识别信号：** 把一整段全部覆盖成同一个值，再查询和。

**先观察：** 覆盖会抹去之前所有修改；0也是一个有效赋值。

**暴力：** 遍历区间每个元素赋新值。

**重复计算：** 覆盖后的和只取决于长度和值，为len*x。

**必要限制：** 必要：一次操作将段内全部元素覆盖成同一val。充分：len*val确定新和，has_set记录赋值0。 边界契约：1<=n<N；合法非空闭区间；true表示赋值、false表示加；所有中间量在ll内；允许赋值0。

**正确性：** 覆盖后每个元素均为val，所以整段和为len*val；下传覆盖会正确抹除儿子的旧信息。

**复杂度：** 建树O(n)，修改/查询O(log n)，空间O(n)。

**怎样推出：** 用has_set表示有没有赋值，不能拿set==0当无标记；复用混合骨架时始终传is_set=true。

| 维护项 | 具体规则 |
|---|---|
| Node | tree为ll区间和；lazy含has_set/set/add，本变体add保持0。 |
| pushup | tree[node]=tree[left]+tree[right]。 |
| 是否lazy | 是；has_set独立于数值0，set/add分别记录赋值和后续增量。 |
| apply | is_set=true：tree=len*val，lazy={true,val,0}；否则tree+=len*val，lazy.add+=val。 |
| pushdown | 若has_set先给左右孩子赋set，再给左右孩子加add，最后lazy={}。 |
| 查询 | 完整覆盖返回tree[node]；部分覆盖先pushdown再累加相交儿子的和。 |

**核心代码：** [competition/data_structure/segment_set_add.hpp](competition/data_structure/segment_set_add.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,L,R,0,true); cout<<query(1,1,n,L,R);
```

**完整可运行调用：** [competition/data_structure/examples/range_set.cpp](competition/data_structure/examples/range_set.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_set_add.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            ll val; cin >> val;
            modify(1, 1, n, l, r, val, true);
        } else cout << query(1, 1, n, l, r) << '\n';
    }
}
```

输入格式：1 l r val赋值；2 l r查询。

输入：
```text
3 4
1 2 3
2 1 3
1 1 3 0
1 2 2 5
2 1 3
```
期望输出：
```text
6
5
```

**最容易出错：** 不能用set!=0判断有无赋值，set=0也是有效操作。；set(v)后add(d)得到v+d；add(d)后set(v)只剩v。；下传先set再add，新的set必须清掉旧add。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/range_set.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-09 区间赋值与区间加混合 · PSD11

**识别信号：** 题目同时有整段覆盖、整段增加和区间求和。

**先观察：** 操作顺序是否可交换：set(5)再add(2)=7，add(2)再set(5)=5。

**暴力：** 逐元素按时间顺序执行每次操作。

**重复计算：** 一串操作可压成：可选最后一次赋值，再加其后的累计增量。

**必要限制：** 必要：操作只有赋值与加法，按输入时间顺序执行。充分：任意操作序列能压成可选set后累计add。 边界契约：1<=n<N；合法非空闭区间；true表示赋值、false表示加；所有中间量在ll内；允许赋值0。

**正确性：** 新set覆盖全部旧操作，新add接在旧操作后；下传先set后add，等价于原时间顺序。

**复杂度：** 建树O(n)，修改/查询O(log n)，空间O(n)。

**怎样推出：** 标签T=(has_set,set,add)表示先set后add；compose(old,new)即先old后new。新set直接覆盖旧T；新add只累加到old.add。

| 维护项 | 具体规则 |
|---|---|
| Node | tree区间和；Tag{has_set,set,add}。 |
| pushup | tree[node]=tree[left]+tree[right]。 |
| 是否lazy | 是；has_set独立于数值0，set/add分别记录赋值和后续增量。 |
| apply | is_set=true：tree=len*val，lazy={true,val,0}；否则tree+=len*val，lazy.add+=val。 |
| pushdown | 若has_set先给左右孩子赋set，再给左右孩子加add，最后lazy={}。 |
| 查询 | 完整覆盖返回tree[node]；部分覆盖先pushdown再累加相交儿子的和。 |

**核心代码：** [competition/data_structure/segment_set_add.hpp](competition/data_structure/segment_set_add.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,L,R,0,true); modify(1,1,n,L,R,5,false); cout<<query(1,1,n,L,R);
```

**完整可运行调用：** [competition/data_structure/examples/set_add.cpp](competition/data_structure/examples/set_add.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_set_add.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op <= 2) {
            ll val; cin >> val;
            modify(1, 1, n, l, r, val, op == 1);
        } else cout << query(1, 1, n, l, r) << '\n';
    }
}
```

输入格式：1 l r val赋值；2 l r delta增加；3 l r查询。

输入：
```text
4 6
1 2 3 4
1 1 4 0
2 2 3 5
3 1 4
2 1 4 2
1 2 3 0
3 1 4
```
期望输出：
```text
10
4
```

**最容易出错：** 不能用set!=0判断有无赋值，set=0也是有效操作。；set(v)后add(d)得到v+d；add(d)后set(v)只剩v。；下传先set再add，新的set必须清掉旧add。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/set_add.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-10 区间最大值及次数 · PSD06

**识别信号：** 问最大值是多少，以及恰好等于这个最大值的元素有多少。

**先观察：** 次数依赖最大值本身，不能独立相加所有儿子次数。

**暴力：** 扫描区间求max，再统计相等个数。

**重复计算：** 较小最大值一侧不贡献；两侧最大值相等才合并次数。

**必要限制：** 必要：cnt只统计等于区间mx的元素。充分：比较两侧mx后仅保留达父最大值一侧的cnt，相等才相加。 边界契约：沿用PSD02边界；单个叶cnt=1；cnt不超过n；值可为任意ll。

**正确性：** 所有最大值只可能来自最大值等于父最大值的子段。

**复杂度：** 建树O(n)，修改/查询O(log n)，空间O(n)。

**怎样推出：** Node保存mx,cnt；三种比较分支构成准确的merge。

| 维护项 | 具体规则 |
|---|---|
| Node | mx：最大值；cnt：该最大值出现次数。 |
| pushup | x.mx>y.mx返回x；小于返回y；相等返回{mx,x.cnt+y.cnt}。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_max_count.hpp](competition/data_structure/segment_max_count.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
struct Node { ll mx; int cnt; };
Node make_node(ll val) { return {val, 1}; }
Node merge(const Node& x, const Node& y) {
    if (x.mx > y.mx) return x;
    if (x.mx < y.mx) return y;
    return {x.mx, x.cnt + y.cnt};
}
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); auto ans=query(1,1,n,L,R); cout<<ans.mx<<' '<<ans.cnt;
```

**完整可运行调用：** [competition/data_structure/examples/max_count.cpp](competition/data_structure/examples/max_count.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_max_count.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.mx << ' ' << ans.cnt << '\n';
        }
    }
}
```

输入格式：2 l r输出最大值和次数；1 pos val赋值。

输入：
```text
4 3
5 5 1 5
2 1 4
1 2 0
2 1 4
```
期望输出：
```text
5 3
5 2
```

**最容易出错：** 子段最大值不等时不能把两边次数相加。；点改叶子必须把cnt重置为1。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/max_count.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-11 最大连续子段和 · PSD07

**识别信号：** 点改后，问某区间内部收益最大的连续一段。

**先观察：** 答案可能跨左右分界；题意是否允许选空段决定全负时的输出。

**暴力：** 枚举子段左右端点并求和，O(n²)到O(n³)。

**重复计算：** 跨中点的段必为左侧某后缀加右侧某前缀；只记best不够。

**必要限制：** 必要：单点改值、连续且默认非空子段。充分：sum/pref/suff/best覆盖全左、全右、跨界三类候选。 边界契约：非空子段；全负返回最大负数。sum/pref/suff/best及相加中间量均在ll内；无任意区间加。

**正确性：** 任一最优子段恰好属于全在左、全在右、跨中点三类之一。

**复杂度：** 建树O(n)，修改/查询O(log n)，空间O(n)。

**怎样推出：** 保存sum/pref/suff/best；按三类：全左、全右、跨中点合并。查询仍保持左右顺序。

| 维护项 | 具体规则 |
|---|---|
| Node | sum总和；pref最大非空前缀和；suff最大非空后缀和；best最大非空子段和。 |
| pushup | sum=x.sum+y.sum；pref=max(x.pref,x.sum+y.pref)；suff=max(y.suff,y.sum+x.suff)；best=max(x.best,y.best,x.suff+y.pref)。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_max_subarray.hpp](competition/data_structure/segment_max_subarray.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
// 非空连续子段：全负时返回最大的那个负数。
struct Node { ll sum, pref, suff, best; };
Node make_node(ll val) { return {val, val, val, val}; }
Node merge(const Node& x, const Node& y) {
    return {x.sum + y.sum,
            max(x.pref, x.sum + y.pref),
            max(y.suff, y.sum + x.suff),
            max({x.best, y.best, x.suff + y.pref})};
}
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); cout<<query(1,1,n,L,R).best;
```

**完整可运行调用：** [competition/data_structure/examples/max_subarray.cpp](competition/data_structure/examples/max_subarray.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_max_subarray.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.best << '\n';
        }
    }
}
```

输入格式：2 l r输出非空答案；如允许空段仅将输出改为max(0LL,ans.best)。

输入：
```text
4 3
-5 -2 -7 -1
2 1 4
1 2 6
2 1 4
```
期望输出：
```text
-1
6
```

**最容易出错：** 只存sum与best无法计算跨中点最优值。；全负结果是最大负数；允许空段时应在输出取max(0LL,best)。；任意区间加不能只修改这四个字段。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/max_subarray.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-12 非负频率第k个 · PSD08

**识别信号：** 维护每个值的出现次数，要找排序后的第k个元素/第k个可用位置。

**先观察：** 位置是自然顺序还是离散化值序；所有频率必须非负。

**暴力：** 从左往右累计频率，直到累计值达到k。

**重复计算：** 整棵左子树的总频率告诉我们前多少个元素可以一次跳过。

**必要限制：** 必要：每个频率非负，k以1为起点，合法时不超过总频率。充分：前缀频率单调且左右子树总量准确。 边界契约：所有频率非负，总频率在ll内；k从1开始；k<=0或k>总量返回-1；modify为频率赋值。

**正确性：** 非负性保证前缀和单调，左段包含的正是最前面的left.sum个元素。

**复杂度：** 建树O(n)，修改/query/kth均O(log n)，空间O(n)。

**怎样推出：** k<=left.sum就向左，否则k-=left.sum向右；合法k需1<=k<=总频率。

| 维护项 | 具体规则 |
|---|---|
| Node | sum：区间总频率。 |
| pushup | sum=x.sum+y.sum。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | query返回总频率；kth按左子树总量下降，非法k返回-1。 |

**核心代码：** [competition/data_structure/segment_frequency.hpp](competition/data_structure/segment_frequency.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
// 每个位置存非负频率；修改是把该位置频率改成 val。
struct Node { ll sum; };
Node make_node(ll val) { assert(val >= 0); return {val}; }
Node merge(const Node& x, const Node& y) { return {x.sum + y.sum}; }
```

```cpp
// k 从 1 开始；返回第 k 个元素所在下标，非法 k 返回 -1。
// 必须保证所有频率非负，且总频率不超过 LLONG_MAX。
int kth(int node, int l, int r, ll k) {
    if (k <= 0 || k > tree[node].sum) return -1;
    if (l == r) return l;
    int mid = (l + r) >> 1;
    if (k <= tree[node * 2].sum) return kth(node * 2, l, mid, k);
    return kth(node * 2 + 1, mid + 1, r, k - tree[node * 2].sum);
}
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,new_count); cout<<kth(1,1,n,k);
```

**完整可运行调用：** [competition/data_structure/examples/frequency.cpp](competition/data_structure/examples/frequency.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_frequency.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op; cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            ll k; cin >> k;
            cout << kth(1, 1, n, k) << '\n';
        }
    }
}
```

输入格式：1 pos count把频率改为非负count；2 k查第k个，下标从1开始。

输入：
```text
4 4
0 2 0 3
2 1
2 5
1 2 0
2 1
```
期望输出：
```text
2
4
4
```

**最容易出错：** 负频率破坏前缀单调；如[2,-1,2]不能据左段总和可靠排位。；这是第k个元素所在下标；若下标来自离散化，还需反查原值。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/frequency.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-13 范围内第一个达到阈值的位置 · PSD04

**识别信号：** 从某下标开始，找第一个a[i]>=x；或要求答案只落在[L,R]。

**先观察：** 需最左位置而非任意位置；寻找范围可能不是全数组。

**暴力：** 从L扫到R，遇到第一个达标元素即返回。

**重复计算：** 某整段max<x时整段都能排除，max>=x仅说明该段存在候选。

**必要限制：** 必要：寻找条件是a[i]>=x，限定[L,R]且要求最左。充分：mx<x可排除整段，先左后右下降。 边界契约：沿用PSD02边界；first_ge允许L>R并返回-1；值可为任意ll。

**正确性：** 剪去不相交段与不达标段，按下标递增搜索，第一个叶就是最左合法位置。

**复杂度：** 建树O(n)，修改/区间查询/first_ge均O(log n)，空间O(n)。

**怎样推出：** 先剪去不相交段与mx<x的段，再先左后右下探；找到就立即返回。

| 维护项 | 具体规则 |
|---|---|
| Node | mx：判断本段有没有候选。 |
| pushup | mx=max(x.mx,y.mx)。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | first_ge剪去不相交或mx<x的段，先左后右，首次找到返回；无解或空搜索范围返回-1。 |

**核心代码：** [competition/data_structure/segment_point_max.hpp](competition/data_structure/segment_point_max.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
// 在闭区间 [L,R] 中找第一个 a[pos]>=val；不存在或 L>R 返回 -1。
int first_ge(int node, int l, int r, int L, int R, ll val) {
    if (L > R || R < l || r < L || tree[node].mx < val) return -1;
    if (l == r) return l;
    int mid = (l + r) >> 1;
    int pos = first_ge(node * 2, l, mid, L, R, val);
    if (pos != -1) return pos;
    return first_ge(node * 2 + 1, mid + 1, r, L, R, val);
}
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); cout<<first_ge(1,1,n,L,R,x);
```

**完整可运行调用：** [competition/data_structure/examples/first_ge.cpp](competition/data_structure/examples/first_ge.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_point_max.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op; cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r; ll val;
            cin >> l >> r >> val;
            cout << first_ge(1, 1, n, l, r, val) << '\n';
        }
    }
}
```

输入格式：1 pos val点赋值；2 l r x返回[l,r]内首个>=x的位置，不存在-1。

输入：
```text
4 4
8 1 8 2
2 2 4 8
2 2 2 8
1 2 9
2 2 4 8
```
期望输出：
```text
3
-1
2
```

**最容易出错：** max不能用0代表空段，全负区间会错误。；first_ge只查给定[L,R]；必须先左后右才是第一个。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/first_ge.cpp；仅承诺此处模型，不猜测缺失的原题题面。

### SEG-14 多个可合并统计量 · PSD09

**识别信号：** 同一个区间要同时问sum/min/max/gcd，或合并需要补充信息。

**先观察：** 每个字段能否只由左右同类字段确定；merge是否有顺序。

**暴力：** 每次为每项统计都扫描区间。

**重复计算：** 四个字段各自满足结合律，可共享同一棵区间树与查询划分。

**必要限制：** 必要：每个统计量都能仅用左右摘要合并。充分：各字段merge满足结合律，查询顺序正确。 边界契约：沿用PSD02边界；受gcd约束禁止LLONG_MIN；所有和和中间量在ll内；不支持区间加。

**正确性：** 每个字段独立正确且查询划分相同，整组答案同时正确。

**复杂度：** 建树O(n log V)，修改/查询O(log n log V)，空间O(n)。

**怎样推出：** Node存四项并逐项merge；这个事实只证明点改可用，不证明所有字段都支持同一种lazy。

| 维护项 | 具体规则 |
|---|---|
| Node | sum,mn,mx,g；如矩阵题还需保存mat和翻转后的rev。 |
| pushup | 分别+、min、max、gcd；非交换字段必须保留左右顺序。 |
| 是否lazy | 否：只有单点赋值，不需要pushdown。 |
| apply | apply(node,val)把该叶改为make_node(val)。 |
| pushdown | 无；modify沿一条根到叶路径递归。 |
| 查询 | 合法非空闭区间；只进相交儿子，跨mid时按左、右顺序merge。 |

**核心代码：** [competition/data_structure/segment_point_stats.hpp](competition/data_structure/segment_point_stats.hpp)。它依赖的完整骨架与组合方式见上节，工程include无需手工拼接。

```cpp
// 一起维护的字段须都能从左右子段独立合并；禁止 LLONG_MIN。
struct Node { ll sum, mn, mx, g; };
Node make_node(ll val) {
    assert(val != LLONG_MIN);
    return {val, val, val, val < 0 ? -val : val};
}
Node merge(const Node& x, const Node& y) {
    return {x.sum + y.sum, min(x.mn, y.mn), max(x.mx, y.mx), gcd(x.g, y.g)};
}
```

**调用定位：**
```cpp
build(1,1,n); modify(1,1,n,pos,val); auto ans=query(1,1,n,L,R); cout<<ans.sum<<' '<<ans.mn<<' '<<ans.mx<<' '<<ans.g;
```

**完整可运行调用：** [competition/data_structure/examples/point_stats.cpp](competition/data_structure/examples/point_stats.cpp)。工程保持include；纸面粘贴已选的完整实现后，删除这个本地include即可。

```cpp
#include "../segment_point_stats.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.sum << ' ' << ans.mn << ' ' << ans.mx
                 << ' ' << ans.g << '\n';
        }
    }
}
```

输入格式：2 l r输出sum min max gcd；1 pos val赋值。

输入：
```text
4 3
6 -12 9 3
2 1 4
1 2 15
2 1 3
```
期望输出：
```text
6 -12 9 3
30 6 15 3
```

**最容易出错：** 可合并不等于可做lazy；gcd不支持从gcd本身处理区间加。；矩阵等非交换merge必须保留左右顺序。

**来源/练习：** 本地练习：cf/segment_tree_cf_edu/1.cpp 的递归骨架变体。本轮可运行模型：competition/data_structure/examples/point_stats.cpp；仅承诺此处模型，不猜测缺失的原题题面。

## Fenwick 完整调用

```cpp
#include "../fenwick.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    Fenwick fen(n);
    for (int i = 1; i <= n; ++i) { ll x; cin >> x; fen.add(i, x); }
    while (q--) {
        int op, l, r; ll val;
        cin >> op;
        if (op == 1) { cin >> l >> val; fen.add(l, val); }
        else if (op == 2) { cin >> l >> r; cout << fen.rangeSum(l, r) << '\n'; }
        else { cin >> r; cout << fen.query(r) << '\n'; }
    }
}
```

输入：
```text
3 4
3 -2 4
2 1 3
1 2 5
3 2
2 2 3
```
期望输出：
```text
5
6
7
```

## 原代码继承与最小改动

以下路径按cf仓库根目录定位。template/Segment tree.cpp不存在，实际旧单点树文件为template/segmen tree1.cpp。

### template/Fenwick_tree.cpp

**直接继承：** 直接保留Fenwick结构、n、tr、add和lowbit循环。

**原问题/限制：** 定义query(l,r)，main却调用不存在的rangeSum(l,r)，不能编译。

**最小改动：** 只统一接口：sum(x)改名query(x)，旧query(l,r)改名rangeSum(l,r)；size参数避免-Wshadow；加合法调用断言。

**接口变化：** add(x,delta)不变；query(x)为前缀，rangeSum(l,r)为区间。没有query(l,r)重载。

**在线证据：** UNKNOWN

### template/segmen tree1.cpp

**直接继承：** 全局a/tree、1下标和pushup/build/递归单点修改结构直接继承。

**原问题/限制：** 原结构可用；原文件名就是segmen tree1.cpp，没有Segment tree.cpp。

**最小改动：** 保留单点递归骨架并将update改名modify；PSD02保持sum，PSD03–09分别替换具体Node/merge，阈值搜索、gcd、次数、最大子段和、第k个等为本轮在骨架上的扩展。查询只进入相交儿子。

**接口变化：** 各模块query返回各自Node：PSD02/08取.sum，PSD03取.mn，PSD04取.mx，PSD05取.g，PSD06取.mx/.cnt，PSD07取.best，PSD09取.sum/.mn/.mx/.g。

**在线证据：** UNKNOWN

### cf/segment_tree_cf_edu/1.cpp

**直接继承：** 沿用用户的node/l/r/mid、左右儿子编号和单点赋值递归。

**原问题/限制：** 代码已由用户使用验证；本地未核验在线提交。

**最小改动：** 输入0下标[l,r)的转换只保留在来源说明，正式接口统一用户熟悉的1下标闭区间。

**接口变化：** 来源update更名modify；示例直接输入1下标闭区间。

**在线证据：** UNKNOWN

### cf/segment_tree_cf_edu/2.cpp

**直接继承：** 直接继承min合并；来源为用户的区间最小值练习。

**原问题/限制：** 原题常规值域下的4e18空段哨兵可直接使用；扩展到整个ll值域时，大于4e18的元素会被该有限哨兵污染。

**最小改动：** 为共用无空节点的Node骨架，查询只进入相交儿子，不再依赖有限INF；不否定旧代码在原约束下的使用验证。

**接口变化：** query返回.mn，update更名modify。

**在线证据：** UNKNOWN

### template/Segment tree2.cpp

**直接继承：** 全局a/tree/lazy；apply、pushdown、modify按原递归结构保留。

**原问题/限制：** apply在声明前使用；tree/query用int；query部分覆盖未pushdown；build未清旧lazy。

**最小改动：** 公共修复为apply声明顺序、ll和/答案、查询下传、build清标记。PSD10扩展Node{sum,mn,mx}；PSD11保留ll区间和，把lazy扩展为Tag{has_set,set,add}，新set覆盖旧标记，下传先set再add；test_set_add验证赋值0、操作先后、多测和随机差分。

**接口变化：** PSD10的query返回Node取.sum/.mn/.mx；PSD11的query直接返回ll，modify(...,val,is_set)以true表示赋值、false表示加。

**在线证据：** UNKNOWN

### cf/date/6/6.9/1_special_segment_tree.cpp

**直接继承：** 直接继承区间加、维护最小值的apply/pushdown/build/modify写法。

**原问题/限制：** 原题维护d[x]=x-count(<=x)及最小位置pos，不能把它当通用数组问题完整题解。

**最小改动：** PSD10沿用区间加与min维护并加入sum/max。PSD11只沿用加法递归和build清标记写法，聚合为ll区间和；has_set与set/add组合是本轮新增，原题mn/pos不搬入。

**接口变化：** 原pos只作题目专用见证；PSD10返回统计Node，PSD11返回ll和并用is_set区分赋值与加法。

**在线证据：** UNKNOWN

### cf/date/6/6.15/1_segment_tree.cpp

**直接继承：** 读取完整原代码；Node保存mat和A/B互换后的rev，区间翻转swap二者、lazy异或；列向量先左后右使父矩阵为right*left。

**原问题/限制：** 原代码为单次建树的具体题目，未声称支持多测；没有实际缺陷证据需要重写。

**最小改动：** 原代码保持原样。仅作为多信息维护与非交换merge的来源讲解，不冒充本轮新增通用矩阵模板。

**接口变化：** 无。具体原程序的seg/pull/push/update命名只在来源说明保留；本轮正式树主版本用tree/pushup/pushdown/modify。

**在线证据：** UNKNOWN

旧区间加树的最小反例是[0,0]全段+1后询问[1,1]：若query不pushdown会读到0，正确值为1。旧最小值树使用4e18作为空段值，若所有值大于4e18则会污染部分查询；当前公共查询不访问不相交儿子，消除了此类哨兵需求。

## 验证与复现

所有正式变体都是真实编译代码；13个完整main覆盖14种维护模型，其中区间加的sum/min/max共用一个main。验证包括固定种子20261024随机差分、n=1/n=200000、全0/全负、重复最大值、负数gcd、频率非法k、搜索范围、set0、set-add、add-set、多层标记与重建。PSD01/PSD02/PSD10与ROUND01 DS01/DS02/DS03使用同一输入流，同时与独立暴力比较；最大子段oracle枚举所有端点。

执行目录为template仓库根目录：
```text
g++ -std=gnu++20 -O2 -Wall -Wextra -Wshadow tests/round02/test_segment.cpp -o tests/round02/test_segment.exe
tests/round02/test_segment.exe
python competition/data_structure/verify_examples.py
```

WSL Sanitizer：
```text
wsl -d Ubuntu -- bash -lc 'cd /mnt/c/Users/TSOU/Desktop/cf/template && g++ -std=gnu++20 -O1 -g -Wall -Wextra -Wshadow -fsanitize=address,undefined -fno-omit-frame-pointer tests/round02/test_segment.cpp -o /tmp/r2_segment_san && /tmp/r2_segment_san'
```

当前结果：PASS Windows GNU++20 -Wall -Wextra -Wshadow；PASS WSL ASan+UBSan；PASS 13个独立main，各自工程include与打印展开两种形式，共26次编译运行；文档I/O一致，无编译警告；UNKNOWN；PASS tests/test_core_data.cpp、test_graph.cpp、test_dp_math_string.cpp。

个人全局树不接收空树或空区间；只有Fenwick空前缀/空区间、first_ge空搜索范围与kth非法k有显式返回值。所有sum/标记/长度乘积及中间运算需处于long long范围。数学gcd可能需要2^63时应另用无符号或更宽表示，本节ll版本不接受LLONG_MIN。
