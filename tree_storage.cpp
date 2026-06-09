#include <bits/stdc++.h>
using namespace std;
using ll = long long;


vector<ll> g[200010];


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    ll n;
    cin >> n;
    for(int i = 1; i <= n - 1; i++) {
        ll u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    } 
    return 0;
}