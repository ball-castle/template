#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PM05. mod>0；负数也统一为 [0,mod) 的同余类。
ll norm_mod(ll a, ll mod) {
    ll r = a % mod;
    return r < 0 ? r + mod : r;
}

bool congruent(ll a, ll b, ll mod) {
    return norm_mod(a, mod) == norm_mod(b, mod);
}

// 模 mod 的环上反复 +step（次数>=0）；|step|,mod<=1e18,mod>=1。
// 这里允许在环上绕回；有限区间内只能向前走不是这个模型。
bool reachable_mod(ll start, ll target, ll step, ll mod) {
    ll d = gcd(step, mod);
    return ((__int128)target - start) % d == 0;
}
