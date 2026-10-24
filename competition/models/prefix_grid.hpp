#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 505;
int n, m;
ll a[N][N], s[N][N], d[N][N];
// 1..n,1..m；n,m<=500；所有前缀和、差分累计值处于ll范围。
void init(int rows, int cols) {
    assert(0 <= rows && rows <= 500 && 0 <= cols && cols <= 500);
    n = rows; m = cols;
    memset(a, 0, sizeof(a));
    memset(s, 0, sizeof(s));
    memset(d, 0, sizeof(d));
}
void build_prefix() {
    for(int i = 1; i <= n; i++) for(int j = 1; j <= m; j++)
        s[i][j] = a[i][j] + s[i-1][j] + s[i][j-1] - s[i-1][j-1];
}
ll query(int x1, int y1, int x2, int y2) {
    if(x1 > x2 || y1 > y2) return 0;
    return s[x2][y2] - s[x1-1][y2] - s[x2][y1-1] + s[x1-1][y1-1];
}
// 离线矩形加；处理完全部操作后调用restore_difference一次。
void add_rect(int x1, int y1, int x2, int y2, ll val) {
    if(x1 > x2 || y1 > y2) return;
    d[x1][y1] += val; d[x2+1][y1] -= val;
    d[x1][y2+1] -= val; d[x2+1][y2+1] += val;
}
void restore_difference() {
    for(int i = 1; i <= n; i++) for(int j = 1; j <= m; j++) {
        d[i][j] += d[i-1][j] + d[i][j-1] - d[i-1][j-1];
        a[i][j] += d[i][j];
    }
    memset(d, 0, sizeof(d)); // 下一批操作可重新累计
    // s仍是旧快照；若要查询新数组，重新build_prefix。
}
