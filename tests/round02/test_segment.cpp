#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#if !__has_include("../../competition/data_structure/fenwick.hpp")
int main() {
    cerr << "FAIL: personal Fenwick and segment recipes are not implemented\n";
    return 1;
}
#else
namespace old_ds {
#include "../../data_structure/fenwick.hpp"
#include "../../data_structure/segment_tree.hpp"
#include "../../data_structure/lazy_segment_tree.hpp"
}
namespace personal_fenwick {
#include "../../competition/data_structure/fenwick.hpp"
}
namespace point_sum {
#include "../../competition/data_structure/segment_point_sum.hpp"
}
namespace point_min {
#include "../../competition/data_structure/segment_point_min.hpp"
}
namespace point_max {
#include "../../competition/data_structure/segment_point_max.hpp"
}
namespace point_gcd {
#include "../../competition/data_structure/segment_point_gcd.hpp"
}
namespace point_count {
#include "../../competition/data_structure/segment_max_count.hpp"
}
namespace point_subarray {
#include "../../competition/data_structure/segment_max_subarray.hpp"
}
namespace point_frequency {
#include "../../competition/data_structure/segment_frequency.hpp"
}
namespace point_stats {
#include "../../competition/data_structure/segment_point_stats.hpp"
}
namespace range_add {
#include "../../competition/data_structure/segment_range_add.hpp"
}
namespace range_set_add {
#include "../../competition/data_structure/segment_set_add.hpp"
}

void check(bool ok, const string& message) {
    if (!ok) { cerr << "FAIL: " << message << '\n'; exit(1); }
}
mt19937_64 rng(20261024);
ll rnd(ll lo, ll hi) {
    return uniform_int_distribution<ll>(lo, hi)(rng);
}
ll brute_sum(const vector<ll>& a, int l, int r) {
    ll ans = 0;
    for (int i = l; i <= r; ++i) ans += a[i];
    return ans;
}
ll brute_gcd(const vector<ll>& a, int l, int r) {
    ll ans = 0;
    for (int i = l; i <= r; ++i) {
        ll x = a[i] < 0 ? -a[i] : a[i];
        while (x) { ll rem = ans % x; ans = x; x = rem; }
    }
    return ans;
}
ll brute_best(const vector<ll>& a, int l, int r) {
    ll ans = LLONG_MIN;
    for (int i = l; i <= r; ++i) {
        ll sum = 0;
        for (int j = i; j <= r; ++j) {
            sum += a[j];
            ans = max(ans, sum);
        }
    }
    return ans;
}
void build_points(const vector<ll>& a) {
    int n = int(a.size()) - 1;
    for (int i = 1; i <= n; ++i) {
        point_sum::a[i] = point_min::a[i] = point_max::a[i] = a[i];
        point_gcd::a[i] = point_count::a[i] = a[i];
        point_subarray::a[i] = point_stats::a[i] = a[i];
    }
    point_sum::build(1, 1, n); point_min::build(1, 1, n);
    point_max::build(1, 1, n); point_gcd::build(1, 1, n);
    point_count::build(1, 1, n); point_subarray::build(1, 1, n);
    point_stats::build(1, 1, n);
}
void modify_points(int n, int pos, ll val) {
    point_sum::modify(1, 1, n, pos, val);
    point_min::modify(1, 1, n, pos, val);
    point_max::modify(1, 1, n, pos, val);
    point_gcd::modify(1, 1, n, pos, val);
    point_count::modify(1, 1, n, pos, val);
    point_subarray::modify(1, 1, n, pos, val);
    point_stats::modify(1, 1, n, pos, val);
}
void check_points(const vector<ll>& a, int l, int r) {
    int n = int(a.size()) - 1;
    ll sum = brute_sum(a, l, r), g = brute_gcd(a, l, r);
    ll mn = *min_element(a.begin() + l, a.begin() + r + 1);
    ll mx = *max_element(a.begin() + l, a.begin() + r + 1);
    int cnt = int(count(a.begin() + l, a.begin() + r + 1, mx));
    check(point_sum::query(1, 1, n, l, r).sum == sum, "point sum");
    check(point_min::query(1, 1, n, l, r).mn == mn, "point min");
    check(point_max::query(1, 1, n, l, r).mx == mx, "point max");
    check(point_gcd::query(1, 1, n, l, r).g == g, "point signed gcd");
    auto mc = point_count::query(1, 1, n, l, r);
    check(mc.mx == mx && mc.cnt == cnt, "maximum and multiplicity");
    auto ms = point_subarray::query(1, 1, n, l, r);
    check(ms.best == brute_best(a, l, r), "nonempty max subarray");
    ll pref = LLONG_MIN, suff = LLONG_MIN;
    for (int i = l; i <= r; ++i) pref = max(pref, brute_sum(a, l, i));
    for (int i = l; i <= r; ++i) suff = max(suff, brute_sum(a, i, r));
    check(ms.sum == sum && ms.pref == pref && ms.suff == suff,
          "subarray boundary states");
    auto st = point_stats::query(1, 1, n, l, r);
    check(st.sum == sum && st.mn == mn && st.mx == mx && st.g == g,
          "all combined statistics");
    ll threshold = rnd(-35, 35);
    int want = -1;
    for (int i = l; i <= r; ++i)
        if (a[i] >= threshold) { want = i; break; }
    check(point_max::first_ge(1, 1, n, l, r, threshold) == want,
          "first threshold hit restricted to query range");
}
void test_point_and_old_versions() {
    for (const vector<ll>& a : {vector<ll>{0, -5},
             vector<ll>{0, -7, -2, -9}, vector<ll>{0, 0, 0, 0},
             vector<ll>{0, 5, 5, 1, 5}, vector<ll>{0, 3, -2, 4}}) {
        build_points(a);
        for (int l = 1; l < int(a.size()); ++l)
            for (int r = l; r < int(a.size()); ++r) check_points(a, l, r);
    }
    // Catch stale nodes on rebuild, wrong assignment/add interpretation,
    // negative identities, incorrect crossing-subarray merge and lost ties.
    for (int rep = 0; rep < 150; ++rep) {
        int n = int(rnd(1, 55));
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) a[i] = rnd(-30, 30);
        build_points(a);
        old_ds::Fenwick old_f(n);
        personal_fenwick::Fenwick f(n);
        old_ds::SegmentTree old_seg(a);
        for (int i = 1; i <= n; ++i) { f.add(i, a[i]); old_f.add(i, a[i]); }
        check(f.query(0) == 0 && f.rangeSum(1, 0) == 0, "Fenwick empty prefix");
        for (int step = 0; step < 180; ++step) {
            int l = int(rnd(1, n)), r = int(rnd(l, n));
            if (step % 3 == 0) {
                ll val = rnd(-30, 30), delta = val - a[l];
                f.add(l, delta); old_f.add(l, delta);
                old_seg.set(l, val); modify_points(n, l, val); a[l] = val;
            } else {
                ll want = brute_sum(a, l, r);
                check(f.rangeSum(l, r) == want && old_f.range_sum(l, r) == want,
                      "PSD01/DS01 same stream and independent oracle");
                check(f.query(r) == brute_sum(a, 1, r), "Fenwick prefix API");
                check(old_seg.sum(l, r) == want, "PSD02/DS02 same stream");
                check_points(a, l, r);
            }
        }
    }
    personal_fenwick::Fenwick empty(0);
    check(empty.query(0) == 0 && empty.rangeSum(1, 0) == 0, "Fenwick n=0");
    point_subarray::a[1] = -5; point_subarray::a[2] = -2;
    point_subarray::build(1, 1, 2);
    check(max(0LL, point_subarray::query(1, 1, 2, 1, 2).best) == 0,
          "documented allow-empty output adapter");
    point_min::a[1] = LLONG_MIN; point_min::build(1, 1, 1);
    point_max::a[1] = LLONG_MAX; point_max::build(1, 1, 1);
    point_gcd::a[1] = -LLONG_MAX; point_gcd::build(1, 1, 1);
    check(point_min::query(1, 1, 1, 1, 1).mn == LLONG_MIN, "min extreme");
    check(point_max::query(1, 1, 1, 1, 1).mx == LLONG_MAX, "max extreme");
    check(point_gcd::query(1, 1, 1, 1, 1).g == LLONG_MAX,
          "gcd largest representable magnitude");
    point_min::a[1] = 5000000000000000000LL;
    point_min::a[2] = 6000000000000000000LL;
    point_min::build(1, 1, 2);
    check(point_min::query(1, 1, 2, 1, 1).mn == 5000000000000000000LL,
          "generic min must not use a finite 4e18 identity");
}
void test_frequency() {
    for (int rep = 0; rep < 200; ++rep) {
        int n = int(rnd(1, 70));
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i)
            point_frequency::a[i] = a[i] = rnd(0, 5);
        point_frequency::build(1, 1, n);
        for (int step = 0; step < 160; ++step) {
            int pos = int(rnd(1, n));
            a[pos] = rnd(0, 5);
            point_frequency::modify(1, 1, n, pos, a[pos]);
            ll total = brute_sum(a, 1, n), k = rnd(0, total + 1);
            ll prefix = 0; int want = -1;
            if (1 <= k && k <= total)
                for (int i = 1; i <= n; ++i) {
                    prefix += a[i];
                    if (prefix >= k) { want = i; break; }
                }
            check(point_frequency::kth(1, 1, n, k) == want,
                  "nonnegative frequencies / first and last / invalid k");
            check(point_frequency::query(1, 1, n, 1, n).sum == total,
                  "frequency total");
        }
    }
    for (int i = 1; i <= 4; ++i) point_frequency::a[i] = 0;
    point_frequency::build(1, 1, 4);
    check(point_frequency::kth(1, 1, 4, 1) == -1, "all zero frequencies");
    point_max::a[1] = 8; point_max::a[2] = 1; point_max::a[3] = 8;
    point_max::build(1, 1, 3);
    check(point_max::first_ge(1, 1, 3, 2, 3, 8) == 3,
          "threshold must not return a hit before L");
    check(point_max::first_ge(1, 1, 3, 2, 2, 8) == -1, "no threshold hit");
    check(point_max::first_ge(1, 1, 3, 3, 2, 8) == -1, "empty search range");
}
void test_lazy_add() {
    for (int rep = 0; rep < 180; ++rep) {
        int n = int(rnd(1, 75));
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) range_add::a[i] = a[i] = rnd(-100, 100);
        range_add::build(1, 1, n);
        old_ds::LazySegmentTree old_seg(a);
        for (int step = 0; step < 250; ++step) {
            int l = int(rnd(1, n)), r = int(rnd(l, n));
            if (step % 2 == 0) {
                ll val = rnd(-1000000, 1000000);
                range_add::modify(1, 1, n, l, r, val);
                old_seg.add(l, r, val);
                for (int i = l; i <= r; ++i) a[i] += val;
            } else {
                auto ans = range_add::query(1, 1, n, l, r);
                check(ans.sum == brute_sum(a, l, r)
                          && old_seg.sum(l, r) == brute_sum(a, l, r),
                      "PSD10/DS03 same lazy stream / independent oracle");
                check(ans.mn == *min_element(a.begin() + l, a.begin() + r + 1),
                      "range add minimum");
                check(ans.mx == *max_element(a.begin() + l, a.begin() + r + 1),
                      "range add maximum");
            }
        }
    }
    range_add::a[1] = range_add::a[2] = 0;
    range_add::build(1, 1, 2);
    range_add::modify(1, 1, 2, 1, 2, 1);
    check(range_add::query(1, 1, 2, 1, 1).sum == 1,
          "legacy partial query must push down");
    range_add::modify(1, 1, 2, 1, 2, 3000000000LL);
    check(range_add::query(1, 1, 2, 1, 2).sum == 6000000002LL,
          "legacy int sum overflow");
    range_add::a[1] = range_add::a[2] = -4;
    range_add::build(1, 1, 2);
    check(range_add::query(1, 1, 2, 2, 2).sum == -4, "rebuild clears lazy");
}
void test_set_add() {
    auto rebuild = [] {
        for (int i = 1; i <= 4; ++i) range_set_add::a[i] = i;
        range_set_add::build(1, 1, 4);
    };
    rebuild();
    range_set_add::modify(1, 1, 4, 1, 4, 0, true);
    range_set_add::modify(1, 1, 4, 2, 3, 5, false);
    check(range_set_add::query(1, 1, 4, 1, 4) == 10, "set zero then add");
    check(range_set_add::query(1, 1, 4, 1, 1) == 0, "has_set distinguishes zero");
    rebuild();
    range_set_add::modify(1, 1, 4, 1, 4, 5, false);
    range_set_add::modify(1, 1, 4, 2, 3, 0, true);
    check(range_set_add::query(1, 1, 4, 1, 4) == 15, "add then set zero");
    rebuild();
    range_set_add::modify(1, 1, 4, 1, 4, 7, true);
    range_set_add::modify(1, 1, 4, 1, 4, -2, false);
    check(range_set_add::query(1, 1, 4, 2, 3) == 10,
          "pushdown set before pending add");
    range_set_add::modify(1, 1, 4, 1, 4, 3, true);
    check(range_set_add::query(1, 1, 4, 2, 2) == 3, "new set erases old add");
    rebuild();
    check(range_set_add::query(1, 1, 4, 2, 2) == 2, "multi-test clears all tags");
    for (int rep = 0; rep < 220; ++rep) {
        int n = int(rnd(1, 70));
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) range_set_add::a[i] = a[i] = rnd(-100, 100);
        range_set_add::build(1, 1, n);
        for (int step = 0; step < 300; ++step) {
            int l = int(rnd(1, n)), r = int(rnd(l, n));
            int op = int(rnd(0, 2)); ll val = rnd(-1000, 1000);
            if (op < 2) {
                bool is_set = op == 0;
                range_set_add::modify(1, 1, n, l, r, val, is_set);
                for (int i = l; i <= r; ++i)
                    a[i] = is_set ? val : a[i] + val;
            } else {
                check(range_set_add::query(1, 1, n, l, r) == brute_sum(a, l, r),
                      "mixed assign/add random oracle");
            }
        }
    }
}
void test_capacity() {
    constexpr int n = 200000;
    for (int i = 1; i <= n; ++i) {
        point_sum::a[i] = 1; range_add::a[i] = 1;
        range_set_add::a[i] = 1;
    }
    point_sum::build(1, 1, n); range_add::build(1, 1, n);
    range_set_add::build(1, 1, n);
    point_sum::modify(1, 1, n, n, 3000000000LL);
    range_add::modify(1, 1, n, 1, n, 3000000000LL);
    range_set_add::modify(1, 1, n, 1, n, 3000000000LL, true);
    check(point_sum::query(1, 1, n, 1, n).sum == n - 1 + 3000000000LL,
          "point capacity boundary");
    check(range_add::query(1, 1, n, n, n).sum == 3000000001LL,
          "lazy capacity rightmost leaf");
    check(range_set_add::query(1, 1, n, 1, n) == 600000000000000LL,
          "set length product uses ll");
}
int main() {
    test_point_and_old_versions(); test_frequency(); test_lazy_add();
    test_set_add(); test_capacity();
    cout << "PASS round02 segment: 14 models; seed=20261024; "
            "DS01/DS02/DS03 consistency; lazy composition; n=200000\n";
}
#endif
