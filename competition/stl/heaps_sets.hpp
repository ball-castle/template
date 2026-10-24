#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 每次合并两份的代价是其和，所有权重非负；总和/答案不溢出ll。
inline ll huffman_cost(const vector<ll>& a) {
    priority_queue<ll, vector<ll>, greater<ll>> q(a.begin(), a.end());
    ll ans = 0;
    while (q.size() > 1) {
        ll x = q.top(); q.pop();
        ll y = q.top(); q.pop();
        ans += x + y; q.push(x + y);
    }
    return ans;
}
// 流式保留最大的k个，重复值保留；输出降序，k可超过元素数。
inline vector<ll> top_k_largest(const vector<ll>& a, int k) {
    assert(k >= 0);
    priority_queue<ll, vector<ll>, greater<ll>> q;
    for (ll x : a) {
        q.push(x);
        if (int(q.size()) > k) q.pop();
    }
    vector<ll> ans;
    while (!q.empty()) { ans.push_back(q.top()); q.pop(); }
    reverse(ans.begin(), ans.end());
    return ans;
}

// 流式保留最小的k个，用大根堆淘汰当前最大者；输出升序。
inline vector<ll> top_k_smallest(const vector<ll>& a, int k) {
    assert(k >= 0);
    priority_queue<ll> q;
    for (ll x : a) {
        q.push(x);
        if (int(q.size()) > k) q.pop();
    }
    vector<ll> ans;
    while (!q.empty()) { ans.push_back(q.top()); q.pop(); }
    reverse(ans.begin(), ans.end());
    return ans;
}

// 动态只插入的下中位数：low有ceil(n/2)个小值，high存其余。
inline priority_queue<ll> low;
inline priority_queue<ll, vector<ll>, greater<ll>> high;
inline void median_init() { low = {}; high = {}; }
inline void median_add(ll x) {
    if (low.empty() || x <= low.top()) low.push(x);
    else high.push(x);
    if (low.size() < high.size()) {
        low.push(high.top()); high.pop();
    }
    if (low.size() > high.size() + 1) {
        high.push(low.top()); low.pop();
    }
}
inline ll median_lower() { assert(!low.empty()); return low.top(); }

// 可改优先级/取消任务：旧条目留在堆里，到堆顶时按版本丢弃。
// id为1..n；相同值时较大id优先；版本次数必须能用ll表示。
inline priority_queue<tuple<ll, int, ll>> task_heap;
inline vector<ll> version;
inline vector<bool> active;
inline void lazy_init(int n) {
    assert(n >= 0);
    task_heap = {}; version.assign(n + 1, 0); active.assign(n + 1, false);
}
inline void lazy_set(int id, ll value) {
    assert(1 <= id && id < int(version.size()));
    active[id] = true;
    task_heap.emplace(value, id, ++version[id]);
}
inline void lazy_erase(int id) {
    assert(1 <= id && id < int(version.size()));
    active[id] = false;
}
inline optional<pair<ll, int>> lazy_top() {
    while (!task_heap.empty()) {
        auto [value, id, stamp] = task_heap.top();
        if (!active[id] || stamp != version[id]) task_heap.pop();
        else return pair<ll, int>{value, id};
    }
    return nullopt;
}

// 严格前驱< x与严格后继> x；不存在时返回nullopt。
inline pair<optional<ll>, optional<ll>>
strict_neighbors(const set<ll>& s, ll x) {
    optional<ll> before, after;
    auto it = s.lower_bound(x);
    if (it != s.begin()) before = *prev(it);
    it = s.upper_bound(x);
    if (it != s.end()) after = *it;
    return {before, after};
}
// 每份资源只用一次；选容量>=need的最小容量，保留更大资源。
inline optional<ll> assign_at_least(multiset<ll>& slots, ll need) {
    auto it = slots.lower_bound(need);
    if (it == slots.end()) return nullopt;
    ll value = *it; slots.erase(it); return value;
}
// 定长滑窗极差。k=0/空数组返回空；非空窗口max-min不溢出ll。
inline vector<ll> window_spread(const vector<ll>& a, int k) {
    assert(k >= 0);
    vector<ll> ans;
    if (!k || k > int(a.size())) return ans;
    multiset<ll> window;
    for (int i = 0; i < int(a.size()); ++i) {
        window.insert(a[i]);
        if (i >= k) window.erase(window.find(a[i - k]));
        if (i + 1 >= k) ans.push_back(*window.rbegin() - *window.begin());
    }
    return ans;
}
// 双multiset滑窗下中位数，支持删除；left比right多0或1个。
inline vector<ll> window_lower_medians(const vector<ll>& a, int k) {
    assert(k >= 0);
    vector<ll> ans;
    if (!k || k > int(a.size())) return ans;
    multiset<ll> left, right;
    auto balance = [&]() {
        while (left.size() > right.size() + 1) {
            auto it = prev(left.end()); right.insert(*it); left.erase(it);
        }
        while (left.size() < right.size()) {
            auto it = right.begin(); left.insert(*it); right.erase(it);
        }
    };
    for (int i = 0; i < int(a.size()); ++i) {
        if (left.empty() || a[i] <= *left.rbegin()) left.insert(a[i]);
        else right.insert(a[i]);
        balance();
        if (i >= k) {
            auto it = left.find(a[i - k]);
            if (it != left.end()) left.erase(it);
            else right.erase(right.find(a[i - k]));
            balance();
        }
        if (i + 1 >= k) ans.push_back(*left.rbegin());
    }
    return ans;
}
