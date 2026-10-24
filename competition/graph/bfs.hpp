#pragma once
#include <bits/stdc++.h>
using namespace std;

// PG03. bfs.cpp 的队列/入队即标记；加 dist/fa 供最短路回溯。
const int N = 200010;
int n, dist[N], fa[N];
vector<int> g[N];
bool visited[N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

void bfs(int start) {
    fill(visited + 1, visited + n + 1, false);
    fill(dist + 1, dist + n + 1, -1);
    fill(fa + 1, fa + n + 1, 0);
    queue<int> q;
    visited[start] = true;
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                fa[v] = u;
                q.push(v);
            }
        }
    }
}

vector<int> path(int t) {
    if (t < 1 || t > n || dist[t] == -1) return {};
    vector<int> ans;
    for (; t; t = fa[t]) ans.push_back(t);
    reverse(ans.begin(), ans.end());
    return ans;
}
