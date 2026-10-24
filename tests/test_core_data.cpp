#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "../core/basics.hpp"
#include "../core/search_window.hpp"
#include "../data_structure/fenwick.hpp"
#include "../data_structure/segment_tree.hpp"
#include "../data_structure/lazy_segment_tree.hpp"
#include "../data_structure/monotonic.hpp"
#ifndef P0_ONLY
#if __has_include("../data_structure/sparse_table.hpp")
#include "../data_structure/sparse_table.hpp"
#define CORE_DATA_HAS_SPARSE_TABLE
#endif
#endif

void check(bool ok, const string& message) {
    if (!ok) { cerr << "FAIL: " << message << '\n'; exit(1); }
}
ll brute_sum(const vector<ll>& a, int l, int r) {
    ll ans = 0;
    for (int i = l; i <= r; ++i) ans += a[i];
    return ans;
}
mt19937_64 rng(20261024);
ll rnd(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
}

void test_basics() {
    __int128 hi = ((__int128(1) << 126) - 1) * 2 + 1;
    __int128 lo = -hi - 1;
    check(to_string_i128(0) == "0", "i128 zero");
    check(to_string_i128(-123) == "-123", "i128 negative");
    check(to_string_i128(hi) == "170141183460469231731687303715884105727",
          "i128 maximum");
    check(to_string_i128(lo) == "-170141183460469231731687303715884105728",
          "i128 minimum must not negate signed minimum");
    for (vector<ll> empty : {vector<ll>{}, vector<ll>{0}}) {
        PrefixSum p(empty);
        check(p.sum(1, 0) == 0, "empty prefix");
        Compression c(empty);
        check(c.values.size() == 1, "empty compression dummy");
    }
    Difference empty(0);
    empty.add(1, 0, 7);
    check(empty.values() == vector<ll>{0}, "empty difference");
    vector<ll> a{0, 9, -2, 9, LLONG_MAX, LLONG_MIN};
    Compression c(a);
    check(c.rank(LLONG_MIN) == 1 && c.rank(9) == 3,
          "compression duplicate / extrema");
    check(c.values == vector<ll>({0, LLONG_MIN, -2, 9, LLONG_MAX}),
          "compression inverse and one based ranks");
    for (int rep = 0; rep < 120; ++rep) {
        int n = int(rnd(1, 60));
        vector<ll> b(n + 1);
        for (int i = 1; i <= n; ++i) b[i] = rnd(-1000, 1000);
        PrefixSum p(b);
        Difference d(n);
        vector<ll> want(n + 1);
        for (int step = 0; step < 100; ++step) {
            int l = int(rnd(1, n)), r = int(rnd(l, n));
            check(p.sum(l, r) == brute_sum(b, l, r), "prefix differential");
            ll x = rnd(-1000, 1000);
            d.add(l, r, x);
            for (int i = l; i <= r; ++i) want[i] += x;
        }
        check(d.values() == want, "difference differential");
        check(d.values() == want, "materializing difference is repeatable");
    }
}

void test_search_window() {
    check(first_true(1, 10, [](ll x) { return x >= 7; }) == 7,
          "first true interior");
    check(last_true(1, 10, [](ll x) { return x <= 7; }) == 7,
          "last true interior");
    check(!first_true(1, 10, [](ll) { return false; }), "all false");
    check(!last_true(1, 10, [](ll) { return false; }), "all false last");
    check(!first_true(2, 1, [](ll) { abort(); return true; }), "empty search");
    check(!last_true(2, 1, [](ll) { abort(); return true; }), "empty last");
    check(first_true(LLONG_MIN, LLONG_MAX, [](ll x) { return x >= 0; }) == 0,
          "first search avoids midpoint overflow");
    check(last_true(LLONG_MIN, LLONG_MAX, [](ll x) { return x <= 0; }) == 0,
          "last search avoids midpoint overflow");
    check(first_true(LLONG_MAX, LLONG_MAX, [](ll) { return true; })
              == LLONG_MAX, "singleton max");
    check(last_true(LLONG_MIN, LLONG_MIN, [](ll) { return true; })
              == LLONG_MIN, "singleton min");
    check(two_sum_sorted({0, LLONG_MIN, LLONG_MAX}, -1) == pair(1, 2),
          "two sum compares in i128");
    check(longest_sum_at_most({0, 0, 0, 0}, 0) == 3, "zero window");
    check(longest_sum_at_most({0, 1}, -1) == 0, "negative limit");
    check(longest_sum_at_most({}, 5) == 0, "empty window");
    check(two_sum_sorted({}, 0) == pair(0, 0), "empty two pointer");
    for (int rep = 0; rep < 400; ++rep) {
        int n = int(rnd(0, 45));
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) a[i] = rnd(0, 20);
        ll k = rnd(0, 100);
        int want = 0;
        for (int i = 1; i <= n; ++i) {
            ll s = 0;
            for (int j = i; j <= n; ++j) {
                s += a[j];
                if (s <= k) want = max(want, j - i + 1);
            }
        }
        check(longest_sum_at_most(a, k) == want, "window differential");
        sort(a.begin() + 1, a.end());
        bool exists = false;
        for (int i = 1; i <= n; ++i)
            for (int j = i + 1; j <= n; ++j)
                exists |= a[i] + a[j] == k;
        auto [l, r] = two_sum_sorted(a, k);
        check(bool(l) == exists, "two sum existence differential");
        if (l) check(l < r && a[l] + a[r] == k, "two sum witness");
        ll pivot = rnd(-50, 50);
        auto f = first_true(-40, 40, [&](ll x) { return x >= pivot; });
        auto g = last_true(-40, 40, [&](ll x) { return x <= pivot; });
        check(f == (pivot > 40 ? optional<ll>{} : optional<ll>(max(-40LL, pivot))),
              "first true differential");
        check(g == (pivot < -40 ? optional<ll>{} : optional<ll>(min(40LL, pivot))),
              "last true differential");
    }
}

void test_data_structures() {
    Fenwick empty_f(0);
    SegmentTree empty_s(vector<ll>{});
    LazySegmentTree empty_l(vector<ll>{0});
    check(empty_f.sum(0) == 0 && empty_f.range_sum(1, 0) == 0,
          "empty fenwick");
    check(empty_s.sum(1, 0) == 0, "empty segment tree");
    empty_l.add(1, 0, 99);
    check(empty_l.sum(1, 0) == 0, "empty lazy tree");
    vector<ll> overflow{0, 2147483647LL, 1};
    SegmentTree wide_s(overflow);
    LazySegmentTree wide_l(overflow);
    check(wide_s.sum(1, 2) == 2147483648LL, "point tree sum uses ll");
    check(wide_l.sum(1, 2) == 2147483648LL, "legacy int overflow regression");
    LazySegmentTree pending(vector<ll>{0, 0, 0});
    pending.add(1, 2, 1);
    check(pending.sum(1, 1) == 1, "legacy query pushdown regression");
    pending.add(2, 2, 3000000000LL);
    check(pending.sum(1, 2) == 3000000002LL, "ll update and nested lazy");
    Fenwick api(2);
    api.add(1, 5); api.add(2, 7);
    check(api.range_sum(1, 2) == 12, "consistent fenwick range interface");
    for (int rep = 0; rep < 100; ++rep) {
        int n = int(rnd(1, 70));
        vector<ll> a(n + 1), b(n + 1);
        Fenwick f(n);
        for (int i = 1; i <= n; ++i) {
            a[i] = b[i] = rnd(-1000000000, 1000000000);
            f.add(i, a[i]);
        }
        SegmentTree s(a);
        LazySegmentTree lazy(b);
        for (int step = 0; step < 400; ++step) {
            int l = int(rnd(1, n)), r = int(rnd(l, n));
            ll x = rnd(-1000000000, 1000000000);
            if (step % 3 == 0) {
                f.add(l, x - a[l]); a[l] = x; s.set(l, x);
                lazy.add(l, r, x);
                for (int i = l; i <= r; ++i) b[i] += x;
            } else {
                check(f.sum(r) == brute_sum(a, 1, r), "fenwick prefix differential");
                check(f.range_sum(l, r) == brute_sum(a, l, r), "fenwick differential");
                check(s.sum(l, r) == brute_sum(a, l, r), "point tree differential");
                check(lazy.sum(l, r) == brute_sum(b, l, r), "lazy differential");
            }
        }
    }
}

void test_monotonic() {
    check(previous_less({0, 2, 2, 1, 3}) == vector<int>({0, 0, 0, 0, 3}),
          "strict smaller pops equal elements");
    check(previous_less({}) == vector<int>{0}, "empty monotonic stack");
    check(window_min({0, 4, 2, 2, 5}, 2) == vector<ll>({0, 2, 2, 2}),
          "window tie / eviction");
    check(window_min({0, 4, 2}, 0) == vector<ll>{0}, "zero window size");
    check(window_min({0, 4, 2}, 3) == vector<ll>{0}, "oversized window");
    check(window_min({}, 1) == vector<ll>{0}, "empty monotonic queue");
    for (int rep = 0; rep < 300; ++rep) {
        int n = int(rnd(1, 70)), k = int(rnd(1, n));
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) a[i] = rnd(-5, 5);
        vector<int> prev(n + 1);
        for (int i = 1; i <= n; ++i)
            for (int j = i - 1; j >= 1; --j)
                if (a[j] < a[i]) { prev[i] = j; break; }
        check(previous_less(a) == prev, "stack differential");
        vector<ll> want{0};
        for (int l = 1; l + k - 1 <= n; ++l) {
            ll best = LLONG_MAX;
            for (int j = l; j < l + k; ++j) best = min(best, a[j]);
            want.push_back(best);
        }
        check(window_min(a, k) == want, "queue differential");
    }
}

void test_large() {
    constexpr int n = 200000;
    vector<ll> a(n + 1, 1); a[0] = 0;
    PrefixSum p(a);
    Difference diff(n);
    diff.add(1, n, 2);
    check(p.sum(1, n) == n && diff.values()[n] == 2, "large basics");
    Compression compressed(a);
    check(compressed.rank(1) == 1, "large compression");
    Fenwick f(n);
    SegmentTree s(a);
    LazySegmentTree lazy(a);
    for (int i = 1; i <= n; ++i) f.add(i, 1);
    check(f.range_sum(1, n) == n && s.sum(1, n) == n, "large initial sum");
    for (int i = 1; i <= n; ++i) {
        f.add(i, 1); s.set(i, 2);
        lazy.add(i, n, 1);
    }
    check(f.range_sum(1, n) == 2LL * n && s.sum(1, n) == 2LL * n,
          "large point operations");
    check(lazy.sum(1, n) == n + 1LL * n * (n + 1) / 2,
          "large lazy aggregate");
    check(lazy.sum(n, n) == n + 1, "large lazy leaf pushdown");
    check(longest_sum_at_most(a, n / 2) == n / 2, "large sliding window");
    auto prev = previous_less(a);
    check(prev.size() == n + 1 && prev[n] == 0, "large equal stack");
    auto mins = window_min(a, 100000);
    check(mins.size() == 100002 && mins.back() == 1, "large monotonic queue");
}

#ifndef P0_ONLY
void test_sparse_table() {
#ifdef CORE_DATA_HAS_SPARSE_TABLE
    SparseTable<MinOp> empty({}, LLONG_MAX);
    SparseTable<GcdOp> empty_gcd({0}, 0);
    check(empty.query(1, 0) == LLONG_MAX, "empty sparse min identity");
    check(empty_gcd.query(1, 0) == 0, "empty sparse gcd identity");
    SparseTable<MinOp> one({0, LLONG_MIN}, LLONG_MAX);
    check(one.query(1, 1) == LLONG_MIN, "sparse minimum singleton");
    for (int rep = 0; rep < 100; ++rep) {
        int n = int(rnd(1, 70));
        vector<ll> a(n + 1), b(n + 1);
        for (int i = 1; i <= n; ++i) {
            a[i] = rnd(-1000, 1000); b[i] = rnd(0, 1000);
        }
        SparseTable<MinOp> st(a, LLONG_MAX);
        SparseTable<GcdOp> sg(b, 0);
        for (int l = 1; l <= n; ++l) {
            ll mn = LLONG_MAX, g = 0;
            for (int r = l; r <= n; ++r) {
                mn = min(mn, a[r]); g = gcd(g, b[r]);
                check(st.query(l, r) == mn, "sparse min differential");
                check(sg.query(l, r) == g, "sparse gcd differential");
            }
        }
    }
    vector<ll> a(200001);
    iota(a.begin(), a.end(), 0LL);
    SparseTable<MinOp> st(a, LLONG_MAX);
    check(st.query(12345, 200000) == 12345, "large sparse min");
#else
    check(false, "P1 sparse-table header is missing");
#endif
}
#endif

int main() {
    test_basics(); test_search_window(); test_data_structures();
    test_monotonic(); test_large();
#ifndef P0_ONLY
    test_sparse_table();
#endif
    cout << "PASS core/data: deterministic + random(seed=20261024) + n=200000\n";
}
