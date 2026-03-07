#include <iostream>
#include <algorithm>
using namespace std;
using ll = long long;

ll logn[10005], f[17][10005];

// ST list sparse table : idempoten
// x opt x = x, (a opt b) opt c = a opt (b opt c)
// gcd, max
void pre() {
    logn[2] = 1;
    for(ll i = 3; i < 1e4 + 5; i++) logn[i] = log[i / 2] + 1;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    pre();

    ll n, m;
    cin >> n >> m;
    for(ll i = 1; i <= n; i++) cin >> f[0][i];
    for(ll j = 1; j  )
}