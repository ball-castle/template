#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
inline constexpr ll DIJK_INF = LLONG_MAX / 4;
struct DijkstraResult {
    bool ok = false;
    vector<ll> dist;
    vector<int> parent;
    vector<int> path(int t) const {
        if (!ok || t < 1 || t >= (int)dist.size()) return {};
        if (dist[t] == DIJK_INF) return {};
        vector<int> ans;
        for (; t; t = parent[t]) ans.push_back(t);
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
// 点 [1,n]；g[u] 存 (v,w)。支持有向、重边、零权、多源。
// 拒绝 w<0 或 w>=INF；真实距离>=INF 的点也视为不可达。
inline DijkstraResult dijkstra(
    const vector<vector<pair<int, ll>>>& g, const vector<int>& sources) {
    int n = (int)g.size() - 1;
    DijkstraResult r{false, vector<ll>(n + 1, DIJK_INF),
                     vector<int>(n + 1)};
    for (int s : sources) if (s < 1 || s > n) return r;
    for (int u = 1; u <= n; ++u) for (auto [v, w] : g[u])
        if (v < 1 || v > n || w < 0 || w >= DIJK_INF) return r;
    r.ok = true;
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> q;
    for (int s : sources) if (r.dist[s] != 0) {
        r.dist[s] = 0; q.push({0, s});
    }
    while (!q.empty()) {
        auto [du, u] = q.top(); q.pop();
        if (du != r.dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (w > DIJK_INF - du) continue;
            if (du + w < r.dist[v]) {
                r.dist[v] = du + w; r.parent[v] = u;
                q.push({r.dist[v], v});
            }
        }
    }
    return r;
}
inline DijkstraResult dijkstra(
    const vector<vector<pair<int, ll>>>& g, int s) {
    return dijkstra(g, vector<int>{s});
}
} // namespace cp
