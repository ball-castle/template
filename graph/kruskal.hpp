#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
struct KruskalEdge { int u, v; ll w; };
struct KruskalResult {
    bool connected = false;
    ll weight = 0; // 不连通时仍返回最小生成森林的总权
    int components = 0;
    vector<int> edges; // 选中边在原输入中的 0-based 下标
};
// 点 [1,n]；无向边只输入一次，支持负权、重边、自环。
// 端点须合法；总权及累加过程须在 ll 范围内。
inline KruskalResult kruskal(int n, const vector<KruskalEdge>& e) {
    vector<int> p(n + 1), sz(n + 1, 1), id(e.size());
    iota(p.begin(), p.end(), 0); iota(id.begin(), id.end(), 0);
    auto find = [&](int x) {
        while (x != p[x]) { p[x] = p[p[x]]; x = p[x]; }
        return x;
    };
    sort(id.begin(), id.end(), [&](int a, int b) {
        return e[a].w < e[b].w;
    });
    KruskalResult r; r.components = n;
    for (int i : id) {
        int a = find(e[i].u), b = find(e[i].v);
        if (a == b) continue;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; --r.components;
        r.weight += e[i].w; r.edges.push_back(i);
    }
    r.connected = (n > 0 && r.components == 1);
    return r;
}
} // namespace cp
