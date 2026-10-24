#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// 无向图，合法点 [1,n]；每条边输入一次，允许重边和自环。
// 返回桥在原 edges 中的 0-based 下标，按输入顺序排列。
inline vector<int> bridges(int n, const vector<pair<int, int>>& edges) {
    int m = (int)edges.size();
    vector<vector<pair<int, int>>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i}); g[v].push_back({u, i});
    }
    vector<int> tin(n + 1), low(n + 1), parent(n + 1);
    vector<int> pe(n + 1, -1), next(n + 1), mark(m), st;
    int timer = 0;
    for (int s = 1; s <= n; ++s) if (!tin[s]) {
        tin[s] = low[s] = ++timer; st.push_back(s);
        while (!st.empty()) {
            int u = st.back();
            if (next[u] < (int)g[u].size()) {
                auto [v, id] = g[u][next[u]++];
                if (id == pe[u]) continue; // 只跳过同一条父边
                if (!tin[v]) {
                    parent[v] = u; pe[v] = id;
                    tin[v] = low[v] = ++timer; st.push_back(v);
                } else low[u] = min(low[u], tin[v]);
            } else {
                st.pop_back(); int p = parent[u];
                if (p) {
                    if (low[u] > tin[p]) mark[pe[u]] = 1;
                    low[p] = min(low[p], low[u]);
                }
            }
        }
    }
    vector<int> ans;
    for (int i = 0; i < m; ++i) if (mark[i]) ans.push_back(i);
    return ans;
}
} // namespace cp
