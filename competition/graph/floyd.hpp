#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PG10. n<N，|边权|<=1e12；负环返回 false，此时不使用距离结果。
const int N = 510;
const ll INF = LLONG_MAX / 4;
int n;
ll dist[N][N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) dist[i][j] = (i == j ? 0 : INF);
}

void add_edge(int u, int v, ll w) { dist[u][v] = min(dist[u][v], w); }

bool floyd() {
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i) if (dist[i][k] != INF)
            for (int j = 1; j <= n; ++j) if (dist[k][j] != INF) {
                __int128 sum = (__int128)dist[i][k] + dist[k][j];
                dist[i][j] = min(dist[i][j], (ll)max(sum, -(__int128)INF));
            }
    for (int i = 1; i <= n; ++i) if (dist[i][i] < 0) return false;
    return true;
}
