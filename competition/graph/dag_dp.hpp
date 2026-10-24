#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PG15. DAG 上从 s 出发的最长边数、路径数；重边视为不同路径选择。
const int N = 200010;
int n, indeg[N], dist[N];
ll ways[N];
vector<int> g[N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

// 1<=s<=n<N，1<=mod<=LLONG_MAX。有环返回 false，结果作废。
bool dag_dp(int s, ll mod) {
    fill(indeg + 1, indeg + n + 1, 0);
    fill(dist + 1, dist + n + 1, -1);
    fill(ways + 1, ways + n + 1, 0);
    for (int u = 1; u <= n; ++u) for (int v : g[u]) ++indeg[v];
    queue<int> q;
    for (int u = 1; u <= n; ++u) if (!indeg[u]) q.push(u);
    dist[s] = 0; ways[s] = 1 % mod;
    int cnt = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop(); ++cnt;
        for (int v : g[u]) {
            if (dist[u] != -1) {
                dist[v] = max(dist[v], dist[u] + 1);
                ways[v] = (ll)(((__int128)ways[v] + ways[u]) % mod);
            }
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return cnt == n;
}
