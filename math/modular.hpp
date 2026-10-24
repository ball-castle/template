#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 1<=mod<=LLONG_MAX；返回[0,mod)。支持负数和LLONG_MIN。
inline ll mod_norm(ll a, ll mod) {
    assert(mod >= 1);
    a %= mod;
    if (a < 0) a += mod;
    return a;
}
inline ll mod_add(ll a, ll b, ll mod) {
    return ll(((__int128)mod_norm(a, mod) + mod_norm(b, mod)) % mod);
}
inline ll mod_mul(ll a, ll b, ll mod) {
    return ll((__int128)mod_norm(a, mod) * mod_norm(b, mod) % mod);
}
// exp是非负整数；0^0定义为1再取模；模1时答案恒为0。
inline ll mod_pow(ll a, unsigned long long exp, ll mod) {
    a = mod_norm(a, mod);
    ll ans = 1 % mod;
    while (exp) {
        if (exp & 1ULL) ans = mod_mul(ans, a, mod);
        a = mod_mul(a, a, mod);
        exp >>= 1;
    }
    return ans;
}
// 费马逆元：仅当mod为质数且a%mod!=0时，inv=mod_pow(a,mod-2,mod)。
// 复合模数不能套此公式；负指数不能直接传入本函数。
