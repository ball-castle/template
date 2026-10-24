#pragma once
#include "tree_storage.hpp"

// PG13. 无权树直径：长度是边数。非树返回 -1。
int diameter_a, diameter_b;
vector<ll> diameter_path;

int farthest(int s, vector<int>& d, vector<int>& parent) {
    d.assign(n + 1, -1); parent.assign(n + 1, 0);
    queue<int> q; q.push(s); d[s] = 0;
    int best = s;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (d[u] > d[best]) best = u;
        for (ll v : g[u]) if (d[v] == -1) {
            d[v] = d[u] + 1; parent[v] = u; q.push((int)v);
        }
    }
    return best;
}

int tree_diameter() {
    diameter_path.clear(); diameter_a = diameter_b = 0;
    if (n < 1) return -1;
    ll edges = 0;
    for (int u = 1; u <= n; ++u) edges += (ll)g[u].size();
    if (edges != 2LL * (n - 1)) return -1;
    vector<int> d, parent;
    diameter_a = farthest(1, d, parent);
    for (int u = 1; u <= n; ++u) if (d[u] == -1) {
        diameter_a = 0;
        return -1;
    }
    diameter_b = farthest(diameter_a, d, parent);
    for (int u = diameter_b; u; u = parent[u]) diameter_path.push_back(u);
    reverse(diameter_path.begin(), diameter_path.end());
    return d[diameter_b];
}
