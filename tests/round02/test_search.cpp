#include <bits/stdc++.h>
using namespace std;
#if !__has_include("../../competition/models/state_search.hpp")
int main() {
    cerr << "RED: state_search.hpp is not implemented\n";
    return 1;
}
#else
#include "../../competition/models/state_search.hpp"

void check_path(int start, int target, int limit, int want = -1) {
    auto path = integer_state_bfs(start, target, limit);
    assert(!path.states.empty() && path.states.front() == start);
    assert(path.states.back() == target);
    assert(path.states.size() == path.operations.size() + 1);
    if(want >= 0) assert((int)path.operations.size() == want);
    for(size_t i = 0; i < path.operations.size(); i++) {
        int x = path.states[i], y = path.states[i + 1];
        assert(0 <= x && x <= limit && 0 <= y && y <= limit);
        char op = path.operations[i];
        assert(op == '+' || op == '-' || op == '*');
        if(op == '+') assert(y == x + 1);
        if(op == '-') assert(y == x - 1);
        if(op == '*') assert(y == x * 2);
    }
}

// All subsets, without recursion or the production pruning bounds.
bool brute_subset(const vector<ll>& a, ll target) {
    int n = (int)a.size() - 1;
    for(int mask = 0; mask < (1 << n); mask++) {
        i128 sum = 0;
        for(int i = 0; i < n; i++) if(mask >> i & 1) sum += a[i+1];
        if(sum == target) return true;
    }
    return false;
}

void check_subset(const vector<ll>& a, ll target) {
    auto chosen = subset_target_nonnegative(a, target);
    assert(chosen.has_value() == brute_subset(a, target));
    if(!chosen) return;
    i128 sum = 0;
    set<int> used;
    for(int id : *chosen) {
        assert(1 <= id && id < (int)a.size());
        assert(used.insert(id).second);
        sum += a[id];
    }
    assert(sum == target);
}

int main() {
    // Floyd independently checks all state-pair distances, including 0*2=0.
    for(int limit = 0; limit <= 30; limit++) {
        vector<vector<int>> d(limit + 1, vector<int>(limit + 1, 100000));
        for(int x = 0; x <= limit; x++) for(int y = 0; y <= limit; y++) {
            if(x == y) d[x][y] = 0;
            else if(y == x + 1 || y == x - 1 || y == 2 * x) d[x][y] = 1;
        }
        for(int k = 0; k <= limit; k++)
            for(int i = 0; i <= limit; i++) for(int j = 0; j <= limit; j++)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        for(int s = 0; s <= limit; s++) for(int t = 0; t <= limit; t++)
            check_path(s, t, limit, d[s][t]);
    }
    check_path(3, 10, 20, 3);
    check_path(0, 1000000, 1000000);
    check_path(1000000, 1000000, 1000000, 0);
    for(int n = 0; n <= 6; n++) {
        int cases = 1;
        for(int i = 0; i < n; i++) cases *= 3;
        for(int state = 0; state < cases; state++) {
            vector<ll> a(n + 1); int code = state, sum = 0;
            for(int i = 1; i <= n; i++) {
                a[i] = code % 3; code /= 3; sum += (int)a[i];
            }
            for(int target = 0; target <= sum + 1; target++) check_subset(a, target);
        }
    }
    mt19937 rng(20261024);
    for(int tc = 0; tc < 300; tc++) {
        int n = (int)(rng() % 15);
        vector<ll> a(n + 1);
        for(int i = 1; i <= n; i++) a[i] = rng() % 21;
        check_subset(a, rng() % 101);
    }
    check_subset({0,LLONG_MAX,LLONG_MAX}, LLONG_MAX);
    check_subset({0,0,0,0}, 0);
    check_subset({0,3,3}, 6);
    check_subset({0,2,4}, 3);
    vector<ll> zeros(41);
    assert(subset_target_nonnegative(zeros, 0)->empty());
    assert(!subset_target_nonnegative(zeros, 1));
    cout << "PASS search: all-pairs Floyd, path replay, exhaustive subsets\n";
}
#endif
