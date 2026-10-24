#pragma once
#include <bits/stdc++.h>
using namespace std;

// n 个元素的 mex 在 [0,n]；负数和大于 n 的值不影响答案。
inline int static_mex(const vector<int>& a) {
    int n = int(a.size());
    vector<bool> vis(n + 1);
    for (int x : a) if (0 <= x && x <= n) vis[x] = true;
    for (int x = 0; x <= n; ++x) if (!vis[x]) return x;
    return n + 1; // n 个元素不可能占满 n+1 个不同值。
}
inline vector<int> prefix_mex(const vector<int>& a) {
    int n = int(a.size()), now = 0;
    vector<bool> vis(n + 1);
    vector<int> ans;
    for (int x : a) {
        if (0 <= x && x <= n) vis[x] = true;
        while (now <= n && vis[now]) ++now;
        ans.push_back(now);
    }
    return ans;
}

// 动态多重集合：全过程元素总数不超过 mex_limit。
// init(N) 建 [0,N] 缺失值；忽略区间外值；删值必须确实存在。
inline int mex_limit;
inline vector<int> cnt;
inline set<int> missing;
inline void mex_init(int limit) {
    assert(limit >= 0);
    mex_limit = limit;
    cnt.assign(limit + 1, 0);
    missing.clear();
    for (int x = 0; x <= limit; ++x) missing.insert(x);
}
inline void mex_add(int x) {
    if (x < 0 || x > mex_limit) return;
    if (cnt[x]++ == 0) missing.erase(x);
}
inline void mex_erase(int x) {
    if (x < 0 || x > mex_limit) return;
    assert(cnt[x] > 0);
    if (--cnt[x] == 0) missing.insert(x);
}
inline int mex_query() {
    assert(!missing.empty());
    return *missing.begin();
}
