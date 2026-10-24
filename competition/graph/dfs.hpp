#pragma once
#include <bits/stdc++.h>
using namespace std;

// PG04. dfs.cpp 的标记后沿邻边访问；栈帧避免长链递归爆栈。
const int N = 200010;
int n, fa[N], comp[N], cnt;
vector<int> g[N], order;
bool visited[N];

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

void dfs(int start) {
    vector<pair<int, int>> st{{start, 0}};
    visited[start] = true; comp[start] = cnt;
    order.push_back(start);
    while (!st.empty()) {
        int u = st.back().first;
        int& i = st.back().second;
        if (i == (int)g[u].size()) { st.pop_back(); continue; }
        int v = g[u][i++];
        if (visited[v]) continue;
        visited[v] = true; comp[v] = cnt; fa[v] = u;
        order.push_back(v);
        st.push_back({v, 0});
    }
}

void dfs_forest() {
    fill(visited + 1, visited + n + 1, false);
    fill(fa + 1, fa + n + 1, 0);
    fill(comp + 1, comp + n + 1, 0);
    cnt = 0; order.clear();
    for (int s = 1; s <= n; ++s) if (!visited[s]) {
        ++cnt;
        dfs(s);
    }
}
