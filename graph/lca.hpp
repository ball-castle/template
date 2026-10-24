#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// 无向树，点 [1,n]，邻接表双向；非法根/非树时 ok=false。
// 邻接端点与双向对称性由调用方保证；根的祖先为哨兵 0。
struct LCA {
    int n, levels = 1;
    bool ok = false;
    vector<int> depth;
    vector<vector<int>> up;
    explicit LCA(const vector<vector<int>>& g, int root = 1)
        : n((int)g.size() - 1) {
        if (n < 1 || root < 1 || root > n) return;
        ll count = 0;
        for (int u = 1; u <= n; ++u) count += (ll)g[u].size();
        if (count != 2LL * (n - 1)) return;
        while ((1LL << levels) <= n) ++levels;
        up.assign(levels, vector<int>(n + 1));
        depth.assign(n + 1, -1); depth[root] = 0;
        vector<int> order{root};
        for (int i = 0; i < (int)order.size(); ++i) {
            int u = order[i];
            for (int v : g[u]) if (depth[v] == -1) {
                depth[v] = depth[u] + 1; up[0][v] = u;
                for (int k = 1; k < levels; ++k)
                    up[k][v] = up[k - 1][up[k - 1][v]];
                order.push_back(v);
            }
        }
        ok = ((int)order.size() == n);
    }
    int jump(int u, ll k) const {
        if (!ok || u < 1 || u > n || k < 0 || k > depth[u]) return 0;
        for (int i = 0; i < levels; ++i) if (k >> i & 1) u = up[i][u];
        return u;
    }
    int lca(int u, int v) const {
        if (!ok || u < 1 || u > n || v < 1 || v > n) return 0;
        if (depth[u] < depth[v]) swap(u, v);
        u = jump(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int k = levels - 1; k >= 0; --k) if (up[k][u] != up[k][v]) {
            u = up[k][u]; v = up[k][v];
        }
        return up[0][u];
    }
    int distance(int u, int v) const {
        int a = lca(u, v);
        return a ? depth[u] + depth[v] - 2 * depth[a] : -1;
    }
};
} // namespace cp
