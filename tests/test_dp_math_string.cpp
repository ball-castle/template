#include <bits/stdc++.h>
#include "../dp/knapsack.hpp"
#include "../dp/lis.hpp"
#include "../dp/hamilton_path.hpp"
#include "../math/integer_bits.hpp"
#include "../math/modular.hpp"
#ifndef P0_ONLY
#include "../dp/interval_merge.hpp"
#include "../math/matrix_power.hpp"
#include "../math/product_recurrence.hpp"
#include "../math/combinations.hpp"
#include "../string/kmp_z.hpp"
#include "../string/double_hash.hpp"
#include "../string/basic_string.hpp"
#endif
using namespace std;
using ll = long long;
mt19937_64 rng(20261024);
int rnd(int l, int r) { return l + int(rng() % (r - l + 1)); }
void check(bool ok, const string& name) {
    if (!ok) { cerr << "FAIL: " << name << '\n'; exit(1); }
}

// Enumerate item multiplicities: independent of the DP capacity order.
vector<ll> brute_bag(const vector<int>& w, const vector<ll>& v,
                     int cap, bool repeat, bool exact) {
    vector<ll> ans(cap + 1, KNAP_NEG);
    function<void(int, int, ll)> dfs = [&](int i, int used, ll value) {
        if (i == int(w.size())) {
            ans[used] = max(ans[used], value);
            return;
        }
        int limit = min(repeat ? cap : 1, (cap - used) / w[i]);
        for (int k = 0; k <= limit; ++k)
            dfs(i + 1, used + k * w[i], value + k * v[i]);
    };
    dfs(1, 0, 0);
    if (!exact)
        for (int c = 1; c <= cap; ++c) ans[c] = max(ans[c], ans[c - 1]);
    return ans;
}

void test_knapsack() {
    check(knapsack01({0, 2}, {0, 3}, 4).back() == 3,
          "01 item must not repeat");
    check(knapsack_complete({0, 2}, {0, 3}, 4).back() == 6,
          "complete item must repeat");
    check(knapsack01({0, 2}, {0, 5}, 3, true).back() == KNAP_NEG,
          "exact unreachable must not inherit zero");
    check(knapsack01({0, 1}, {0, -5}, 1, true).back() == -5,
          "exact negative value must stay reachable");
    check(knapsack01({0}, {0}, 0).back() == 0, "empty bag");
    check(knapsack01({0, 1, 1}, {0, 3000000000LL, 3000000000LL},
                     2).back() == 6000000000LL, "64-bit bag value");
    for (int rep = 0; rep < 600; ++rep) {
        int n = rnd(0, 4), cap = rnd(0, 12);
        vector<int> w(n + 1);
        vector<ll> v(n + 1);
        for (int i = 1; i <= n; ++i) w[i] = rnd(1, 6), v[i] = rnd(-8, 20);
        for (bool exact : {false, true}) {
            check(knapsack01(w, v, cap, exact) ==
                  brute_bag(w, v, cap, false, exact), "random 01 bag");
            check(knapsack_complete(w, v, cap, exact) ==
                  brute_bag(w, v, cap, true, exact), "random complete bag");
        }
    }
}

int brute_lis(const vector<ll>& a, bool strict) {
    int n = int(a.size()) - 1, ans = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
        ll last = LLONG_MIN;
        bool seen = false, ok = true;
        for (int i = 1; i <= n; ++i) if (mask >> (i - 1) & 1) {
            if (seen && (strict ? last >= a[i] : last > a[i])) ok = false;
            last = a[i]; seen = true;
        }
        if (ok) ans = max(ans, __builtin_popcount(unsigned(mask)));
    }
    return ans;
}

void test_lis() {
    check(lis_length({0}) == 0, "empty LIS");
    check(lis_length({0, 7, 7, 7}) == 1, "strict duplicate LIS");
    check(lis_length({0, 7, 7, 7}, false) == 3, "nonstrict LIS");
    for (int rep = 0; rep < 350; ++rep) {
        vector<ll> a(rnd(0, 11) + 1);
        for (int i = 1; i < int(a.size()); ++i) a[i] = rnd(-4, 4);
        for (bool strict : {false, true}) {
            int want = brute_lis(a, strict);
            check(lis_length(a, strict) == want, "random fast LIS");
            check(lis_quadratic(a, strict) == want, "random basic LIS");
        }
    }
}

ll brute_hamilton(const vector<vector<ll>>& w) {
    int n = int(w.size()) - 1;
    if (n <= 1) return 0;
    vector<int> middle;
    for (int v = 2; v < n; ++v) middle.push_back(v);
    ll ans = HAMILTON_INF;
    do {
        vector<int> p = {1};
        p.insert(p.end(), middle.begin(), middle.end()); p.push_back(n);
        ll cost = 0;
        bool ok = true;
        for (int i = 1; i < n; ++i) {
            if (w[p[i - 1]][p[i]] == HAMILTON_INF) { ok = false; break; }
            cost += w[p[i - 1]][p[i]];
        }
        if (ok) ans = min(ans, cost);
    } while (next_permutation(middle.begin(), middle.end()));
    return ans;
}

void test_hamilton() {
    check(hamilton_path(vector<vector<ll>>(1, vector<ll>(1))) == 0,
          "zero-vertex Hamilton convention");
    check(hamilton_path(vector<vector<ll>>(2, vector<ll>(2))) == 0,
          "one-vertex Hamilton");
    for (int rep = 0; rep < 250; ++rep) {
        int n = rnd(2, 8);
        vector<vector<ll>> w(n + 1, vector<ll>(n + 1, HAMILTON_INF));
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                if (i != j && rnd(0, 4)) w[i][j] = rnd(-9, 30);
        check(hamilton_path(w) == brute_hamilton(w), "random Hamilton");
    }
}

void test_integer_bits() {
    check(gcd_abs(-6, -4) == 2, "negative gcd");
    check(gcd_abs(0, 0) == 0, "zero gcd");
    check(gcd_abs(LLONG_MIN, 0) == (1ULL << 63), "LLONG_MIN gcd");
    check(lcm_checked(-6, 4) == optional<ll>(12), "negative lcm");
    check(lcm_checked(0, LLONG_MIN) == optional<ll>(0), "zero lcm");
    check(!lcm_checked(LLONG_MAX, 2), "lcm overflow rejected");
    check(!lcm_checked(LLONG_MIN, 1), "lcm outside signed 64-bit");
    check(lowbit(0) == 0 && lowbit(12) == 4, "unsigned lowbit");
    check(lowbit(1ULL << 63) == (1ULL << 63), "highest unsigned bit");
    check(bit_count(0) == 0 && bit_count(~0ULL) == 64, "popcount boundaries");
    check(lowest_bit_index(0) == -1, "empty bitset has no index");
    check(lowest_bit_index(1ULL << 63) == 63, "highest bit index");
    for (unsigned long long mask = 0; mask < 128; ++mask) {
        vector<unsigned long long> actual;
        for (auto sub = mask;; sub = (sub - 1) & mask) {
            actual.push_back(sub);
            if (sub == 0) break;
        }
        vector<unsigned long long> expected;
        for (auto sub = mask + 1; sub-- > 0;)
            if ((sub & mask) == sub) expected.push_back(sub);
        check(actual == expected, "submask loop includes zero once");
    }
}

void test_modular() {
    check(mod_pow(-2, 3, 5) == 2, "negative modular base");
    check(mod_pow(3, 0, 1) == 0, "exponent zero modulus one");
    check(mod_norm(LLONG_MIN, LLONG_MAX) == LLONG_MAX - 1,
          "LLONG_MIN normalization");
    check(mod_mul(LLONG_MAX - 1, LLONG_MAX - 1, LLONG_MAX) == 1,
          "128-bit modular product");
    check(mod_pow(0, 0, 7) == 1, "zero power zero convention");
    check(mod_pow(LLONG_MAX - 1, 2, LLONG_MAX) == 1,
          "64-bit modular exponentiation");
    check(mod_add(LLONG_MAX - 1, LLONG_MAX - 1, LLONG_MAX) ==
          LLONG_MAX - 2, "addition does not overflow");
    for (int rep = 0; rep < 1500; ++rep) {
        ll a = rnd(-1000, 1000), m = rnd(1, 10000);
        int e = rnd(0, 25);
        ll want = 1 % m;
        for (int i = 0; i < e; ++i) want = (want * a % m + m) % m;
        check(mod_pow(a, e, m) == want, "random modular power");
    }
}

#ifndef P0_ONLY
ll brute_merge(vector<ll> piles) {
    if (piles.size() <= 1) return 0;
    ll ans = LLONG_MAX;
    for (int i = 0; i + 1 < int(piles.size()); ++i) {
        vector<ll> next = piles;
        ll cost = next[i] + next[i + 1];
        next[i] = cost; next.erase(next.begin() + i + 1);
        ans = min(ans, cost + brute_merge(next));
    }
    return ans;
}

void test_interval_merge() {
    check(interval_merge({0}) == 0, "empty interval merge");
    check(interval_merge({0, 17}) == 0, "single pile merge");
    check(interval_merge({0, 1000000000, 1000000000}) == 2000000000,
          "interval INF must exceed valid cost");
    check(interval_merge({0, 2000000000, 2000000000}) == 4000000000LL,
          "interval prefix sums use 64-bit");
    for (int rep = 0; rep < 180; ++rep) {
        vector<ll> a(rnd(0, 7) + 1);
        for (int i = 1; i < int(a.size()); ++i) a[i] = rnd(0, 20);
        vector<ll> plain(a.begin() + 1, a.end());
        check(interval_merge(a) == brute_merge(plain),
              "interval DP versus all adjacent merge orders");
    }
}

void test_matrix_recurrence() {
    check(matrix_pow({}, 0, 7).empty(), "empty matrix");
    check(matrix_pow({{9}}, 0, 1) == Matrix({{0}}), "matrix modulus one");
    check(matrix_pow({{3}}, 0, 7) == Matrix({{1}}), "matrix identity");
    check(matrix_pow({{LLONG_MAX - 1}}, 2, LLONG_MAX) == Matrix({{1}}),
          "wide matrix product");
    for (int rep = 0; rep < 200; ++rep) {
        int n = rnd(1, 4), e = rnd(0, 8);
        ll mod = rnd(1, 10000);
        Matrix a(n, vector<ll>(n)), want(n, vector<ll>(n));
        for (int i = 0; i < n; ++i) {
            want[i][i] = 1 % mod;
            for (ll& x : a[i]) x = rnd(-100, 100);
        }
        for (int step = 0; step < e; ++step) {
            Matrix next(n, vector<ll>(n));
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    for (int k = 0; k < n; ++k) {
                        ll term = (want[i][k] * a[k][j] % mod + mod) % mod;
                        next[i][j] = (next[i][j] + term) % mod;
                    }
            want = next;
        }
        check(matrix_pow(a, e, mod) == want, "random matrix power");
    }
    const ll p = 1000000007;
    for (int rep = 0; rep < 200; ++rep) {
        ll f1 = rnd(0, 50), f2 = rnd(0, 50), f3 = rnd(0, 50);
        ll c = rnd(0, 50);
        vector<ll> f = {0, f1, f2, f3};
        for (int n = 4; n <= 24; ++n) {
            ll cpow = 1;
            for (int j = 0; j < 2 * n - 6; ++j) cpow = cpow * c % p;
            f.push_back(f[n - 1] * f[n - 2] % p * f[n - 3] % p * cpow % p);
        }
        for (int n = 1; n <= 24; ++n)
            check(product_recurrence(n, f1, f2, f3, c) == f[n],
                  "1182E recurrence versus direct recurrence");
    }
    check(product_recurrence(4, 1, 1, 1, 1) == 1, "legacy zero product");
    check(product_recurrence(2, 2, 3, 5, 7) == 3, "n below three");
    check(product_recurrence(1000000000000000000ULL, 0, 2, 3, 4) == 0,
          "zero base with reduced exponent");
    check(product_recurrence(4, p, 2, 3, 4) == 0, "base divisible by p");
    check(product_recurrence(5, 1, 2, 5, 3) == 72900, "1182E sample one");
    check(product_recurrence(17, 97, 41, 37, 11) == 317451037,
          "1182E sample two");
    check(product_recurrence(1000000000000000000ULL, 1, 1, 1, 1) == 1,
          "large recurrence exponent");
    check(product_recurrence(1, -1, 2, 3, 4) == p - 1,
          "recurrence base normalization");
}

void test_combinations() {
    PrimeComb comb(30, 1000000007);
    vector<vector<ll>> pascal(31, vector<ll>(31));
    for (int n = 0; n <= 30; ++n) {
        pascal[n][0] = pascal[n][n] = 1;
        for (int k = 1; k < n; ++k)
            pascal[n][k] = pascal[n - 1][k - 1] + pascal[n - 1][k];
        for (int k = -1; k <= n + 1; ++k)
            check(comb.choose(n, k) == (k < 0 || k > n ? 0 : pascal[n][k]),
                  "combinations versus Pascal triangle");
    }
    PrimeComb zero(0, 2);
    check(zero.choose(0, 0) == 1, "C(0,0)");
    PrimeComb small(6, 7);
    check(small.choose(6, 3) == 6, "prime near preprocessing bound");
    PrimeComb wide(8, 2305843009213693951LL);
    check(wide.choose(8, 4) == 70, "large prime multiplication");
}

vector<int> naive_search(const string& s, const string& p) {
    vector<int> ans;
    for (int i = 0; i + int(p.size()) <= int(s.size()); ++i)
        if (s.compare(i, p.size(), p) == 0) ans.push_back(i);
    return ans;
}

string random_string(int n) {
    string s;
    while (n--) s.push_back(char('a' + rnd(0, 2)));
    return s;
}

void test_strings() {
    check(kmp_find_all("aaa", "aa") == vector<int>({0, 1}),
          "overlapping KMP matches");
    check(kmp_find_all("a", "") == vector<int>({0, 1}), "empty KMP pattern");
    check(z_function("").empty(), "empty Z function");
    for (int rep = 0; rep < 1200; ++rep) {
        string s = random_string(rnd(0, 35)), p = random_string(rnd(0, 10));
        check(kmp_find_all(s, p) == naive_search(s, p), "random KMP");
        auto pi = prefix_function(s), z = z_function(s);
        for (int i = 0; i < int(s.size()); ++i) {
            int expected_pi = 0, expected_z = 0;
            for (int len = 1; len <= i; ++len)
                if (s.substr(0, len) == s.substr(i + 1 - len, len))
                    expected_pi = len;
            if (i > 0)
                while (i + expected_z < int(s.size()) &&
                       s[expected_z] == s[i + expected_z]) ++expected_z;
            check(pi[i] == expected_pi, "random prefix function");
            check(z[i] == expected_z, "random Z function");
        }
        DoubleHash hs(s), hp(p);
        for (int q = 0; q < 15; ++q) {
            int l = rnd(0, int(s.size())), r = rnd(l, int(s.size()));
            DoubleHash separate(s.substr(l, r - l));
            check(hs.get(l, r) == separate.get(0, r - l), "substring hash");
        }
        for (int i = 0; i + int(p.size()) <= int(s.size()); ++i)
            check((hs.get(i, i + int(p.size())) == hp.get(0, int(p.size()))) ==
                  (s.compare(i, p.size(), p) == 0), "sample hash equality");
    }
    string bytes = string(1, char(255)) + string(1, '\0') + "a";
    DoubleHash h(bytes), one(string(1, char(255)));
    check(h.get(0, 1) == one.get(0, 1), "unsigned byte hashing");
    check(split_char("a,,b,", ',') == vector<string>({"a", "", "b", ""}),
          "split keeps empty fields");
    check(split_char("", ',') == vector<string>({""}), "empty split");
    check(is_palindrome("abba", 0, 4), "even palindrome");
    check(is_palindrome("", 0, 0), "empty palindrome");
    check(!is_palindrome("ab", 0, 2), "non-palindrome");
    check(ascii_lower("AZ-09z") == "az-09z", "ASCII case conversion");
    check(run_length("aaabbc") == vector<pair<char, int>>(
          {{'a', 3}, {'b', 2}, {'c', 1}}), "run length groups");
    check(run_length("").empty(), "empty run length");
}
#endif

int main() {
    test_knapsack(); test_lis(); test_hamilton();
    test_integer_bits(); test_modular();
#ifndef P0_ONLY
    test_interval_merge(); test_matrix_recurrence();
    test_combinations(); test_strings();
#endif
    cout << "PASS dp/math/string fixed-seed differential tests\n";
}
