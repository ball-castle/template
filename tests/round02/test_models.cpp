#include <bits/stdc++.h>
using namespace std;
#if !__has_include("../../competition/models/interval_count.hpp")
int main() { cerr << "RED: interval models not implemented\n"; return 1; }
#else
namespace interval_model {
#include "../../competition/models/interval_count.hpp"
#include "../../competition/models/interval_construct.hpp"
}
using ll = long long;
using i128 = __int128_t;
vector<int> distances(int n, int lo, int hi, int mode) {
    int base=hi-lo+1, states=1;
    for(int i=0;i<n;i++) states*=base;
    auto encode=[&](const vector<int>& a) {
        int s=0, mul=1;
        for(int x:a) { s+=(x-lo)*mul; mul*=base; }
        return s;
    };
    vector<int> dist(states,-1), a(n), b(n);
    queue<int> q; int start=encode(vector<int>(n));
    dist[start]=0; q.push(start);
    while(!q.empty()) {
        int state=q.front();q.pop(); int temp=state;
        for(int i=0;i<n;i++) { a[i]=temp%base+lo; temp/=base; }
        for(int l=0;l<n;l++) for(int r=l;r<n;r++) {
            if(mode==2 && l!=0) continue;
            if(mode==3 && r!=n-1) continue;
            for(int sign:{-1,1}) {
                if(mode==1 && sign<0) continue;
                b=a; bool ok=true;
                for(int i=l;i<=r;i++) {
                    b[i]+=sign; if(b[i]<lo || b[i]>hi) ok=false;
                }
                if(!ok) continue;
                int next=encode(b);
                if(dist[next]==-1) {dist[next]=dist[state]+1;q.push(next);}
            }
        }
    }
    return dist;
}
int brute_arbitrary(vector<i128> d) {
    d.erase(remove(d.begin(),d.end(),0),d.end());
    int n=(int)d.size(), full=(1<<n)-1;
    vector<int> memo(1<<n,-1);
    function<int(int)> go=[&](int mask) {
        if(!mask) return 0;
        int &ans=memo[mask]; if(ans!=-1) return ans;
        ans=0; int first=mask&-mask;
        for(int sub=mask;sub;sub=(sub-1)&mask) if(sub&first) {
            i128 sum=0;
            for(int i=0;i<n;i++) if(sub>>i&1) sum+=d[i];
            if(sum==0) ans=max(ans,1+go(mask^sub));
        }
        return ans;
    };
    return n-go(full);
}
int main() {
    using namespace interval_model;
    for(int n=0;n<=4;n++) {
        if(n==0) { assert(unit_intervals({0},0)==0); continue; }
        auto both=distances(n,-2,2,0);
        auto pref=distances(n,-2,2,2);
        auto suff=distances(n,-2,2,3);
        for(int mask=0;mask<(int)both.size();mask++) {
            vector<ll> a(n+1); int x=mask;
            for(int i=1;i<=n;i++) {a[i]=x%5-2;x/=5;}
            assert(unit_intervals(a,0)==both[mask]);
            assert(prefix_units(a)==pref[mask]);
            assert(suffix_units(a)==suff[mask]);
            auto ops=construct_unit_intervals(a,0);
            vector<i128> made(n+1); i128 cost=0;
            assert(ops.has_value());
            for(auto op:*ops) {
                assert(1<=op.l && op.l<=op.r && op.r<=n && op.cnt>0);
                for(int i=op.l;i<=op.r;i++) made[i]+=op.sign*op.cnt;
                cost+=op.cnt;
            }
            for(int i=1;i<=n;i++) assert(made[i]==a[i]);
            assert(cost==both[mask]);
        }
        auto positive=distances(n,0,2,1);
        for(int mask=0;mask<(int)positive.size();mask++) {
            vector<ll> a(n+1), b(n+1); int x=mask;
            for(int i=1;i<=n;i++) {a[i]=x%3;x/=3;b[i]=-a[i];}
            assert(unit_intervals(a,1)==positive[mask]);
            assert(unit_intervals(b,-1)==positive[mask]);
            for(auto [target,sign]:vector<pair<vector<ll>,int>>{{a,1},{b,-1}}) {
                auto ops=construct_unit_intervals(target,sign); assert(ops);
                for(auto op:*ops) assert(op.sign==sign);
            }
        }
    }
    assert(!unit_intervals({0,-1},1));
    assert(!construct_unit_intervals({0,1},-1));
    assert(unit_intervals({0,LLONG_MIN,LLONG_MAX},0)
           ==(i128)LLONG_MAX-(i128)LLONG_MIN);
    mt19937 rng(20261024);
    for(int tc=0;tc<200;tc++) {
        int n=1+(int)(rng()%7);vector<ll>a(n+1);
        vector<i128>d(n+1);
        for(int i=1;i<=n;i++)a[i]=(int)(rng()%7)-3;
        for(int i=0;i<=n;i++) d[i]=(i==n?0:(i128)a[i+1])-a[i];
        assert(arbitrary_intervals_small(a)==brute_arbitrary(d));
    }
    assert(arbitrary_intervals_small({0,2,2})==1);
    assert(arbitrary_intervals_small({0,1,3,2})==2);
    for(int n=0;n<=7;n++) {
        vector<int>d(1<<n,-1);queue<int>q;d[0]=0;q.push(0);
        while(!q.empty()) {
            int m=q.front();q.pop();
            for(int l=0;l<n;l++)for(int r=l;r<n;r++) {
                int z=m^(((1<<(r-l+1))-1)<<l);
                if(d[z]<0){d[z]=d[m]+1;q.push(z);}
            }
        }
        for(int m=0;m<(1<<n);m++) {
            vector<int>a(n+1);
            for(int i=1;i<=n;i++)a[i]=m>>(i-1)&1;
            assert(binary_interval_flips(a)==d[m]);
        }
    }
    assert(adjacent_moves({0,3,0,0},{0,0,0,3})==6);
    assert(!adjacent_moves({0,1},{0,2}));
    cout << "PASS interval models: exhaustive BFS + independent partitions + constructions\n";
}
#endif
