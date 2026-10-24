#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PG05. graph/dijkstra.hpp 的同一松弛式，个人全局 g/dist/fa 版。
const int N = 200010;
const ll INF = LLONG_MAX / 4;
int n, fa[N];
ll dist[N];
vector<pair<int, ll>> g[N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

// 点 1..n；0<=w<INF；距离>=INF 视为不可达。
bool dijkstra(const vector<int>& sources) {
    fill(dist + 1, dist + n + 1, INF);
    fill(fa + 1, fa + n + 1, 0);
    for (int s : sources) if (s < 1 || s > n) return false;
    for (int u = 1; u <= n; ++u)
        for (auto [v, w] : g[u])
            if (v < 1 || v > n || w < 0 || w >= INF) return false;
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> q;
    for (int s : sources) if (dist[s] != 0) {
        dist[s] = 0; q.push({0, s});
    }
    while (!q.empty()) {
        auto [du, u] = q.top(); q.pop();
        if (du != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (w > INF - du) continue;
            if (du + w < dist[v]) {
                dist[v] = du + w; fa[v] = u;
                q.push({dist[v], v});
            }
        }
    }
    return true;
}

bool dijkstra(int start) { return dijkstra(vector<int>{start}); }

vector<int> path(int t) {
    if (t < 1 || t > n || dist[t] == INF) return {};
    vector<int> ans;
    for (; t; t = fa[t]) ans.push_back(t);
    reverse(ans.begin(), ans.end());
    return ans;
}
