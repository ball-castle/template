#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 可执行短例：每个花括号可单独抄用；assert给出具体结果。
inline void stl_container_recipes() {
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
    { // deque：两端增删，仍可O(1)按下标访问。
        deque<int> q;
        q.push_back(2); q.push_front(1); q.push_back(3);
        q.pop_front(); q.pop_back();
        assert(q.size() == 1 && q[0] == 2);
    }
    { // pair / tuple：字典序排序和结构化绑定。
        vector<pair<int, int>> tasks{{2, 1}, {1, 7}, {1, 3}};
        sort(tasks.begin(), tasks.end());
        auto [time, id] = tasks.front();
        auto state = make_tuple(time, id, string("ready"));
        auto [t, i, label] = state;
        assert(t == 1 && i == 3 && label == "ready");
        assert(get<1>(state) == 3);
    }
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
    { // queue：先进先出，BFS已发现结点的处理顺序。
        queue<int> q;
        q.push(1); q.push(2);
        vector<int> order;
        while (!q.empty()) { order.push_back(q.front()); q.pop(); }
        assert((order == vector<int>{1, 2}));
    }
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
    { // 哈希去重/计频；只要等值查询，不要求前驱或排序。
        unordered_set<int> seen;
        unordered_map<int, int> cnt;
        seen.max_load_factor(0.7f); seen.reserve(100);
        cnt.max_load_factor(0.7f); cnt.reserve(100);
        for (int x : {4, 4, -1}) { seen.insert(x); ++cnt[x]; }
        assert(seen.size() == 2 && cnt.at(4) == 2);
    }
}

inline void stl_algorithm_recipes() {
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
    { // 第k小只需分隔，不要求其余位置有序；k为0-based。
        vector<int> a{7, 1, 4, 4, 9};
        int k = 2;
        nth_element(a.begin(), a.begin() + k, a.end());
        assert(a[k] == 4);
        for (int i = 0; i < k; ++i) assert(a[i] <= a[k]);
        for (int i = k + 1; i < int(a.size()); ++i) assert(a[i] >= a[k]);
    }
    { // unique只压相邻重复；erase才实际缩短容器。
        vector<int> a{3, 1, 3, 2, 2};
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        assert((a == vector<int>{1, 2, 3}));
        a.erase(remove_if(a.begin(), a.end(),
            [](int x) { return x % 2; }), a.end());
        assert((a == vector<int>{2}));
    }
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
}

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
