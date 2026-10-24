#pragma once
#include <bits/stdc++.h>
#include "matrix_power.hpp"
using namespace std;
using ll = long long;

// CF1182E：f[n]=f[n-1]*f[n-2]*f[n-3]*c^(2n-6)，n>=4。
// n>=1；f1,f2,f3,c可为任意ll；固定模P=1e9+7。
// 这里额外支持n=1/2/3和零底数，原题输入从n=4开始。
inline ll product_recurrence(unsigned long long n, ll f1, ll f2,
                             ll f3, ll c) {
    assert(n >= 1);
    const ll p = 1000000007, q = p - 1;
    array<ll, 4> f = {f1, f2, f3, c};
    for (ll& x : f) x = mod_norm(x, p);
    if (n <= 3) return f[n - 1];
    // n>=4时四个底数的真实指数都为正。先判零，再取指数模p-1。
    for (ll x : f) if (x == 0) return 0;
    Matrix t = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}};
    Matrix a = matrix_pow(t, n - 3, q);
    // c的指数E：E[t+1]=E[t]+E[t-1]+E[t-2]+2*t-4。
    // 状态列向量为 [E[t],E[t-1],E[t-2],t,1]，初态[0,0,0,3,1]。
    Matrix u = {
        {1, 1, 1, 2, -4}, {1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0}, {0, 0, 0, 1, 1}, {0, 0, 0, 0, 1}
    };
    Matrix b = matrix_pow(u, n - 3, q);
    ll ec = mod_add(mod_mul(b[0][3], 3, q), b[0][4], q);
    array<ll, 4> e = {a[0][2], a[0][1], a[0][0], ec};
    ll ans = 1;  // 连乘单位元是1，不能从0开始。
    for (int i = 0; i < 4; ++i)
        ans = mod_mul(ans, mod_pow(f[i], e[i], p), p);
    return ans;
}
