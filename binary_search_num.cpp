#include <bits/stdc++.h>
#define ll long long 
using namespace std;

ll n, m;
ll a[1000010];
ll t;

void binary_search() {
	ll l = 1, r = n;
	scanf("%lld", &t);
	while(l < r) {
		ll mid = l + ((r - l) >> 1);
		if(a[mid] == t) {printf("%lld ", mid); return;}
		if(a[mid] < t) l = mid + 1;
		if(a[mid] > t) r = mid - 1; 
	}
	printf("-1 ");
}



int main() {
	scanf("%lld%lld", &n, &m);
	for(ll i = 1; i <= n; i++) scanf("%lld", &a[i]);
	while(m--) {
		binary_search();
	}
	return 0;
}
