#pragma once
#include <bits/stdc++.h>
using namespace std;

// PG12. 输入边号为 0..m-1；无向的两个方向共用同一个 id。
const int N = 200010;
int n, color[N], fa[N], pe[N];
vector<pair<int, int>> g[N]; // (终点,边号)
vector<int> cycle, cycle_edges; // 首尾同点，边号与相邻点逐一对应。

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) g[i].clear();
}

bool find_cycle(bool directed) {
    fill(color + 1, color + n + 1, 0);
    fill(fa + 1, fa + n + 1, 0);
    fill(pe + 1, pe + n + 1, -1);
    cycle.clear(); cycle_edges.clear();
    for (int s = 1; s <= n; ++s) if (!color[s]) {
        vector<pair<int, int>> st{{s, 0}};
        color[s] = 1;
        while (!st.empty()) {
            int u = st.back().first;
            int& i = st.back().second;
            if (i == (int)g[u].size()) {
                color[u] = 2; st.pop_back(); continue;
            }
            auto [v, id] = g[u][i++];
            if (!directed && id == pe[u]) continue;
            if (!color[v]) {
                fa[v] = u; pe[v] = id; color[v] = 1;
                st.push_back({v, 0});
            } else if (color[v] == 1) {
                for (int x = u; x != v; x = fa[x]) {
                    cycle.push_back(x); cycle_edges.push_back(pe[x]);
                }
                cycle.push_back(v);
                reverse(cycle.begin(), cycle.end());
                reverse(cycle_edges.begin(), cycle_edges.end());
                cycle.push_back(v); cycle_edges.push_back(id);
                return true;
            }
        }
    }
    return false;
}
