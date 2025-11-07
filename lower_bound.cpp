#include <bits/stdc++.h>
#define ll long long 
using namespace std;
ll n;
// the first i a[i] >= t lower_bound(a + 1, a + n + 1, t);
ll low_bound() {
	ll l = 1, r = n;
	while(l < r) {
		ll mid = l + ((l - r) >> 1);
		if(a[mid] < t) l = mid + 1;
		else r = mid;
	}
	return l;
}
