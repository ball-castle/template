#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PG02. 直接保留 DSU.cpp / cf/xian/B.cpp 的 fa/rk 写法。
// 1<=x<=n<N；rk 是秩，不是集合大小。多测重新 init(n)。
const ll N = 200010;
ll fa[N], rk[N];

void init(ll n) {
    for (ll i = 1; i <= n; i++) {
        fa[i] = i;
        rk[i] = 1;
    }
}

int find(ll x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}

void unite(ll x, ll y) {
    ll fx = find(x), fy = find(y);
    if (fx == fy) return;
    if (rk[fx] < rk[fy]) fa[fx] = fy;
    else {
        fa[fy] = fx;
        if (rk[fx] == rk[fy]) rk[fx]++;
    }
}

bool same(ll x, ll y) {
    return find(x) == find(y);
}
