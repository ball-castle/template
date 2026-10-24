#pragma once
#include <bits/stdc++.h>
using namespace std;

// PG09. 点 1..n；权只能为 0 或 1。有向/无向都可。
const int N = 200010, INF = 1000000000;
int n, dist[N], fa[N];
vector<pair<int, int>> g[N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

void bfs01(int start) {
    fill(dist + 1, dist + n + 1, INF);
    fill(fa + 1, fa + n + 1, 0);
    deque<pair<int, int>> q; // (距离快照,点)，弹出时丢掉过期状态。
    dist[start] = 0; q.push_front({0, start});
    while (!q.empty()) {
        auto [du, u] = q.front(); q.pop_front();
        if (du != dist[u]) continue;
        for (auto [v, w] : g[u]) if (du + w < dist[v]) {
            dist[v] = du + w; fa[v] = u;
            if (w == 0) q.push_front({dist[v], v});
            else q.push_back({dist[v], v});
        }
    }
}
