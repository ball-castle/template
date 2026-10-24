#pragma once
#include <bits/stdc++.h>
#include "modular.hpp"
using namespace std;
using ll = long long;
using Matrix = vector<vector<ll>>;

// 矩阵下标0-indexed；只接受同阶方阵；1<=mod<=LLONG_MAX。
// 普通模加/模乘，不是最短路min-plus或最大值max-plus矩阵。
inline Matrix matrix_mul(const Matrix& a, const Matrix& b, ll mod) {
    int n = int(a.size());
    assert(mod >= 1 && b.size() == a.size());
    for (const auto& row : a) assert(int(row.size()) == n);
    for (const auto& row : b) assert(int(row.size()) == n);
    Matrix c(n, vector<ll>(n));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            ll x = mod_norm(a[i][k], mod);
            if (x == 0) continue;
            for (int j = 0; j < n; ++j) {
                ll y = mod_norm(b[k][j], mod);
                c[i][j] = ll((c[i][j] + (__int128)x * y) % mod);
            }
        }
    return c;
}
inline Matrix matrix_pow(Matrix a, unsigned long long exp, ll mod) {
    assert(mod >= 1);
    int n = int(a.size());
    for (const auto& row : a) assert(int(row.size()) == n);
    Matrix ans(n, vector<ll>(n));
    for (int i = 0; i < n; ++i) ans[i][i] = 1 % mod;
    while (exp) {
        if (exp & 1ULL) ans = matrix_mul(ans, a, mod);
        a = matrix_mul(a, a, mod);
        exp >>= 1;
    }
    return ans;
}
// 例：Matrix t={{1,1},{1,0}}; auto p=matrix_pow(t,n,mod);
// 先写清状态列向量，再验证转移一次；指数0返回单位矩阵。
