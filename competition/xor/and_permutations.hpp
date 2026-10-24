#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// cf/date/7/7.9/2_and.cpp：909F的两段原构造，仅抽出输入输出。
bool and_zero_perm(int n, vector<ll>& p) {
    p.clear();
    if(n % 2 == 1) return false;
    p.assign(n + 1, 0);
    ll r = n;
    while(r > 0) {
        ll mask = 1;
        while(mask < r) mask = mask * 2 + 1;
        ll l = mask - r;
        for(ll i = l; i <= r; i++) p[i] = mask ^ i;
        r = l - 1;
    }
    return true;
}
bool and_nonzero_perm(int n, vector<ll>& q) {
    q.clear();
    if(n < 6 || (n & (n - 1)) == 0) return false;
    q.assign(n + 1, 0);
    vector<ll> a = {1, 3, 2, 6, 4, 5};
    if(n != 6) a.push_back(7);
    for(ll i = 0; i < ll(a.size()); i++) q[a[i]] = a[(i + 1) % a.size()];
    for(ll l = 8; l <= n; l <<= 1) {
        ll r = min(ll(n), 2 * l - 1);
        for(ll i = l; i < r; i++) q[i] = i + 1;
        q[r] = l;
    }
    return true;
}
