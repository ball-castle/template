#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// g.size()==n+1；点 [1,n]；有向/无向均可，边权均视为 1。
struct BFSResult {
    vector<int> dist, parent; // 不可达 dist=-1；起点 parent=0
    vector<int> path(int t) const {
        if (t < 1 || t >= (int)dist.size() || dist[t] < 0) return {};
        vector<int> ans;
        for (; t; t = parent[t]) ans.push_back(t);
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
inline BFSResult bfs(const vector<vector<int>>& g, int s) {
    int n = (int)g.size() - 1;
    BFSResult r{vector<int>(n + 1, -1), vector<int>(n + 1)};
    if (s < 1 || s > n) return r;
    queue<int> q; q.push(s); r.dist[s] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (r.dist[v] == -1) {
            r.dist[v] = r.dist[u] + 1;
            r.parent[v] = u;
            q.push(v);
        }
    }
    return r;
}
} // namespace cp
