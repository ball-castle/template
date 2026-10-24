#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// PM02. 沿用 Mobius_inversion.cpp 的线性筛循环。
const int N = 1000010;
int primes[N], lp[N], cnt;

void sieve(int n) { // 1<=n<N；lp[x] 为最小质因数，lp[1]=0。
    fill(lp, lp + n + 1, 0); cnt = 0;
    for (int i = 2; i <= n; ++i) {
        if (!lp[i]) lp[i] = i, primes[++cnt] = i;
        for (int j = 1; j <= cnt && primes[j] <= n / i; ++j) {
            lp[i * primes[j]] = primes[j];
            if (i % primes[j] == 0) break;
        }
    }
}

// 单个数试除；n<=1e12 时最多约 1e6 次，不能批量处理大量大数。
bool is_prime(ll n) {
    if (n < 2) return false;
    for (ll p = 2; p <= n / p; ++p) if (n % p == 0) return false;
    return true;
}

vector<pair<ll, int>> factorize(ll n) { // 1<=n<=1e12。
    vector<pair<ll, int>> ans;
    for (ll p = 2; p <= n / p; ++p) if (n % p == 0) {
        int c = 0;
        do { n /= p; ++c; } while (n % p == 0);
        ans.push_back({p, c});
    }
    if (n > 1) ans.push_back({n, 1});
    return ans;
}

vector<ll> divisors(ll n) { // 包括 1 和 n，升序。
    vector<ll> ans{1};
    for (auto [p, c] : factorize(n)) {
        int old = (int)ans.size();
        ll power = 1;
        for (int k = 1; k <= c; ++k) {
            power *= p;
            for (int i = 0; i < old; ++i) ans.push_back(ans[i] * power);
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}
