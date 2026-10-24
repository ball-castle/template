#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// 无向树：点 [1,n]，双向邻接，端点合法且对称由调用方保证。
// Euler 序仅入点一次；子树对应闭区间 [tin[u],tout[u]]。
struct EulerTree {
    int n;
    bool ok = false;
    vector<int> tin, tout, parent, order; // order[1..n]，order[0]=0
    explicit EulerTree(const vector<vector<int>>& g, int root = 1)
        : n((int)g.size() - 1) {
        if (n < 1 || root < 1 || root > n) return;
        ll count = 0;
        for (int u = 1; u <= n; ++u) count += (ll)g[u].size();
        if (count != 2LL * (n - 1)) return;
        tin.resize(n + 1); tout.resize(n + 1); parent.resize(n + 1);
        vector<pair<int, int>> st{{root, 0}};
        order = {0, root}; tin[root] = 1;
        int timer = 1;
        while (!st.empty()) {
            auto& [u, i] = st.back();
            if (i == (int)g[u].size()) {
                tout[u] = timer; st.pop_back(); continue;
            }
            int v = g[u][i++];
            if (tin[v]) continue;
            parent[v] = u; tin[v] = ++timer;
            order.push_back(v); st.push_back({v, 0});
        }
        ok = (timer == n);
    }
    int subtree_size(int u) const {
        return ok && u >= 1 && u <= n ? tout[u] - tin[u] + 1 : 0;
    }
    // 简单树 DP：逆序合并子状态；权值/中间和需在 ll 范围内。
    vector<ll> subtree_sum(const vector<ll>& value) const {
        if (!ok || (int)value.size() != n + 1) return {};
        vector<ll> sum = value;
        for (int i = n; i >= 1; --i) {
            int u = order[i];
            if (parent[u]) sum[parent[u]] += sum[u];
        }
        return sum;
    }
};
} // namespace cp
