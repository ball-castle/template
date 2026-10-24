#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
struct PrimResult {
    bool connected = false;
    ll weight = 0; // 不连通时为最小生成森林总权
    int components = 0;
    vector<int> parent; // 每个连通块的根为 0
};
// 点 [1,n]；g[u] 存 (v,w)，无向边双向加入；负权/重边可用。
// 端点须合法；累加过程和总权须在 ll 范围内。
inline PrimResult prim(const vector<vector<pair<int, ll>>>& g) {
    int n = (int)g.size() - 1;
    PrimResult r; r.parent.resize(n + 1);
    vector<int> vis(n + 1);
    using State = tuple<ll, int, int>; // 权、点、来自哪个父亲
    priority_queue<State, vector<State>, greater<State>> q;
    for (int s = 1; s <= n; ++s) if (!vis[s]) {
        ++r.components; q.push({0, s, 0});
        while (!q.empty()) {
            auto [w, u, p] = q.top(); q.pop();
            if (vis[u]) continue;
            vis[u] = 1; r.weight += w; r.parent[u] = p;
            for (auto [v, cost] : g[u])
                if (!vis[v]) q.push({cost, v, u});
        }
    }
    r.connected = (n > 0 && r.components == 1);
    return r;
}
} // namespace cp
