#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// 无向图，点 [1,n]；显式栈模拟 DFS，重边/自环均可。
struct DFSForest {
    vector<int> parent, comp, order;
    int components = 0; // comp 从 1 开始；每个根的 parent=0
};
inline DFSForest dfs_forest(const vector<vector<int>>& g) {
    int n = (int)g.size() - 1;
    DFSForest r;
    r.parent.resize(n + 1); r.comp.resize(n + 1);
    vector<pair<int, int>> st; // (当前点, 下一条邻边下标)
    for (int s = 1; s <= n; ++s) if (!r.comp[s]) {
        r.comp[s] = ++r.components;
        r.order.push_back(s); st.push_back({s, 0});
        while (!st.empty()) {
            auto& [u, i] = st.back();
            if (i == (int)g[u].size()) { st.pop_back(); continue; }
            int v = g[u][i++];
            if (r.comp[v]) continue;
            r.comp[v] = r.components; r.parent[v] = u;
            r.order.push_back(v); st.push_back({v, 0});
        }
    }
    return r; // order 为入栈顺序，父亲先于儿子
}
} // namespace cp
