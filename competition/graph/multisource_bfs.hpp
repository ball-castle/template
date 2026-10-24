#pragma once
#include "bfs.hpp"

// PG08. 复用 PG03 的数组；所有源点同时从距离 0 开始。
void bfs(const vector<int>& sources) {
    fill(visited + 1, visited + n + 1, false);
    fill(dist + 1, dist + n + 1, -1);
    fill(fa + 1, fa + n + 1, 0);
    queue<int> q;
    for (int s : sources) if (!visited[s]) {
        visited[s] = true; dist[s] = 0; q.push(s);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (!visited[v]) {
            visited[v] = true; dist[v] = dist[u] + 1;
            fa[v] = u; q.push(v);
        }
    }
}
