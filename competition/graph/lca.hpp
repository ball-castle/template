#pragma once
#include "tree_storage.hpp"

// PG06. 复用 PG01 的 g/fa/dep/order，不重新存一份树。
const int LOG = 20; // N=200010 时足够；扩大 N 时同步调整。
int up[N][LOG];
bool lca_ready = false;

bool build_lca(int root = 1) {
    lca_ready = root_tree(root);
    if (!lca_ready) return false;
    fill(up[0], up[0] + LOG, 0);
    for (ll u : order) {
        up[u][0] = (int)fa[u];
        for (int k = 1; k < LOG; ++k)
            up[u][k] = up[up[u][k - 1]][k - 1];
    }
    return true;
}

int jump(int u, ll k) {
    if (!lca_ready || u < 1 || u > n || k < 0 || k > dep[u]) return 0;
    for (int i = 0; i < LOG; ++i) if (k >> i & 1) u = up[u][i];
    return u;
}

int lca(int u, int v) {
    if (!lca_ready || u < 1 || u > n || v < 1 || v > n) return 0;
    if (dep[u] < dep[v]) swap(u, v);
    u = jump(u, dep[u] - dep[v]);
    if (u == v) return u;
    for (int k = LOG - 1; k >= 0; --k) if (up[u][k] != up[v][k]) {
        u = up[u][k]; v = up[v][k];
    }
    return up[u][0];
}

ll distance(int u, int v) {
    int a = lca(u, v);
    return a ? dep[u] + dep[v] - 2 * dep[a] : -1;
}
