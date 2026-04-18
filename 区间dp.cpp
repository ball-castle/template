#include <bits/stdc++.h>
using namespace std;

const int N = 310;
int dp[N][N];        // dp[i][j]:合并区间[i,j]的最小代价
int sum[N];          // 前缀和

int main() {
    int n; cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> sum[i];
        sum[i] += sum[i-1];  // 计算前缀和
    }
    
    memset(dp, 0x3f, sizeof dp);  // 初始化为无穷大
    for (int i = 1; i <= n; i++) dp[i][i] = 0;  // 单个石子不用合并
    
    // 区间DP模板：先枚举长度，再枚举左端点
    for (int len = 2; len <= n; len++) {          // 区间长度
        for (int i = 1; i + len - 1 <= n; i++) {  // 左端点
            int j = i + len - 1;                  // 右端点
            for (int k = i; k < j; k++) {         // 枚举分割点
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k+1][j] + sum[j] - sum[i-1]);
            }
        }
    }
    cout << dp[1][n] << endl;
    return 0;
}