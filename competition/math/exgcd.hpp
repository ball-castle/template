#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PM01. |a|,|b|<=1e18；返回非负 gcd，且 a*x+b*y=gcd。
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = (a < 0 ? -1 : 1); y = 0;
        return a < 0 ? -a : a;
    }
    ll x1, y1;
    ll d = exgcd(b, a % b, x1, y1);
    x = y1;
    y = (ll)((__int128)x1 - (__int128)(a / b) * y1);
    return d;
}

// 2<=mod<=1e18，|a|<=1e18；不存在逆元返回 -1。
ll inverse_mod(ll a, ll mod) {
    a %= mod;
    if (a < 0) a += mod;
    ll x, y;
    if (exgcd(a, mod, x, y) != 1) return -1;
    x %= mod;
    return x < 0 ? x + mod : x;
}
