#include <bits/stdc++.h>
#define ll long long 
using namespace std;

ll a, b, p;   // a at p inverse element a ^ (p - 2)

ll ksm(ll a, ll b, ll p) {
	ll sum = 1;
	while(b) {
		if(b & 1) sum = (sum * a) % p;
		a = (a * a) % p;
		b = b >> 1;
	}
	return sum;
}



int main() {
	scanf("%lld%lld%lld", &a, &b, &p);
	ll s = ksm(a, b, p);
	printf("%lld^%lld mod %lld=%lld", a, b, p, s);
	return 0;
}
