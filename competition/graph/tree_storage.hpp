#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PG01. 原 tree_storage.cpp 的双向邻接表；点 1..n，1<=n<N。
const int N = 200010;
int n;
vector<ll> g[N];
ll fa[N], dep[N], sz[N];
vector<ll> order; // 父亲先于儿子；逆序可做子树 DP。

void init(int count) {
    n = count;
    for (int i = 1; i <= n; ++i) {
        g[i].clear();
        fa[i] = sz[i] = 0;
        dep[i] = -1;
    }
    order.clear();
}

void read_tree(istream& in = cin) {
    int count;
    in >> count;
    init(count);
    for (int i = 1; i <= n - 1; ++i) {
        ll u, v;
        in >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
}

// 双向对称的邻接表由调用方保证。非树返回 false。
bool root_tree(int root = 1) {
    order.clear();
    if (root < 1 || root > n) return false;
    ll edges = 0;
    for (int i = 1; i <= n; ++i) {
        fa[i] = 0; dep[i] = -1; sz[i] = 1;
        edges += (ll)g[i].size();
    }
    if (edges != 2LL * (n - 1)) return false;
    vector<ll> st{root};
    dep[root] = 0;
    while (!st.empty()) {
        ll u = st.back(); st.pop_back();
        order.push_back(u);
        for (ll v : g[u]) if (dep[v] == -1) {
            fa[v] = u; dep[v] = dep[u] + 1;
            st.push_back(v);
        }
    }
    if ((int)order.size() != n) return false;
    for (int i = n - 1; i > 0; --i) {
        ll u = order[i];
        sz[fa[u]] += sz[u];
    }
    return true;
}
