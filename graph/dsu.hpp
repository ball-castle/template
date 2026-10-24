#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace cp {
// 点编号 [1,n]；路径减半 + 按集合大小合并。
struct DSU {
    vector<int> p, sz;
    int components;
    explicit DSU(int n) : p(n + 1), sz(n + 1, 1), components(n) {
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; --components;
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
    int size(int x) { return sz[find(x)]; }
};
} // namespace cp
