#include <bits/stdc++.h>
using namespace std;
#if !__has_include("../../competition/models/greedy_construct.hpp")
int main() {
    cerr << "RED: greedy_construct.hpp is not implemented\n";
    return 1;
}
#else
#include "../../competition/models/greedy_construct.hpp"

// Catches earliest-start/longest-first choices and wrong touching-endpoint rules.
int brute_intervals(const vector<pair<ll,ll>>& seg) {
    int n = (int)seg.size() - 1, best = 0;
    for(int mask = 0; mask < (1 << n); mask++) {
        bool ok = true;
        for(int i = 1; i <= n; i++) if(mask >> (i - 1) & 1)
            for(int j = i + 1; j <= n; j++) if(mask >> (j - 1) & 1)
                if(max(seg[i].first, seg[j].first)
                        < min(seg[i].second, seg[j].second)) ok = false;
        if(ok) best = max(best, __builtin_popcount((unsigned)mask));
    }
    return best;
}

void check_intervals(const vector<pair<ll,ll>>& seg) {
    auto chosen = interval_schedule(seg);
    assert((int)chosen.size() == brute_intervals(seg));
    set<int> used;
    for(int id : chosen) {
        assert(1 <= id && id < (int)seg.size());
        assert(used.insert(id).second);
    }
    for(int x : chosen) for(int y : chosen) if(x != y)
        assert(max(seg[x].first, seg[y].first)
               >= min(seg[x].second, seg[y].second));
}

// Independently enumerate all perfect matchings; no sorting in the oracle.
i128 brute_pairing(const vector<ll>& a, int mask) {
    int n = (int)a.size() - 1;
    if(mask == (1 << n) - 1) return -((i128)1 << 120);
    int i = 0;
    while(mask >> i & 1) i++;
    i128 best = (i128)1 << 120;
    for(int j = i + 1; j < n; j++) if(!(mask >> j & 1)) {
        i128 rest = brute_pairing(a, mask | (1 << i) | (1 << j));
        best = min(best, max((i128)a[i+1] + a[j+1], rest));
    }
    return best;
}

void check_pairing(const vector<ll>& a) {
    vector<pair<ll,ll>> pairs{{123,456}};
    i128 got = minmax_pair_sum(a, pairs);
    assert(got == (a.size() == 1 ? 0 : brute_pairing(a, 0)));
    assert(pairs.size() * 2 + 1 == a.size());
    vector<ll> used;
    i128 high = -((i128)1 << 120);
    for(auto [x,y] : pairs) {
        used.push_back(x); used.push_back(y);
        high = max(high, (i128)x + y);
    }
    vector<ll> want(a.begin() + 1, a.end());
    sort(used.begin(), used.end()); sort(want.begin(), want.end());
    assert(used == want);
    assert(a.size() == 1 || high == got);
}

// A stack oracle catches balanced totals with a negative prefix, e.g. ")(".
bool stack_brackets(const string& s) {
    vector<char> st;
    for(char c : s) {
        if(c == '(') st.push_back(c);
        else if(c == ')') {
            if(st.empty()) return false;
            st.pop_back();
        } else return false;
    }
    return st.empty();
}

int main() {
    mt19937 rng(20261024);
    check_intervals({{0,0}});
    check_intervals({{0,0},{1,10},{2,3},{3,4},{4,5}});
    check_intervals({{0,0},{LLONG_MIN,-1},{-1,LLONG_MAX}});
    for(int tc = 0; tc < 700; tc++) {
        int n = (int)(rng() % 11);
        vector<pair<ll,ll>> seg(n + 1);
        for(int i = 1; i <= n; i++) {
            ll l = (int)(rng() % 15) - 7;
            seg[i] = {l, l + 1 + (ll)(rng() % 6)};
        }
        check_intervals(seg);
    }
    for(int n = 0; n <= 8; n += 2) {
        int cases = 1;
        for(int i = 0; i < n; i++) cases *= 3;
        for(int state = 0; state < cases; state++) {
            int code = state;
            vector<ll> a(n + 1);
            for(int i = 1; i <= n; i++) { a[i] = code % 3 - 1; code /= 3; }
            check_pairing(a);
        }
    }
    check_pairing({0,LLONG_MIN,LLONG_MIN,LLONG_MAX,LLONG_MAX});
    check_pairing({0,LLONG_MAX,LLONG_MAX});
    check_pairing({0,LLONG_MIN,LLONG_MIN});
    check_pairing({0,-5,-2,3,8});
    for(int n = 0; n <= 16; n++) for(int mask = 0; mask < (1 << n); mask++) {
        string s(n, '(');
        for(int i = 0; i < n; i++) if(mask >> i & 1) s[i] = ')';
        assert(regular_brackets(s) == stack_brackets(s));
    }
    assert(!regular_brackets("(x)"));
    assert(regular_brackets(string(100000, '(') + string(100000, ')')));
    vector<pair<ll,ll>> big(200001);
    for(int i = 1; i <= 200000; i++) big[i] = {i - 1, i};
    assert(interval_schedule(big).size() == 200000);
    vector<ll> equal(200001, -7); equal[0] = 0;
    vector<pair<ll,ll>> pairs;
    assert(minmax_pair_sum(equal, pairs) == -14 && pairs.size() == 100000);
    cout << "PASS greedy: subsets, all matchings, bracket stacks, boundaries\n";
}
#endif
