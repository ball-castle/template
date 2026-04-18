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
        // 必须倒序！防止重复选（01背包每个物品只能选一次）
        for (int j = m; j >= w[i]; j--) {
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
        }
    }
    cout << dp[m] << endl;
    return 0;
}