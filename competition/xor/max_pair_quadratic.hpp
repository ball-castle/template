#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// cf/4/1090/E_xor_find_pattern.cpp：2218E原O(n²)足够，保留双循环。
ull max_xor_pair_quadratic(const vector<ull>& a) {
    ull ans = 0;
    for(size_t i = 1; i < a.size(); i++)
        for(size_t j = i + 1; j < a.size(); j++)
            ans = max(ans, a[i] ^ a[j]);
    return ans;
}
