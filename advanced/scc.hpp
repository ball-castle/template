#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
struct SCCResult {
    int count = 0;
    vector<int> comp;
    vector<vector<int>> groups{{}}; // groups[1..count]
};
// Kosaraju 两遍迭代 DFS；有向图，合法点 [1,n]，可有重边自环。
inline SCCResult scc(const vector<vector<int>>& g) {
    int n = (int)g.size() - 1;
    vector<vector<int>> rev(n + 1);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) rev[v].push_back(u);
    vector<int> seen(n + 1), finish;
    vector<pair<int, int>> st;
    for (int s = 1; s <= n; ++s) if (!seen[s]) {
        seen[s] = 1; st.push_back({s, 0});
        while (!st.empty()) {
            auto& [u, i] = st.back();
            if (i == (int)g[u].size()) {
                finish.push_back(u); st.pop_back(); continue;
            }
            int v = g[u][i++];
            if (!seen[v]) { seen[v] = 1; st.push_back({v, 0}); }
        }
    }
    SCCResult r; r.comp.resize(n + 1);
    vector<int> stack;
    reverse(finish.begin(), finish.end());
    for (int s : finish) if (!r.comp[s]) {
        ++r.count; r.groups.push_back({});
        r.comp[s] = r.count; stack.push_back(s);
        while (!stack.empty()) {
            int u = stack.back(); stack.pop_back();
            r.groups.back().push_back(u);
            for (int v : rev[u]) if (!r.comp[v]) {
                r.comp[v] = r.count; stack.push_back(v);
            }
        }
    }
    return r;
}
} // namespace cp
