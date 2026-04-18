#include <bits/stdc++.h>
using namespace std;

const int N = 1010;
int a[N], dp[N];     // dp[i]:以i结尾的最长上升子序列长度

int main() {
    int n; cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];
    
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        dp[i] = 1;   // 初始：只有自己
        for (int j = 1; j < i; j++) {
            if (a[j] < a[i])
                dp[i] = max(dp[i], dp[j] + 1);
        }
        ans = max(ans, dp[i]);
    }
    cout << ans << endl;
    return 0;
}