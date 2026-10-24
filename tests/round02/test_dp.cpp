#include <bits/stdc++.h>
#include "../../dp/knapsack.hpp"
#include "../../dp/lis.hpp"
#include "../../dp/interval_merge.hpp"
#include "../../dp/hamilton_path.hpp"
using namespace std;
using ll = long long;

#include "../../competition/dp/linear_grid.hpp"
#include "../../competition/dp/knapsack_variants.hpp"
#include "../../competition/dp/sequence_state.hpp"
namespace digit_source {
#include "../../competition/dp/digit_count.hpp"
}
using digit_source::digit_calc;
using digit_source::digit_range;
using digit_source::binary_no_adjacent;
#include "../../competition/dp/tree_independent_set.hpp"
#include "../../competition/dp/probability_expectation.hpp"

// 精确Git基线快照只用于审计对照；无需依赖父cf仓库。
// 来源与LF SHA见originals/dp/sources.json，不是第二个正式实现。
namespace original_digit {
// 原稿递归lambda的形参也叫dfs；保留源码，只在该include抑制同名告警。
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#define main original_digit_main
#include "originals/dp/digit.cpp"
#undef main
#pragma GCC diagnostic pop
}
namespace original_direction {
#define main original_direction_main
#include "originals/dp/direction.cpp"
#undef main
}
namespace original_chain {
#define main original_chain_main
#include "originals/dp/jakarta.cpp"
#undef main
}

mt19937_64 rng(20261024);
int checks = 0, failures = 0;
int rnd(int l, int r) { return l + int(rng() % (r - l + 1)); }
void check(bool ok, const string& name) {
    ++checks;
    if (!ok) {
        if (failures < 40) cerr << "FAIL: " << name << '\n';
        ++failures;
    }
}
void close_to(long double got, long double want, const string& name) {
    check(fabsl(got - want) <= 1e-12L * max(1.0L, fabsl(want)), name);
}

ll brute_subarray(const vector<ll>& a) {
    ll ans = LLONG_MIN;
    for (int l = 1; l < int(a.size()); ++l) {
        ll sum = 0;
        for (int r = l; r < int(a.size()); ++r) {
            sum += a[r];
            ans = max(ans, sum);
        }
    }
    return ans;
}
pair<ll, ll> brute_grid(const vector<vector<ll>>& a,
                       const vector<string>& g) {
    int n = int(g.size()) - 1, m = int(g[1].size()) - 1;
    ll ways = 0, best = GRID_INF;
    auto dfs = [&](auto&& self, int x, int y, ll sum) -> void {
        if (x > n || y > m || g[x][y] == '#') return;
        sum += a[x][y];
        if (x == n && y == m) { ++ways; best = min(best, sum); return; }
        self(self, x + 1, y, sum);
        self(self, x, y + 1, sum);
    };
    dfs(dfs, 1, 1, 0);
    return {ways, best};
}
void test_linear_grid() {
    check(max_subarray({0, -8, -2, -5}) == -2, "nonempty all-negative");
    check(max_subarray({0, 4000000000LL, -1, 4000000000LL}) ==
          7999999999LL, "64-bit linear");
    check(max_subarray({0, -(1LL << 59)}) == -(1LL << 59),
          "large negative linear");
    for (int rep = 0; rep < 500; ++rep) {
        vector<ll> a(rnd(1, 10) + 1);
        for (int i = 1; i < int(a.size()); ++i) a[i] = rnd(-10, 10);
        check(max_subarray(a) == brute_subarray(a), "linear interval oracle");
    }
    for (int rep = 0; rep < 350; ++rep) {
        int n = rnd(1, 5), m = rnd(1, 5), mod = rnd(1, 17);
        vector<string> g(n + 1, string(m + 1, '.'));
        vector<vector<ll>> a(n + 1, vector<ll>(m + 1));
        for (int i = 1; i <= n; ++i) for (int j = 1; j <= m; ++j) {
            if (rnd(0, 4) == 0) g[i][j] = '#';
            a[i][j] = rnd(-10, 10);
        }
        auto [ways, best] = brute_grid(a, g);
        check(grid_paths(g, mod) == ways % mod, "grid enumerated paths");
        check(grid_min_path(a, g) == best, "grid enumerated min cost");
    }
    check(grid_paths({"", " #"}, 7) == 0, "blocked singleton");
    check(grid_paths({"", " ."}, 1) == 0, "grid mod one");
    check(grid_min_path({{}, {0, -(1LL << 59)}}, {"", " ."}) ==
          -(1LL << 59), "wide singleton grid");
}
vector<ll> brute_bounded(const vector<int>& w, const vector<ll>& v,
                        const vector<int>& cnt, int cap, bool exact) {
    vector<ll> ans(cap + 1, KNAP_NEG);
    auto dfs = [&](auto&& self, int i, int used, ll value) -> void {
        if (i == int(w.size())) { ans[used] = max(ans[used], value); return; }
        for (int k = 0; k <= min(cnt[i], (cap - used) / w[i]); ++k)
            self(self, i + 1, used + k * w[i], value + k * v[i]);
    };
    dfs(dfs, 1, 0, 0);
    if (!exact) for (int j = 1; j <= cap; ++j) ans[j] = max(ans[j], ans[j-1]);
    return ans;
}
vector<ll> brute_groups(const vector<vector<pair<int, ll>>>& groups,
                        int cap, bool exact) {
    vector<ll> ans(cap + 1, KNAP_NEG);
    auto dfs = [&](auto&& self, int i, int used, ll value) -> void {
        if (i == int(groups.size())) {
            ans[used] = max(ans[used], value);
            return;
        }
        self(self, i + 1, used, value);
        for (auto [w, v] : groups[i])
            if (used + w <= cap) self(self, i + 1, used + w, value + v);
    };
    dfs(dfs, 1, 0, 0);
    if (!exact) for (int j = 1; j <= cap; ++j) ans[j] = max(ans[j], ans[j-1]);
    return ans;
}
void test_bags() {
    for (int rep = 0; rep < 400; ++rep) {
        int n = rnd(0, 4), cap = rnd(0, 12);
        vector<int> w(n + 1), cnt(n + 1), once(n + 1, 1), many(n + 1);
        vector<ll> v(n + 1);
        vector<vector<pair<int, ll>>> groups(n + 1), singleton(n + 1);
        for (int i = 1; i <= n; ++i) {
            w[i] = rnd(1, 6); cnt[i] = rnd(0, 4); v[i] = rnd(-8, 15);
            many[i] = cap / w[i]; singleton[i] = {{w[i], v[i]}};
            for (int j = rnd(0, 3); j > 0; --j)
                groups[i].push_back({rnd(1, 6), rnd(-8, 15)});
        }
        for (bool exact : {false, true}) {
            check(knapsack_bounded(w, v, cnt, cap, exact) ==
                  brute_bounded(w, v, cnt, cap, exact), "bounded quantities");
            check(knapsack_grouped(groups, cap, exact) ==
                  brute_groups(groups, cap, exact), "group tuple choices");
            auto one = brute_bounded(w, v, once, cap, exact);
            check(knapsack01(w, v, cap, exact) == one, "old 01 same oracle");
            check(knapsack_bounded(w, v, once, cap, exact) == one,
                  "new bounded count one same oracle");
            check(knapsack_grouped(singleton, cap, exact) == one,
                  "new singleton group same oracle");
            auto unlimited = brute_bounded(w, v, many, cap, exact);
            check(knapsack_complete(w, v, cap, exact) == unlimited,
                  "old complete same oracle");
            check(knapsack_bounded(w, v, many, cap, exact) == unlimited,
                  "new sufficient bounded same oracle");
        }
    }
    check(knapsack_bounded({0, 1}, {0, 2}, {0, INT_MAX}, 3, false) ==
          vector<ll>({0, 2, 4, 6}), "large count clipped to capacity");
    check(knapsack_grouped({{}, {{1, 4}, {1, 5}}}, 2, false) ==
          vector<ll>({0, 5, 5}), "cannot take two from one group");
    check(knapsack_bounded({0, 1}, {0, -(1LL << 58)}, {0, 2}, 2, true)
          == vector<ll>({0, -(1LL << 58), -(1LL << 59)}),
          "negative exact wide values");
}
vector<string> subsequences(const string& s, bool empty = true) {
    set<string> all;
    for (int mask = empty ? 0 : 1; mask < (1 << int(s.size())); ++mask) {
        string t;
        for (int i = 0; i < int(s.size()); ++i) if (mask >> i & 1) t += s[i];
        all.insert(t);
    }
    return {all.begin(), all.end()};
}
int brute_lcs(const string& s, const string& t) {
    auto a = subsequences(s), b = subsequences(t);
    set<string> bs(b.begin(), b.end());
    int best = 0;
    for (const auto& x : a) if (bs.count(x)) best = max(best, int(x.size()));
    return best;
}
// BFS over actual strings and edits, independent of prefix DP.
int brute_edit(const string& s, const string& target) {
    queue<string> q;
    map<string, int> dis;
    q.push(s); dis[s] = 0;
    int limit = int(max(s.size(), target.size()));
    while (!q.empty()) {
        string cur = q.front(); q.pop();
        int d = dis[cur];
        if (cur == target) return d;
        vector<string> next;
        for (int i = 0; i < int(cur.size()); ++i) {
            string a = cur; a.erase(i, 1); next.push_back(a);
            for (char c : {'a', 'b'}) {
                a = cur; a[i] = c; next.push_back(a);
            }
        }
        if (int(cur.size()) < limit)
            for (int i = 0; i <= int(cur.size()); ++i)
                for (char c : {'a', 'b'}) {
                    string a = cur; a.insert(a.begin() + i, c); next.push_back(a);
                }
        for (const auto& a : next) if (!dis.count(a)) {
            dis[a] = d + 1; q.push(a);
        }
    }
    return -1;
}
bool brute_direction(const string& s, const string& t) {
    if (t.empty()) return true;
    for (int start = 0; start < int(s.size()); ++start)
        for (int right = 0; right < int(t.size()); ++right) {
            string cur;
            int p = start;
            for (int step = 0; step < int(t.size()); ++step) {
                if (p < 0 || p >= int(s.size())) break;
                cur += s[p];
                p += step < right ? 1 : -1;
            }
            if (cur == t) return true;
        }
    return false;
}
ll brute_chain(const vector<string>& a) {
    ll ans = -1;
    auto dfs = [&](auto&& self, int i, const string& last, ll total) -> void {
        if (i == int(a.size())) { ans = max(ans, total); return; }
        for (const auto& cur : subsequences(a[i], false))
            if (i == 1 || last < cur) self(self, i + 1, cur, total + cur.size());
    };
    dfs(dfs, 1, "", 0);
    return ans;
}
string random_word(int n) {
    string s;
    while (n--) s += char('a' + rnd(0, 1));
    return s;
}
void test_sequence_state() {
    check(lcs_length("abc", "ac") == 2, "LCS example");
    check(edit_distance("kitten", "sitting") == 3, "edit example");
    check(right_then_left_match("abc", "cba"), "turn before moving right");
    check(!right_then_left_match("abc", "abca"), "cannot turn twice");
    check(lex_subsequence_chain({"", "ab", "bc"}) == 4, "lex chain sample");
    check(lex_subsequence_chain({"", "b", "a"}) == -1, "lex chain unreachable");
    check(lex_subsequence_chain({""}) == 0, "empty chain");
    for (int rep = 0; rep < 350; ++rep) {
        string a = random_word(rnd(0, 7)), b = random_word(rnd(0, 7));
        check(lcs_length(a, b) == brute_lcs(a, b), "LCS subset oracle");
        check(right_then_left_match(a, b) == brute_direction(a, b),
              "direction enumerate starts and turns");
    }
    for (int rep = 0; rep < 100; ++rep) {
        string a = random_word(rnd(0, 4)), b = random_word(rnd(0, 4));
        check(edit_distance(a, b) == brute_edit(a, b), "edit BFS oracle");
    }
    for (int rep = 0; rep < 160; ++rep) {
        vector<string> a(rnd(0, 3) + 1);
        for (int i = 1; i < int(a.size()); ++i) a[i] = random_word(rnd(1, 4));
        check(lex_subsequence_chain(a) == brute_chain(a), "lex chain cartesian oracle");
    }
}
int nonzero_digits(ll x) {
    int ans = 0;
    while (x) { ans += x % 10 != 0; x /= 10; }
    return ans;
}
// Enumerate positions and nonzero digits, not upper-bound digit recursion.
ll sparse_integer_oracle(ll bound) {
    ll answer = 1;
    auto dfs = [&](auto&& self, int pos, int used, __int128 value) -> void {
        if (used == 3) return;
        __int128 base = 1;
        for (int i = 0; i < 19; ++i) {
            if (i >= pos) for (int d = 1; d <= 9; ++d) {
                __int128 next = value + base * d;
                if (next <= bound) { ++answer; self(self, i + 1, used + 1, next); }
            }
            base *= 10;
        }
    };
    if (bound < 0) return 0;
    dfs(dfs, 0, 0, 0);
    return answer;
}
void test_digit_count() {
    check(binary_no_adjacent(INT_MAX, 1) == 0, "binary INT_MAX counter boundary");
    check(digit_calc(-1) == 0, "negative digit prefix");
    check(digit_range(0, 0) == 1, "zero counted");
    ll count = 0;
    for (int x = 0; x <= 20000; ++x) {
        count += nonzero_digits(x) <= 3;
        if (x % 17 == 0)
            check(digit_calc(x) == count, "digit prefix brute enumeration");
    }
    check(digit_calc(LLONG_MAX) == sparse_integer_oracle(LLONG_MAX),
          "digit LLONG_MAX sparse oracle");
    check(digit_range(1000, 1111) == 111, "digit interval excludes 1111");
    for (int rep = 0; rep < 150; ++rep) {
        int l = rnd(0, 10000), r = l + rnd(0, 500);
        ll want = 0;
        for (int x = l; x <= r; ++x) want += nonzero_digits(x) <= 3;
        check(digit_range(l, r) == want, "digit multi-query reset");
    }
    for (int n = 0; n <= 16; ++n) {
        int ways = 0;
        for (int mask = 0; mask < (1 << n); ++mask)
            ways += (mask & (mask << 1)) == 0;
        for (int mod : {1, 2, 17, 1000000007, INT_MAX})
            check(binary_no_adjacent(n, mod) == ways % mod, "binary mask oracle");
    }
}
ll brute_tree(const vector<vector<int>>& g, const vector<ll>& a) {
    int n = int(g.size()) - 1;
    ll ans = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
        bool ok = true;
        ll value = 0;
        for (int u = 1; u <= n; ++u) if (mask >> (u - 1) & 1) {
            value += a[u];
            for (int v : g[u]) if (mask >> (v - 1) & 1) ok = false;
        }
        if (ok) ans = max(ans, value);
    }
    return ans;
}
void test_tree() {
    for (int rep = 0; rep < 350; ++rep) {
        int n = rnd(1, 11);
        vector<vector<int>> g(n + 1);
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) {
            a[i] = rnd(-15, 20);
            if (i > 1) {
                int p = rnd(1, i - 1);
                g[p].push_back(i); g[i].push_back(p);
            }
        }
        check(tree_independent_set(g, a, rnd(1, n)) == brute_tree(g, a),
              "tree subset oracle arbitrary root");
    }
    check(tree_independent_set({{}, {}}, {0, -(1LL << 59)}) == 0,
          "tree empty selection all negative");
    check(tree_independent_set({{}, {2}, {1}}, {0, 1LL << 59, 2}) ==
          1LL << 59, "wide tree weight");
    int n = 200000;
    vector<vector<int>> g(n + 1);
    vector<ll> a(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        g[i].push_back(i - 1); g[i - 1].push_back(i);
    }
    check(tree_independent_set(g, a) == n / 2, "deep tree avoids recursion stack");
}
void test_probability() {
    for (int rep = 0; rep < 250; ++rep) {
        int n = rnd(0, 10);
        vector<long double> p(n + 1), want(n + 1);
        for (int i = 1; i <= n; ++i) p[i] = rnd(0, 4) / 4.0L;
        for (int mask = 0; mask < (1 << n); ++mask) {
            long double probability = 1;
            for (int i = 1; i <= n; ++i)
                probability *= mask >> (i - 1) & 1 ? p[i] : 1 - p[i];
            want[popcount(unsigned(mask))] += probability;
        }
        auto got = coin_heads_probability(p);
        check(got.size() == want.size(), "probability output size");
        if (got.size() == want.size()) {
            for (int j = 0; j <= n; ++j)
                close_to(got[j], want[j], "coin outcome enumeration");
            close_to(accumulate(got.begin(), got.end(), 0.0L), 1, "probability sum one");
        }
        long double expected = 0;
        for (int i = 1; i <= n; ++i) {
            p[i] = rnd(1, 4) / 4.0L;
            long double tail = 1;
            for (int k = 0; k < 256; ++k) {
                expected += tail;
                tail *= 1 - p[i];
            }
        }
        close_to(expected_stage_attempts(p), expected, "expectation survival-sum oracle");
    }
    close_to(expected_stage_attempts({0, 1e-12L}), 1e12L, "small success probability");
    close_to(expected_stage_attempts({0, 1, 1}), 2, "deterministic stages");
}
void tested_card_calls() {
    check(knapsack01({0, 2, 3}, {0, 3, 5}, 5, true)[5] == 8, "card 01");
    check(knapsack_complete({0, 2}, {0, 3}, 4)[4] == 6, "card complete");
    check(interval_merge({0, 1, 2, 3}) == 9, "card interval");
    check(lis_length({0, 3, 1, 2, 2}) == 2, "card LIS strict");
    check(lis_length({0, 3, 1, 2, 2}, false) == 3, "card LIS nonstrict");
    vector<vector<ll>> w(4, vector<ll>(4, HAMILTON_INF));
    w[1][2] = 4; w[2][3] = -1; w[1][3] = 1;
    check(hamilton_path(w) == 3, "card Hamilton");
}
template<class F>
string run_original(F solve, const string& input) {
    istringstream in(input);
    ostringstream out;
    auto old_in = cin.rdbuf(in.rdbuf());
    auto old_out = cout.rdbuf(out.rdbuf());
    cin.clear();
    solve();
    cin.rdbuf(old_in); cout.rdbuf(old_out);
    cin.clear();
    return out.str();
}
void test_personal_originals() {
    for (int rep = 0; rep < 120; ++rep) {
        int l = rnd(0, 10000), r = l + rnd(0, 500);
        ll want = 0;
        for (int x = l; x <= r; ++x) want += nonzero_digits(x) <= 3;
        string input = to_string(l) + " " + to_string(r) + "\n";
        check(run_original(original_digit::solve, input) ==
              to_string(want) + "\n", "personal digit same oracle");
        check(digit_range(l, r) == want, "extracted digit same oracle");
        string a = random_word(rnd(1, 7)), b = random_word(rnd(1, 7));
        bool ok = brute_direction(a, b);
        check(run_original(original_direction::solve, a + " " + b + "\n")
              == (ok ? "YES\n" : "NO\n"), "personal direction same oracle");
        check(right_then_left_match(a, b) == ok, "extracted direction same oracle");
        vector<string> names(rnd(1, 3) + 1);
        input = to_string(names.size() - 1) + "\n";
        for (int i = 1; i < int(names.size()); ++i) {
            names[i] = random_word(rnd(1, 4));
            input += names[i] + "\n";
        }
        ll best = brute_chain(names);
        check(run_original(original_chain::solve, input) ==
              to_string(best) + "\n", "personal Jakarta same oracle");
        check(lex_subsequence_chain(names) == best, "extracted Jakarta same oracle");
    }
}
int main() {
    test_linear_grid(); test_bags(); test_sequence_state(); test_digit_count();
    test_tree(); test_probability(); tested_card_calls();
    test_personal_originals();
    if (failures) {
        cerr << failures << " / " << checks << " checks failed\n";
        return 1;
    }
    cout << "PASS round02 DP: " << checks << " checks, seed 20261024\n";
    cout << "PASS original digit/direction/Jakarta: 120 shared-oracle cases each\n";
}

