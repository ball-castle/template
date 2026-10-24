#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// cf/1/1124/B.cpp：原判定原样。2268B限定0<=x<16、n>=2。
bool good(int x) {
    return __builtin_popcount(x) % 2 == 0;
}
ll all_meat_count(const vector<ll>& a) {
    ll ans = 0;
    for(size_t i = 1; i < a.size(); i++) if(good(int(a[i]))) ans++;
    return ans;
}
void all_meat_update(vector<ll>& a, ll& ans, int p, ll x) {
    if(good(int(a[p]))) ans--;
    a[p] = x;
    if(good(int(a[p]))) ans++;
}
