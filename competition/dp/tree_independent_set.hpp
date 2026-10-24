#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// g、a从1开始，g为无向简单连通树；n>=1。允许不选任何点。
// 相邻点不能都选，求最大权和；sum(abs(a[i]))<2^60。
// 按原tree_dp.cpp“儿子先算，再合并父亲”的思路补完整状态。
inline ll tree_independent_set(const vector<vector<int>>& g,
                               const vector<ll>& a, int root = 1) {
    int n = int(g.size()) - 1;
    assert(n >= 1 && g.size() == a.size() && 1 <= root && root <= n);
    vector<int> fa(n + 1), order = {root};
    fa[root] = -1;
    for (int i = 0; i < int(order.size()); ++i) {
        int u = order[i];
        for (int v : g[u]) if (v != fa[u]) {
            assert(1 <= v && v <= n && fa[v] == 0);
            fa[v] = u;
            order.push_back(v);
        }
    }
    assert(int(order.size()) == n);
    vector<array<ll, 2>> dp(n + 1);
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        dp[u][1] = a[u]; // 选u；dp[u][0]不选u。
        for (int v : g[u]) if (fa[v] == u) {
            dp[u][0] += max(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    }
    return max(dp[root][0], dp[root][1]);
}

