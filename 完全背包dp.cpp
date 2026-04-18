#include <bits/stdc++.h>
using namespace std;

const int N = 1010;
int w[N], v[N];      // w[i]:重量, v[i]:价值
int dp[N];           // dp[j]:容量为j时的最大价值

int main() {
    int n, m;        // n个物品, 背包容量m
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> w[i] >> v[i];
    
    for (int i = 1; i <= n; i++) {
        // 正序遍历！表示物品i可以选多次（完全背包）
        for (int j = w[i]; j <= m; j++) {
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
        }
    }
    cout << dp[m] << endl;
    return 0;
}