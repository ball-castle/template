# STL 高频技巧：先按操作选择，再抄最短正确写法

本文从 `notes/round02/stl_topics.json` 指向的规范源码原样抽取片段。所有片段已在 `tests/round02/test_stl.cpp` 实际执行；需要 `#include <bits/stdc++.h>`、`using namespace std;` 与 `using ll = long long;`，局部花括号放在函数体中。整数位运算示例另依赖M01；完整代码用 STL01/STL02 模块索引。

| 需要的操作 | 第一选择 | 最容易漏掉的条件 |
|---|---|---|
| 定长/动态按下标 | array / vector | reserve不是resize；中间删除会搬移 |
| 两端增删 | deque | 两端操作也可能使迭代器失效，勿长期保存 |
| 最近未处理 / 按到达处理 | stack / queue | top/front前先确认非空 |
| 只取当前最优 | priority_queue | 无有序遍历/任意位置删除 |
| 阈值、前后继、动态有序 | set / multiset | 是否需要保留重复，是否只删一个 |
| 键映射 / 同键多个值 | map / multimap | []可能插入；equal_range遍历重复键 |
| 只等值判定/计频 | unordered_set / unordered_map | 平均O(1)，最坏线性；无顺序 |

## 定长/动态顺序表

按下标访问、尾部加入。 访问O(1)，中间erase O(n)。reserve不改变size。

来源：`competition/stl/recipes.hpp:8`；模块 STL01。

```cpp
{ // array / vector：定长与动态序列。
    array<int, 4> a{3, 1, 2, 0};
    sort(a.begin(), a.end());
    assert(a.front() == 0 && a.back() == 3);
    a.fill(7); assert(a[2] == 7);
    vector<int> b; b.reserve(8); // reserve不改变size。
    b.push_back(3); b.emplace_back(4); b.resize(4, 9);
    b.pop_back(); b.erase(b.begin());
    assert((b == vector<int>{4, 9}));
    b.clear(); assert(b.empty()); // 多测清逻辑长度，容量可复用。
}
```

## 双端队列

两头插删且还要按下标取值。 两端插删O(1)，中间插删线性。

来源：`competition/stl/recipes.hpp:19`；模块 STL01。

```cpp
{ // deque：两端增删，仍可O(1)按下标访问。
    deque<int> q;
    q.push_back(2); q.push_front(1); q.push_back(3);
    q.pop_front(); q.pop_back();
    assert(q.size() == 1 && q[0] == 2);
}
```

## 组合键

将多个排序关键字绑定，字典序与结构化绑定。 pair/tuple按字段依次比较；引用绑定改值时写auto&。

来源：`competition/stl/recipes.hpp:25`；模块 STL01。

```cpp
{ // pair / tuple：字典序排序和结构化绑定。
    vector<pair<int, int>> tasks{{2, 1}, {1, 7}, {1, 3}};
    sort(tasks.begin(), tasks.end());
    auto [time, id] = tasks.front();
    auto state = make_tuple(time, id, string("ready"));
    auto [t, i, label] = state;
    assert(t == 1 && i == 3 && label == "ready");
    assert(get<1>(state) == 3);
}
```

## 栈

最近未匹配的左括号先匹配。 这是固定合法例的API示范；通用括号验证另见正式模型模块。

来源：`competition/stl/recipes.hpp:34`；模块 STL01。

```cpp
{ // stack：左括号等待最近的右括号。
    string s = "(()())";
    stack<int> left;
    bool ok = true;
    for (int i = 0; i < int(s.size()); ++i) {
        if (s[i] == '(') left.push(i);
        else if (left.empty()) { ok = false; break; }
        else left.pop();
    }
    assert(ok && left.empty());
}
```

## 队列

按被发现/到达顺序处理。 front前检查empty，pop不返回弹出值。

来源：`competition/stl/recipes.hpp:45`；模块 STL01。

```cpp
{ // queue：先进先出，BFS已发现结点的处理顺序。
    queue<int> q;
    q.push(1); q.push(2);
    vector<int> order;
    while (!q.empty()) { order.push_back(q.front()); q.pop(); }
    assert((order == vector<int>{1, 2}));
}
```

## 三种优先队列

反复拿最大/最小/自定义优先级。 push/pop O(log n)，top O(1)；cmp返回true表示a比b优先级低。

来源：`competition/stl/recipes.hpp:52`；模块 STL01。

```cpp
{ // 三种常用堆，以及结构体自定义优先级。
    priority_queue<ll> big;
    priority_queue<ll, vector<ll>, greater<ll>> small;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>,
                   greater<pair<ll, int>>> pairs;
    for (ll x : {2LL, 8000000000LL, 3LL}) { big.push(x); small.push(x); }
    assert(big.top() == 8000000000LL && small.top() == 2);
    pairs.push({4000000000LL, 9}); pairs.push({4000000000LL, 3});
    assert(pairs.top() == make_pair(4000000000LL, 3));
    struct Job { int deadline, id; };
    auto cmp = [](const Job& a, const Job& b) {
        return tie(a.deadline, a.id) > tie(b.deadline, b.id);
    };
    priority_queue<Job, vector<Job>, decltype(cmp)> pq(cmp);
    pq.push({4, 1}); pq.push({2, 9}); pq.push({2, 3});
    assert(pq.top().deadline == 2 && pq.top().id == 3);
}
```

## 有序唯一/多重集合

前后继、阈值、保留重复值。 用成员lower_bound；multiset按值erase删除全部，按迭代器只删一份。

来源：`competition/stl/recipes.hpp:69`；模块 STL01。

```cpp
{ // set去重；multiset保留重复；删除一个要用迭代器。
    set<int> s{3, 1, 3}; assert(s.size() == 2);
    assert(s.contains(1) && !s.contains(2)); // C++20等值存在性。
    assert(s.find(1) != s.end() && s.find(9) == s.end());
    s.insert(2); assert(s.count(2) == 1);
    assert(s.erase(2) == 1); // set无重复；按值删除返回删除数。
    auto it = s.lower_bound(2); assert(it != s.end() && *it == 3);
    multiset<int> ms{2, 2, 5};
    auto pos = ms.find(2);
    if (pos != ms.end()) ms.erase(pos);
    assert(ms.count(2) == 1); // ms.erase(2)会删所有2。
    ms.insert(2);
    assert(ms.erase(2) == 2 && !ms.contains(2)); // 按值删全部副本。
}
```

## 有序键值与重复键

计频或同一时间多个事件。 map[]在键不存在时插入默认值；只查用find/contains/at。multimap用equal_range。

来源：`competition/stl/recipes.hpp:83`；模块 STL01。

```cpp
{ // map计频；multimap存同一时间的多项事件。
    map<string, int> cnt;
    for (string x : {"b", "a", "b"}) ++cnt[x];
    assert(cnt.begin()->first == "a" && cnt.at("b") == 2);
    assert(cnt.find("c") == cnt.end()); // 查询不想插入时不用[]。
    multimap<int, string> events{{2, "B"}, {1, "A"}, {2, "C"}};
    auto [l, r] = events.equal_range(2);
    int total = 0;
    for (auto it = l; it != r; ++it) ++total;
    assert(total == 2);
}
```

## 哈希集合与映射

只需要等值查询/计频。 平均O(1)最坏O(n)，没有顺序；reserve与load_factor只控制桶容量，不保证防碰撞。

来源：`competition/stl/recipes.hpp:94`；模块 STL01。

```cpp
{ // 哈希去重/计频；只要等值查询，不要求前驱或排序。
    unordered_set<int> seen;
    unordered_map<int, int> cnt;
    seen.max_load_factor(0.7f); seen.reserve(100);
    cnt.max_load_factor(0.7f); cnt.reserve(100);
    for (int x : {4, 4, -1}) { seen.insert(x); ++cnt[x]; }
    assert(seen.size() == 2 && cnt.at(4) == 2);
}
```

## 自定义排序与稳定排序

按分数降序，或同分保留原相对次序。 cmp必须严格弱序，不能<=，不能混EPS；emplace_back不保证总比push_back快。

来源：`competition/stl/recipes.hpp:105`；模块 STL01。

```cpp
{ // 自定义结构体排序；同分按id升序，必须严格小于。
    struct Node { int score, id; };
    vector<Node> a{{9, 2}, {7, 1}, {9, 1}};
    sort(a.begin(), a.end(), [](const Node& x, const Node& y) {
        if (x.score != y.score) return x.score > y.score;
        return x.id < y.id;
    });
    assert(a[0].id == 1 && a[1].id == 2);
    stable_sort(a.begin(), a.end(), [](const Node& x, const Node& y) {
        return x.score < y.score;
    });
    assert(a[0].id == 1 && a[1].id == 1 && a[2].id == 2);
}
```

## 升降序的二分边界

统计某值出现次数或第一个满足阈值的位置。 同一序关系排序与查询；升序lower是>=，upper是>；降序lower是<=，upper是<。

来源：`competition/stl/recipes.hpp:118`；模块 STL01。

```cpp
{ // 升序找等值段；降序必须传同一比较器。
    vector<int> a{1, 2, 2, 5};
    auto l = lower_bound(a.begin(), a.end(), 2);
    auto r = upper_bound(a.begin(), a.end(), 2);
    assert(r - l == 2 && binary_search(a.begin(), a.end(), 5));
    sort(a.begin(), a.end(), greater<int>());
    auto dl = lower_bound(a.begin(), a.end(), 2, greater<int>());
    auto dr = upper_bound(a.begin(), a.end(), 2, greater<int>());
    assert(dl - a.begin() == 1 && dr - a.begin() == 3);
    assert(binary_search(a.begin(), a.end(), 5, greater<int>()));
}
```

## 第k小

只想知道某个秩，不需完整排序。 平均线性；k为0-based且0<=k<n；其他位置并未整体有序。

来源：`competition/stl/recipes.hpp:129`；模块 STL01。

```cpp
{ // 第k小只需分隔，不要求其余位置有序；k为0-based。
    vector<int> a{7, 1, 4, 4, 9};
    int k = 2;
    nth_element(a.begin(), a.begin() + k, a.end());
    assert(a[k] == 4);
    for (int i = 0; i < k; ++i) assert(a[i] <= a[k]);
    for (int i = k + 1; i < int(a.size()); ++i) assert(a[i] >= a[k]);
}
```

## 去重与批量删除

排序后去重，或按谓词删除所有命中元素。 unique仅压相邻重复，remove只搬移，最后必须erase尾部。

来源：`competition/stl/recipes.hpp:137`；模块 STL01。

```cpp
{ // unique只压相邻重复；erase才实际缩短容器。
    vector<int> a{3, 1, 3, 2, 2};
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    assert((a == vector<int>{1, 2, 3}));
    a.erase(remove_if(a.begin(), a.end(),
        [](int x) { return x % 2; }), a.end());
    assert((a == vector<int>{2}));
}
```

## 反转、循环移位、排列

对称构造、旋转、枚举不同排列。 rotate区间半开；next_permutation先排序并do-while；最多n!，重复字符不重复枚举。

来源：`competition/stl/recipes.hpp:146`；模块 STL01。

```cpp
{ // 反转/循环左移；排列枚举必须包含初始排列。
    vector<int> a{1, 2, 3, 4};
    reverse(a.begin(), a.end());
    rotate(a.begin(), a.begin() + 1, a.end());
    assert((a == vector<int>{3, 2, 1, 4}));
    string p = "aab";
    int ways = 0;
    do { ++ways; } while (next_permutation(p.begin(), p.end()));
    assert(ways == 3); // 初始有序时，重复元素不会重复枚举。
    assert(prev_permutation(p.begin(), p.end()) == false);
}
```

## 累计、前后缀和、极值

多次查前缀/后缀信息，枚举中间点两边合并。 accumulate初值0LL；partial_sum输入元素也用ll；空区间min/max返回end，不可解引用。

来源：`competition/stl/recipes.hpp:157`；模块 STL01。

```cpp
{ // 64位前缀/后缀；partial_sum的内部类型取决于输入类型。
    vector<long long> a{2000000000LL, 2000000000LL, -1};
    auto sum = accumulate(a.begin(), a.end(), 0LL);
    vector<long long> pre(a.size()), suf(a.size());
    partial_sum(a.begin(), a.end(), pre.begin());
    partial_sum(a.rbegin(), a.rend(), suf.rbegin());
    assert(sum == 3999999999LL && pre[1] == 4000000000LL);
    assert(suf[1] == 1999999999LL);
    assert(*min_element(a.begin(), a.end()) == -1);
    assert(*max_element(a.begin(), a.end()) == 2000000000LL);
}
```

## 递归lambda

局部回溯带上下文，不另开全局函数。 显式返回类型和self参数；可读性优先，深递归仍会爆栈。

来源：`competition/stl/recipes.hpp:168`；模块 STL01。

```cpp
{ // 递归lambda：参数self传自身，枚举每项选/不选。
    vector<int> a{1, 3}, sums;
    auto dfs = [&](auto&& self, int i, int sum) -> void {
        if (i == int(a.size())) { sums.push_back(sum); return; }
        self(self, i + 1, sum);
        self(self, i + 1, sum + a[i]);
    };
    dfs(dfs, 0, 0); sort(sums.begin(), sums.end());
    assert((sums == vector<int>{0, 1, 3, 4}));
}
```

## 字符串查找、截取与大小写

查某个模式第一次出现的位置，截取固定长度并转换ASCII字符。 find失败等于string::npos；substr第二参数是长度；tolower/toupper先转unsigned char，按字节而非UTF-8汉字。

来源：`competition/stl/recipes.hpp:180`；模块 STL01。

```cpp
inline void stl_string_recipes() {
    string s = "abC abC";
    size_t pos = s.find("abC", 1);
    assert(pos != string::npos && pos == 4);
    string part = s.substr(pos, 3); // 第二参数是长度，不是右端点。
    reverse(part.begin(), part.end());
    assert(part == "Cba");
    for (char& c : part) c = char(tolower((unsigned char)c));
    assert(part == "cba");
    for (char& c : part) c = char(toupper((unsigned char)c));
    assert(part == "CBA"); // 按字节/当前locale；不处理UTF-8汉字。
    assert(s.find("xyz") == string::npos);
}
```

## 整数、位运算与子集枚举

最低位、位数、前导/后缀零、mask子集/补集与负数除模。 引用M01；GNU ctz/clz对0未定义，C++20对应count函数对0返回位宽。补集限制在full中，位移量<64。floor/ceil除法另引PM03，余数归一化引PM05。

来源：`tests/round02/test_stl.cpp:107`；模块 M01。

```cpp
void test_integer_reference_recipes() {
    assert(gcd_abs(-6, 4) == 2);
    assert(lcm_checked(6, 4) == 12);
    assert(lowbit(12ULL) == 4 && bit_count(12ULL) == 2);
    assert(lowest_bit_index(0) == -1 && lowest_bit_index(12) == 2);
    assert(countr_zero(0ULL) == 64 && countl_zero(0ULL) == 64);
    for (unsigned long long mask = 0; mask < 256; ++mask) {
        if (mask) { // GNU builtins ctz/clz的0输入未定义，必须先判断。
            assert(__builtin_ctzll(mask) == countr_zero(mask));
            assert(__builtin_clzll(mask) == countl_zero(mask));
        }
        unsigned long long full = (1ULL << 8) - 1;
        auto complement = full ^ mask; // 仅在8位全集内取补集。
        assert((mask & complement) == 0 && (mask | complement) == full);
        int ways = 0;
        for (auto sub = mask; ; sub = (sub - 1) & mask) {
            assert((sub & mask) == sub);
            ++ways;
            if (sub == 0) break;
        }
        assert(ways == (1 << popcount(mask)));
    }
    assert(-7 / 3 == -2 && -7 % 3 == -1); // C++向0截断。
    assert(((-7 % 3) + 3) % 3 == 2); // 正模数归一化。
}
```

## 堆与集合要能解决题，不只记API

| 题意识别信号 | 调用 / 不变量 | 证明与反例 |
|---|---|---|
| 任意两份合并，成本为和 | `huffman_cost(a)`，每次最小两份 | 最小叶放最深兄弟，收缩归纳；相邻合并不适用 |
| 流式最大/最小k项 | `top_k_largest(a,k)`用小根堆；`top_k_smallest(a,k)`用大根堆 | 淘汰最差候选；删旧值需换结构 |
| 在线下中位数 | `median_init/add/lower`，小半比大半多0或1 | 分界就是第ceil(n/2)小；平均需另算 |
| 任务修改/取消后拿最优 | `lazy_init/set/erase/top`，有效版本 | 旧记录到顶再清，空间按总操作数 |
| 严格前驱/后继 | `strict_neighbors(s,x)` | lower前一个/upper；不能退begin或解引用end |
| 每份资源一次且容量>=需求 | `assign_at_least(slots,need)` | 选最小够用保留大资源；额外上界需重证 |
| 定长窗口极差/中位数 | `window_spread/window_lower_medians` | 插新删一份旧值；重复值不等于去重 |

这些模型的规范实现是 `competition/stl/heaps_sets.hpp`（STL02）；完整识别、暴力、推导、条件、证明、反例与练习见 `notes/round02/cards/stl.json`。Huffman对所有小规模合并方案穷举，TopK/中位数/延迟堆/窗口均与直接扫描或排序比较。

## 失效、多测与复杂度

vector扩容会使全部旧元素指针、引用和迭代器失效；未扩容的插入使插入点及其后的迭代器/引用失效，erase使删除点及其后失效。遍历中删除应使用erase返回的新迭代器，或收集完成后统一删。[C++工作草案：vector修饰操作](https://eel.is/c++draft/vector.modifiers)。

unordered容器rehash使迭代器失效；不依赖其遍历顺序。其复杂度应写平均O(1)、最坏O(n)，不能把reserve当作最坏情况保证。[C++工作草案：无序容器](https://eel.is/c++draft/unord.req)。

比较器必须保持严格弱序；降序数据必须给二分相同的比较器。set的成员lower_bound是对数搜索，通用lower_bound在非随机访问迭代器上可能线性前进。[C++工作草案：排序与查找](https://eel.is/c++draft/alg.sorting)。

accumulate累加类型由初值决定；partial_sum中间类型与输入元素有关，单把输出数组换成ll不够。[C++工作草案：数值算法](https://eel.is/c++draft/numeric.ops)。

多测用局部容器最直接；复用全局容器时clear/重新赋空，堆用`q={}`，Trie清零已用节点。vector.clear保留容量供下组复用。不能只清答案却忘记计数、lazy版本或已访问标记。

粗估时间先写操作量：n=2e5时n²约4e10通常不可行；n log n约数百万次比较通常更合适，但常数、内存、语言与时限仍需实测。数组连续访问通常更省常数；map/set每节点有额外内存，固定小值域优先计数数组。

## 整数、输出与已有模块复用

- M01：`gcd_abs`、`lcm_checked`、`lowbit`、`bit_count`、`lowest_bit_index`，原文件直接引用。lcm先除gcd再乘，结果仍需确认能表示。
- PM03：`floor_div(a,b)`、`ceil_div(a,b)`用于正负整除，b非0且排除商超ll。C++的`/`向0截断，不等于向下取整。PM05提供模归一化与同余。
- CO02：`to_string_i128`输出完整i128，包括最小负值。乘法应先将一侧转i128，不能在ll溢出后再转。特殊大整数输入先用cin读十进制字符串，再按明确范围解析；本轮不扩展原read。
- IO01/IO02：原read按题目n/T次数读常规整数，其他情况cin。关闭同步后同一输入/输出流勿混用。

## 补丁抽取约定

短补丁优先容器选择表、三类堆、set/multiset单删、map/multimap、升降序bounds、unique/erase和64位累加。所有代码块的完整起止行均在`notes/round02/stl_topics.json`；V2保留完整STL01与STL02。正文与打印仅引用/抽取规范代码，不另维护实现。
