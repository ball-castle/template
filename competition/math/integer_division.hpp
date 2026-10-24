#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PM03. b!=0，排除 a=LLONG_MIN,b=-1（结果超出 ll）。
// C++ / 向零截断；根据非零余数的符号补一次修正。
ll floor_div(ll a, ll b) {
    ll q = a / b, r = a % b;
    if (r != 0 && ((r > 0) != (b > 0))) --q;
    return q;
}

ll ceil_div(ll a, ll b) {
    ll q = a / b, r = a % b;
    if (r != 0 && ((r > 0) == (b > 0))) ++q;
    return q;
}
