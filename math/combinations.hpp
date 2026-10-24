#pragma once
#include <bits/stdc++.h>
#include "modular.hpp"
using namespace std;
using ll = long long;

// 仅适用于已知质数prime，并且0<=预处理上限n<prime。
// 本模板不检测质数；复合模数或n>=prime不能用阶乘逆元法。
struct PrimeComb {
    ll mod;
    vector<ll> fac, ifac;
    PrimeComb(int n, ll prime) : mod(prime) {
        assert(n >= 0 && prime >= 2 && ll(n) < prime);
        fac.resize(n + 1); ifac.resize(n + 1);
        fac[0] = 1;
        for (int i = 1; i <= n; ++i)
            fac[i] = mod_mul(fac[i - 1], i, mod);
        ifac[n] = mod_pow(fac[n], mod - 2, mod);
        for (int i = n; i >= 1; --i)
            ifac[i - 1] = mod_mul(ifac[i], i, mod);
    }
    ll choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        assert(n >= 0 && n < int(fac.size()));
        return mod_mul(fac[n], mod_mul(ifac[k], ifac[n - k], mod), mod);
    }
};
// PrimeComb comb(max_n,1000000007); cout << comb.choose(n,k);
// C(0,0)=1；无效k返回0；有效查询的n不能超过预处理上限。
// 预处理O(n+log mod)时间/O(n)空间，每次查询O(1)。
