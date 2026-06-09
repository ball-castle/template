#include <bits/stdc++.h>
#define ll long long 
using namespace std;
ll n;
// the first i a[i] >= t lower_bound(a + 1, a + n + 1, t);

ll a[110];

inline ll read() {
    ll x = 0, f = 1; char ch = getchar();
    while(ch < '0' || ch > '9') {if(ch == '-') f = -1; ch = getchar();}
    while(ch >= '0' && ch <= '9') {x = (x << 3) + (x << 1) + (ch ^ 48); ch = getchar();}
    return x * f;
}
int main() {
	ll n;
	n = read();
	for(ll i = 1; i <= n; i++) a[i] = read();

	auto it = upper_bound(a + 1, a + n + 1, 4, [&](ll x, ll y) {
		return x > y;
	});
	cout << a[it - a] << '\n';
	return 0;
}	