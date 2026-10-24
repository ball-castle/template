#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

// PM04. 先提升类型再乘。返回 i128，转 ll 前检查结果范围。
i128 sum1(ll n) { // 0<=n<=1e18。
    return (i128)n * (n + 1) / 2;
}

i128 sum2(ll n) { // 0<=n<=1e12。
    return (i128)n * (n + 1) * (2 * n + 1) / 6;
}

i128 arithmetic_sum(ll first, ll step, ll count) { // 0<=count<=1e9。
    return (i128)count * first + (i128)step * count * (count - 1) / 2;
}

// 源于 cf/date/7/7.28/1.cpp；n<=2e5，|a[i]|<=1e18。
i128 pair_abs_sum(vector<ll> a) {
    sort(a.begin(), a.end());
    ll n = (ll)a.size();
    i128 ans = 0;
    for (ll i = 0; i < n; ++i) ans += (i128)a[i] * (2 * i - n + 1);
    return ans;
}
