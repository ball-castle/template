#pragma once
#include <bits/stdc++.h>
using namespace std;

// PG07. 矩形网格，g[1..n][1..m]，# 为障碍；四邻接单位代价。
const int N = 1010;
int n, m, dist[N][N];
string g[N]; // 每行读入后前面补一个空格。
const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};

void bfs(int sx, int sy) {
    for (int i = 1; i <= n; ++i)
        fill(dist[i] + 1, dist[i] + m + 1, -1);
    if (sx < 1 || sx > n || sy < 1 || sy > m || g[sx][sy] == '#')
        return;
    queue<pair<int, int>> q;
    dist[sx][sy] = 0; q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (g[nx][ny] == '#' || dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }
}
