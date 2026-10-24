#pragma once
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

// b[k] 的最高位为 k；P2，维护任意子集XOR，不是从集合选一个数。
ull b[64];
int rk = 0;
void basis_init() { fill(b, b + 64, 0); rk = 0; }
bool basis_insert(ull x) {
    for(int k = 63; k >= 0; k--) {
        if(((x >> k) & 1) == 0) continue;
        if(b[k]) x ^= b[k];
        else { b[k] = x; rk++; return true; }
    }
    return false; // 消成0：原数已在已有数的XOR张成空间内
}
ull basis_max(ull x = 0) {
    for(int k = 63; k >= 0; k--)
        if((x ^ b[k]) > x) x ^= b[k];
    return x;
}
