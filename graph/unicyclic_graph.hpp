#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
struct Unicycle {
    bool ok = false;
    vector<int> cycle; // 环顺序，不重复首点；末点与首点相邻
};
// 连通简单无向基环图，点 [1,n]；每条无向边只输入一次。
// 非 n 条边、重边、自环、越界、不连通均返回 {false,{}}。
inline Unicycle unicyclic_graph(int n, const vector<pair<int, int>>& edges) {
    if (n < 3 || (int)edges.size() != n) return {};
    vector<vector<int>> g(n + 1);
    vector<int> deg(n + 1), seen(n + 1);
    set<pair<int, int>> used;
    for (auto [u, v] : edges) {
        if (u < 1 || u > n || v < 1 || v > n || u == v) return {};
        if (u > v) swap(u, v);
        if (!used.insert({u, v}).second) return {};
        g[u].push_back(v); g[v].push_back(u);
        ++deg[u]; ++deg[v];
    }
    queue<int> q; q.push(1); seen[1] = 1;
    int count = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop(); ++count;
        for (int v : g[u]) if (!seen[v]) seen[v] = 1, q.push(v);
    }
    if (count != n) return {};
    for (int u = 1; u <= n; ++u) if (deg[u] == 1) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop(); deg[u] = 0;
        for (int v : g[u]) if (deg[v] && --deg[v] == 1) q.push(v);
    }
    int start = 0, length = 0;
    for (int u = 1; u <= n; ++u) if (deg[u]) {
        if (deg[u] != 2) return {};
        if (!start) start = u;
        ++length;
    }
    if (length < 3) return {};
    Unicycle r; int prev = 0, u = start;
    for (int i = 0; i < length; ++i) {
        r.cycle.push_back(u);
        int next = 0;
        for (int v : g[u]) if (deg[v] && v != prev) { next = v; break; }
        if (!next) return {};
        prev = u; u = next;
    }
    if (u != start) return {};
    r.ok = true;
    return r;
}
} // namespace cp
