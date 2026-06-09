#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/*
考虑一个很简单的问题
1 - n有多少个数和 30 互质
考虑容斥 全集剪掉有因数的
是不是对于一些情况加减 那么什么时候加 什么时候减呢
2整除 -1     6    1
3     -1     10   1
5     -1
相交一次就反一次
平方根本没有出现

所以一开始的全集 u = 1
奇数个 -1 ^ k;
平方直接为0

本质：这正是组合数学里的经典结论：一个由k个元素组成的集合，它的奇数个元素的子集个数，和偶数个元素的子集个数是一样多的。
所以 n = 1 u(d)和为1 这个性质非常强
n > 1 为 0

利用这个性质，对于gcd(i) == 1 就是 u(gcd(i)) 和为1
现在精确

推广
f[n] 精确版本 比如 gcd == 1
F[n] 是某某的倍数
F[n] = sum(f[d]) (d | n)很简单
但是反过来就是反演

f(n) = sum(u(d / n))F(d) (d | n)
如果n是1 求的是 gcd（1） 如果是2的话相当于等比例缩小两倍，一一映射

https://www.luogu.com.cn/problem/P2522
*/

vector<ll> get_mu(ll n) {
    vector<ll> mu(n + 1), primes;
    vector<bool> not_prime(n + 1);
    primes.reserve(n);
    mu[1] = 1;
    for(int x = 2; x <= n; x++) {
        if(!not_prime[x]) primes.push_back(x);
        mu[x] = -1;
        for(ll p : primes) {
            if(x * p > n) break;
            not_prime[x * p] = 1;
            if(x % p == 0) {mu[x * p] = 0; break;}
            else mu[x * p] = -mu[x];
        }
    }
    return mu;
}

int mu[50005], sum[50005], primes[50005], is_prime[50005];
int cnt = 0, k;


void init_mu() {
    for(int i = 1; i <= 50002; i++) is_prime[i] = 1;
    is_prime[0] = 0, is_prime[1] = 0;
    mu[1] = 1;

    for(int i = 2; i <= 50000; i++) { // 线性筛
        if(is_prime[i]) primes[++cnt] = i, mu[i] = -1;
        for(int j = 1; j <= cnt && i * primes[j] <= 50000; j++) {
            is_prime[i * primes[j]] = 0;
            if(i % primes[j] == 0) {mu[i * primes[j]] = 0; break;} // 有平方项的就可以不管其它了
            else mu[i * primes[j]] = -mu[i];
        }
    }

    for(int i = 1; i <= 50000; i++) sum[i] = sum[i - 1] + mu[i];
}

ll cal(int n, int m) {
    if(n == 0 || m == 0) return 0;
    ll ans = 0;
    int li = min(n, m);

    for(int l = 1, r; l <= li; l = r + 1) {
        r = min(n / (n / l), m / (m / l));   // 这个代码很关键，就是在相同答案的最大边界，一块一块处理
        ll mu_part = sum[r] - sum[l - 1];
        ans += mu_part * (n / l) * (m / l);
    }
    return ans;
} 


void solve() {
    int a, b, c, d;
    cin >> a >> b >> c >> d >> k;

    ll ans = cal(b / k, d / k) - cal((a - 1) / k, d / k) - cal(b / k, (c - 1) / k) + cal((a - 1) / k, (c - 1) / k);
    cout << ans << '\n';
}



int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    init_mu();
    int T;
    cin >> T;
    while(T--) solve();    
    return 0;
}