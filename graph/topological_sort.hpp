#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// 有向图，点 [1,n]，n>=1；返回空表示有环；重边计入入度。
inline vector<int> topological_sort(const vector<vector<int>>& g) {
    int n = (int)g.size() - 1;
    vector<int> in(n + 1), order;
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++in[v];
    queue<int> q;
    for (int u = 1; u <= n; ++u) if (!in[u]) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop(); order.push_back(u);
        for (int v : g[u]) if (--in[v] == 0) q.push(v);
    }
    if ((int)order.size() != n) return {};
    return order;
}
} // namespace cp
