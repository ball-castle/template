#pragma once
#include <bits/stdc++.h>
using namespace std;

// PG11. graph_paint.cpp / 6.16 的 1/2 染色，补冲突检测。
const int N = 200010;
int n, col[N];
vector<int> g[N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

bool bipartite() {
    fill(col + 1, col + n + 1, 0);
    for (int s = 1; s <= n; ++s) if (!col[s]) {
        queue<int> q;
        col[s] = 1; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (!col[v]) {
                    col[v] = 3 - col[u]; q.push(v);
                } else if (col[v] == col[u]) return false;
            }
        }
    }
    return true;
}
