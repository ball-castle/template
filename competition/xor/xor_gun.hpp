#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// cf/date/5.15/3.cpp：1457D，正数、非降序、a[i]<=1e9。
// 原ca每次扫区间；仅替成前缀查询，O(n^4)降到O(n^3)。
int xor_gun(const vector<ll>& a) {
    int n = int(a.size()) - 1;
    if(n > 60) return 1;
    vector<ll> p(n + 1);
    for(int i = 1; i <= n; i++) p[i] = p[i - 1] ^ a[i];
    int ans = n;
    for(int l = 1; l <= n; l++)
        for(int r = l + 1; r <= n; r++)
            for(int m = l + 1; m <= r; m++)
                if((p[m - 1] ^ p[l - 1]) > (p[r] ^ p[m - 1]))
                    ans = min(ans, r - l - 1);
    return ans == n ? -1 : ans;
}
