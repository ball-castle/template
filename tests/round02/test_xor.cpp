#include <bits/stdc++.h>
using namespace std;

// 先复现真正缺口：2268C 的原 solve() 没有输出。
#ifdef XOR_ORIGINAL_RED
namespace original_c {
#define main original_main
#include "originals/xor/1/1124/C.cpp"
#undef main
}
int main() {
    istringstream in("5\n1 7 3 7 2\n");
    ostringstream out;
    auto *oldin = cin.rdbuf(in.rdbuf());
    auto *oldout = cout.rdbuf(out.rdbuf());
    original_c::solve();
    cin.rdbuf(oldin); cout.rdbuf(oldout);
    if(out.str() != "6\n") {
        cerr << "EXPECTED FAIL: original 2268C solve is empty; expected 6\n";
        return 1;
    }
}
#else
// 规范模块各自独立可抄；仅测试隔离常见全局变量名。
namespace prefix_model {
#include "../../competition/xor/prefix_xor.hpp"
}
namespace contribution_model {
#include "../../competition/xor/bit_contribution.hpp"
}
namespace construct_model {
#include "../../competition/xor/xor_construct.hpp"
}
namespace trie_model {
#include "../../competition/xor/binary_trie.hpp"
}
namespace graph_model {
#include "../../competition/xor/graph_xor.hpp"
}
namespace basis_model {
#include "../../competition/xor/linear_basis.hpp"
}
namespace review_model {
#include "../../competition/xor/personal_models.hpp"
}
namespace energy_model {
#include "../../competition/xor/energy_intervals.hpp"
}
// 直接编入用户旧源，不复制第二份旧实现。只在测试内隔离main和全局名。
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#define main original_main
namespace old_meat {
#include "originals/xor/1/1124/B.cpp"
}
namespace old_pair {
#include "originals/xor/4/1090/E_xor_find_pattern.cpp"
}
namespace old_gun {
#include "originals/xor/date/5.15/3.cpp"
}
namespace old_perm {
#include "originals/xor/date/7/7.9/2_and.cpp"
}
namespace old_or {
#include "originals/xor/2/1129/B.cpp"
}
#undef main
#pragma GCC diagnostic pop

using ull = unsigned long long;
using u128 = __uint128_t;
mt19937_64 rng(20261024);
int checks = 0;
void check(bool ok, const string& name) {
    checks++;
    if(!ok) { cerr << "FAIL: " << name << '\n'; exit(1); }
}
int rnd(int l, int r) { return l + int(rng() % unsigned(r - l + 1)); }

void test_prefix() {
    using namespace prefix_model;
    check(xor_prefix({0}) == vector<ull>{0}, "empty prefix");
    check(count_subarray_xor({0, 0, 0, 0}, 0) == 6, "zero duplicates");
    check(count_subarray_xor({0, 7}, 7) == 1, "prefix zero counted");
    for(int rep = 0; rep < 700; rep++) {
        int n = rnd(0, 35);
        vector<ull> a(n + 1);
        for(int i = 1; i <= n; i++) a[i] = rng();
        if(rep & 1) for(int i = 1; i <= n; i++) a[i] &= 15;
        auto p = xor_prefix(a);
        ull k = (rep & 1) ? rng() % 16 : rng();
        long long want = 0;
        for(int l = 1; l <= n; l++) {
            ull value = 0;
            for(int r = l; r <= n; r++) {
                value ^= a[r];
                check(range_xor(p, l, r) == value, "prefix versus loop");
                want += value == k;
            }
        }
        check(count_subarray_xor(a, k) == want, "xor count brute");
        vector<array<ull, 3>> updates;
        auto direct = a;
        for(int j = 0; n && j < 20; j++) {
            int l = rnd(1, n), r = rnd(l, n);
            ull x = rng();
            updates.push_back({ull(l), ull(r), x});
            for(int i = l; i <= r; i++) direct[i] ^= x;
        }
        check(xor_offline_update(a, updates) == direct, "xor difference");
    }
}

void test_contribution() {
    using namespace contribution_model;
    for(int rep = 0; rep < 500; rep++) {
        int n = rnd(0, 25);
        vector<ull> a(n + 1);
        for(int i = 1; i <= n; i++) a[i] = rng();
        array<u128, 3> want{};
        u128 sub_sum = 0;
        ull sub_xor = 0;
        for(int i = 1; i <= n; i++) {
            for(int j = i + 1; j <= n; j++) {
                want[0] += a[i] ^ a[j];
                want[1] += a[i] | a[j];
                want[2] += a[i] & a[j];
            }
            ull value = 0;
            for(int j = i; j <= n; j++) {
                value ^= a[j]; sub_sum += value; sub_xor ^= value;
            }
        }
        check(pair_bit_sums(a) == want, "pair xor/or/and sums");
        check(subarray_xor_sum(a) == sub_sum, "subarray arithmetic sum");
        check(all_subarray_xor(a) == sub_xor, "subarray xor multiplicity");
    }
    auto high = pair_bit_sums({0, ~0ULL, ~0ULL, 0});
    check(high[1] == u128(~0ULL) * 3, "pair sum exceeds uint64");
    check(to_string_u128(high[1]) == "55340232221128654845", "print u128");
    check(to_string_u128(0) == "0", "print zero");
}

void test_construct() {
    using namespace construct_model;
    for(int n = 0; n <= 500; n++) {
        ull want = 0;
        for(int i = 1; i <= n; i++) want ^= ull(i);
        check(xor_1_to_n(ull(n)) == want, "xor 1..n");
    }
    for(int rep = 0; rep < 500; rep++) {
        int n = rnd(1, 100);
        ull target = rng() & ((1ULL << 60) - 1);
        vector<ull> a;
        check(xor_distinct(n, target, a), "distinct feasible target");
        check(int(a.size()) == n + 1, "distinct 1-indexed");
        ull value = 0;
        set<ull> seen;
        for(int i = 1; i <= n; i++) {
            value ^= a[i]; seen.insert(a[i]);
            check(a[i] < (1ULL << 62), "distinct declared range");
        }
        check(value == target && int(seen.size()) == n, "distinct xor target");
        check(xor_missing(a, target) == 0, "known total recovers unknown zero");
        ull last = a.back(); a.pop_back();
        check(xor_missing(a, target) == last, "recover removed last element");
    }
    vector<ull> a;
    check(!xor_distinct(2, 0, a), "distinct pair cannot xor zero");
    check(xor_distinct(1, 0, a) && a[1] == 0, "single zero allowed");
    check(interval_or(4, 6) == 7 && interval_or(8, 8) == 8,
          "highest different bit interval OR");
    check(interval_or(0, ~0ULL) == ~0ULL, "full width OR no shift by64");
    for(ull l = 0; l < 100; l++) for(ull r = l; r < 100; r++) {
        ull want = 0;
        for(ull x = l; x <= r; x++) want |= x;
        check(interval_or(l, r) == want, "interval OR brute");
    }
}

void test_trie() {
    using namespace trie_model;
    check(trie_init(64, 200000), "trie initialization");
    ull out = 123;
    check(!trie_query(0, out) && !trie_erase(0), "empty trie");
    multiset<ull> plain;
    for(int rep = 0; rep < 4000; rep++) {
        ull x = (rep & 1) ? rng() % 32 : rng();
        if(rnd(0, 3) < 2) {
            check(trie_insert(x), "insert within capacity"); plain.insert(x);
        } else {
            if(!plain.empty() && rnd(0, 1)) {
                x = *next(plain.begin(), rnd(0, int(plain.size()) - 1));
            }
            auto it = plain.find(x);
            check(trie_erase(x) == (it != plain.end()), "erase existence");
            if(it != plain.end()) plain.erase(it);
        }
        for(bool maximum : {false, true}) {
            bool ok = trie_query(x, out, maximum);
            check(ok == !plain.empty(), "query empty flag");
            if(ok) {
                ull want = maximum ? 0 : ~0ULL;
                for(ull y : plain) want = maximum ? max(want, x ^ y)
                                                   : min(want, x ^ y);
                check(out == want, "trie versus multiset scan");
            }
        }
        check(cnt[1] == (long long)plain.size(), "root multiplicity");
    }
    check(trie_init(1, 2) && trie_insert(0), "exact capacity");
    check(!trie_insert(1) && cnt[1] == 1, "capacity failure atomic");
    check(trie_insert(0) && trie_erase(0), "duplicate nodes reused");
    check(trie_query(1, out) && out == 1, "one duplicate remains");
    check(trie_erase(0) && !trie_query(0, out), "last duplicate deleted");
    check(!trie_insert(2), "width rejection");
    check(!trie_init(0, 10) && !trie_init(65, 10), "invalid width");
    check(!trie_init(64, 0), "root needs capacity");
    for(int rep = 0; rep < 300; rep++) {
        int n = rnd(0, 30);
        vector<ull> a(n + 1);
        for(int i = 1; i <= n; i++) a[i] = rng();
        ull pair_want = 0, sub_want = 0;
        for(int i = 1; i <= n; i++) {
            ull value = 0;
            for(int j = i; j <= n; j++) {
                value ^= a[j]; sub_want = max(sub_want, value);
                if(i != j) pair_want = max(pair_want, a[i] ^ a[j]);
            }
        }
        check(max_xor_pair(a) == pair_want, "maximum pair brute");
        check(max_xor_subarray(a) == sub_want, "maximum subarray brute");
    }
}

bool brute_graph(int n, const vector<array<int, 3>>& edges) {
    // 2-bit labels; enumerate every assignment, not a traversal oracle.
    for(int mask = 0; mask < (1 << (2 * n)); mask++) {
        bool ok = true;
        for(auto [u, v, w] : edges)
            ok &= (((mask >> (2 * (u - 1))) ^
                    (mask >> (2 * (v - 1)))) & 3) == w;
        if(ok) return true;
    }
    return false;
}
void test_graph() {
    using namespace graph_model;
    for(int rep = 0; rep < 200; rep++) {
        int n = rnd(1, 5);
        xor_graph_init(n);
        vector<array<int, 3>> edges;
        for(int j = rnd(0, 8); j; j--) {
            int u = rnd(1, n), v = rnd(1, n), w = rnd(0, 3);
            edges.push_back({u, v, w}); xor_add_edge(u, v, ull(w));
        }
        bool ok = xor_assign();
        check(ok == brute_graph(n, edges), "graph xor brute assignment");
        if(ok) for(auto [u, v, w] : edges) {
            check((value[u] ^ value[v]) == ull(w), "graph witness");
            ull t = 1ULL << 63;
            check(((value[u] ^ t) ^ (value[v] ^ t)) == ull(w),
                  "component freedom");
        }
    }
    xor_graph_init(3); xor_add_edge(1, 2, 1ULL << 63);
    check(xor_assign() && components == 2 && value[2] == (1ULL << 63),
          "disconnected and highest bit");
    xor_graph_init(1); xor_add_edge(1, 1, 1);
    check(!xor_assign(), "self loop contradiction");
    xor_graph_init(200000);
    for(int i = 2; i <= 200000; i++) xor_add_edge(i - 1, i, ull(i));
    check(xor_assign() && components == 1, "iterative deep chain");
}

void test_basis() {
    using namespace basis_model;
    for(int rep = 0; rep < 350; rep++) {
        basis_init();
        set<ull> reachable{0};
        int n = rnd(0, 12);
        for(int i = 0; i < n; i++) {
            ull x = (rep & 1) ? rng() % 32 : rng();
            bool independent = !reachable.count(x);
            check(basis_insert(x) == independent, "basis dependence");
            auto old = reachable;
            for(ull y : old) reachable.insert(x ^ y);
        }
        check(reachable.size() == (1ULL << rk), "basis rank");
        check(basis_max() == *reachable.rbegin(), "max subset exhaustive");
        ull seed = rng(), want = 0;
        for(ull x : reachable) want = max(want, seed ^ x);
        check(basis_max(seed) == want, "seeded subset maximum");
    }
    basis_init();
    check(!basis_insert(0), "basis zero dependent");
    check(basis_insert(1ULL << 63) && !basis_insert(1ULL << 63),
          "basis highest bit and duplicate");
}

vector<ull> run_old(const function<void()>& solve, const string& input) {
    istringstream in(input);
    ostringstream out;
    auto *oldin = cin.rdbuf(in.rdbuf()), *oldout = cout.rdbuf(out.rdbuf());
    cin.clear(); solve();
    cin.rdbuf(oldin); cout.rdbuf(oldout); cin.clear();
    istringstream result(out.str());
    vector<ull> ans;
    ull x;
    while(result >> x) ans.push_back(x);
    return ans;
}

void test_meat() {
    using namespace review_model;
    for(int n = 2; n <= 4; n++) {
        int total = 1 << (4 * n);
        vector<int> best(total, -1);
        for(int start = 0; start < total; start++) {
            if(best[start] != -1) continue;
            vector<int> states{start}; best[start] = -2;
            int want = 0;
            for(size_t head = 0; head < states.size(); head++) {
                int state = states[head], have = 0;
                for(int i = 0; i < n; i++)
                    have += ((state >> (4 * i)) & 15) % 3 == 0;
                want = max(want, have);
                for(int i = 0; i + 1 < n; i++) for(int k = 1; k <= 5; k++) {
                    int next = state ^ ((3 * k) << (4 * i)) ^
                               ((3 * k) << (4 * (i + 1)));
                    if(best[next] == -1) {
                        best[next] = -2; states.push_back(next);
                    }
                }
            }
            for(int state : states) best[state] = want;
        }
        for(int state = 0; state < total; state++) {
            vector<long long> a(n + 1);
            int old = 0;
            for(int i = 1; i <= n; i++) {
                a[i] = (state >> (4 * (i - 1))) & 15;
                old += old_meat::good(int(a[i]));
            }
            check(all_meat_count(a) == best[state] && old == best[state],
                  "2268B all operation-connected components n2..4");
        }
    }
    for(int rep = 0; rep < 80; rep++) {
        int n = rnd(2, 30);
        vector<long long> a(n + 1);
        ostringstream input;
        input << n << " 30\n";
        for(int i = 1; i <= n; i++) input << (a[i] = rnd(0, 15)) << ' ';
        input << '\n';
        long long ans = all_meat_count(a);
        vector<ull> want{ull(ans)};
        for(int j = 0; j < 30; j++) {
            int pos = rnd(1, n), x = rnd(0, 15);
            input << pos << ' ' << x << '\n';
            all_meat_update(a, ans, pos, x);
            check(ans == all_meat_count(a), "2268B update count");
            want.push_back(ull(ans));
        }
        check(run_old(old_meat::solve, input.str()) == want,
              "2268B original solve same update stream");
    }
}

ull brute_eliminate(vector<ull> a) {
    if(a.size() == 1) return a[0];
    ull ans = 0;
    for(size_t i = 0; i < a.size(); i++) {
        vector<ull> next;
        for(size_t j = 0; j < a.size(); j++) if(i != j)
            next.push_back(a[j] ^ a[i]);
        ans = max(ans, brute_eliminate(next));
    }
    return ans;
}
int brute_gun(vector<long long> a) {
    queue<pair<vector<long long>, int>> q;
    set<vector<long long>> seen;
    q.push({a, 0}); seen.insert(a);
    while(!q.empty()) {
        auto [v, dist] = q.front(); q.pop();
        if(!is_sorted(v.begin(), v.end())) return dist;
        for(size_t i = 1; i < v.size(); i++) {
            auto next = v;
            next[i - 1] ^= next[i]; next.erase(next.begin() + int(i));
            if(seen.insert(next).second) q.push({next, dist + 1});
        }
    }
    return -1;
}
void test_pair_and_gun() {
    for(int rep = 0; rep < 120; rep++) {
        int n = rnd(2, 7);
        vector<ull> a(n + 1), plain;
        ostringstream input; input << n << '\n';
        for(int i = 1; i <= n; i++) {
            a[i] = ull(rnd(0, 31)); plain.push_back(a[i]); input << a[i] << ' ';
        }
        ull want = brute_eliminate(plain);
        check(review_model::max_xor_pair_quadratic(a) == want &&
              trie_model::max_xor_pair(a) == want, "2218E actual operation oracle");
        check(run_old(old_pair::solve, input.str()) == vector<ull>{want},
              "2218E preserve original double loop");
    }
    for(int rep = 0; rep < 500; rep++) {
        int n = rnd(2, 8);
        vector<long long> a(n + 1);
        for(int i = 1; i <= n; i++) a[i] = rnd(1, 40);
        sort(a.begin() + 1, a.end());
        int want = brute_gun(vector<long long>(a.begin() + 1, a.end()));
        check(review_model::xor_gun(a) == want, "1457D merge BFS oracle");
        ostringstream input; input << n << '\n';
        for(int i = 1; i <= n; i++) input << a[i] << ' ';
        auto old = run_old([] { old_gun::original_main(); }, input.str());
        check(old == vector<ull>{ull(want)}, "1457D original same oracle");
    }
    vector<long long> long_a(62, 1);
    long_a[0] = 0;
    check(review_model::xor_gun(long_a) == 1, "1457D 61 equal positives");
    check(review_model::xor_gun({0, 1, 2, 3}) == -1, "1457D no solution");
    check(review_model::xor_gun({0, 1, 2, 4, 6, 20}) == 2, "1457D sample");
}

bool valid_perm(const vector<long long>& p, int n, bool nonzero) {
    if(int(p.size()) != n + 1) return false;
    vector<int> seen(n + 1);
    for(int i = 1; i <= n; i++) {
        if(p[i] < 1 || p[i] > n || p[i] == i || seen[p[i]]++) return false;
        if(((p[i] & i) != 0) != nonzero) return false;
    }
    return true;
}
void test_permutations() {
    for(int n = 1; n <= 9; n++) {
        vector<int> p(n); iota(p.begin(), p.end(), 1);
        bool zero_exists = false, nonzero_exists = false;
        do {
            bool zero = true, nonzero = true;
            for(int i = 1; i <= n; i++) {
                zero &= p[i - 1] != i && (p[i - 1] & i) == 0;
                nonzero &= p[i - 1] != i && (p[i - 1] & i) != 0;
            }
            zero_exists |= zero; nonzero_exists |= nonzero;
        } while(next_permutation(p.begin(), p.end()));
        vector<long long> a;
        check(review_model::and_zero_perm(n, a) == zero_exists,
              "909F zero necessity brute all permutations");
        check(review_model::and_nonzero_perm(n, a) == nonzero_exists,
              "909F nonzero necessity brute all permutations");
    }
    for(int n = 1; n <= 2000; n++) {
        vector<long long> p, q;
        bool ok = review_model::and_zero_perm(n, p);
        check(!ok || valid_perm(p, n, false), "909F zero constructed witness");
        ok = review_model::and_nonzero_perm(n, q);
        check(!ok || valid_perm(q, n, true), "909F nonzero constructed witness");
    }
    for(int n : {65535, 65536, 65537, 100000}) {
        vector<long long> p;
        if(review_model::and_zero_perm(n, p))
            check(valid_perm(p, n, false), "909F large complement blocks");
        if(review_model::and_nonzero_perm(n, p))
            check(valid_perm(p, n, true), "909F large highest-bit cycles");
    }
    // 直接运行原solve，逐项验证输出，不根据输出顺序重新写算法。
    for(int n = 1; n <= 100; n++) {
        istringstream in(to_string(n)); ostringstream out;
        auto *oldin = cin.rdbuf(in.rdbuf()), *oldout = cout.rdbuf(out.rdbuf());
        cin.clear(); old_perm::solve();
        cin.rdbuf(oldin); cout.rdbuf(oldout); cin.clear();
        istringstream result(out.str());
        for(bool nonzero : {false, true}) {
            string flag; result >> flag;
            bool expected = nonzero ? (n >= 6 && (n & (n - 1))) : n % 2 == 0;
            check((flag == "YES") == expected, "909F original feasibility");
            if(flag == "YES") {
                vector<long long> p(n + 1);
                for(int i = 1; i <= n; i++) result >> p[i];
                check(valid_perm(p, n, nonzero), "909F original witness");
            }
        }
    }
}

ull brute_restaurant(const vector<int>& a) {
    vector<vector<int>> states{a}; set<vector<int>> seen{a};
    ull ans = 0;
    for(size_t h = 0; h < states.size(); h++) {
        const auto v = states[h];
        ull value = 0; for(int x : v) value |= ull(x);
        ans = max(ans, value);
        for(size_t i = 1; i < v.size(); i++) {
            if(v[i - 1] == v[i]) continue;
            auto next = v;
            next[v[i - 1] < v[i] ? i - 1 : i]++;
            if(seen.insert(next).second) states.push_back(next);
        }
    }
    return ans;
}
void test_restaurant() {
    for(int n = 1; n <= 4; n++) {
        int total = 1; for(int i = 0; i < n; i++) total *= 5;
        for(int mask = 0; mask < total; mask++) {
            vector<int> a(n); int x = mask;
            ostringstream input; input << n << '\n';
            for(int& value : a) { value = x % 5; x /= 5; input << value << ' '; }
            ull want = brute_restaurant(a);
            auto [mn, mx] = minmax_element(a.begin(), a.end());
            check(construct_model::interval_or(ull(*mn), ull(*mx)) == want,
                  "2261B actual adjacent-increment BFS");
            check(run_old(old_or::solve, input.str()) == vector<ull>{want},
                  "2261B original matches operation oracle");
        }
    }
}

int brute_energy(const vector<int>& a) {
    int ans = 0;
    for(int l = 1; l < int(a.size()); l++) {
        int mx = a[l];
        for(int r = l + 1; r < int(a.size()); r++) {
            mx = max(mx, a[r]);
            int value = 0;
            // 按题意先AND每个元素再XOR；不共用前缀化简。
            for(int i = l; i <= r; i++) value ^= a[i] & mx;
            ans = max(ans, value);
        }
    }
    return ans;
}
void test_energy() {
    using namespace energy_model;
    check(energy_max({0, 1, 7, 3, 7, 2}) == 6, "2268C formerly empty solve");
    check(energy_max({0, 3, 1, 2}) == 2, "2268C singleton exclusion");
    check(energy_max({0, 1, 5, 2, 3, 4}) == 5, "2268C sample three");
    check(energy_max({0, 3, 5, 2, 6}) == 5, "2268C sample four");
    check(energy_max({0, 7, 7}) == 0, "2268C forbidden singleton pair");
    for(int n = 2; n <= 7; n++) {
        for(int mask = 0; mask < (1 << (2 * n)); mask++) {
            vector<int> input(n + 1);
            for(int i = 1; i <= n; i++) input[i] = (mask >> (2 * (i - 1))) & 3;
            check(energy_max(input, 2) == brute_energy(input),
                  "2268C exhaustive arrays value0..3 length2..7");
        }
    }
    for(int rep = 0; rep < 700; rep++) {
        int width = rep < 650 ? 6 : 18;
        vector<int> input(rnd(2, 60) + 1);
        for(size_t i = 1; i < input.size(); i++) input[i] = rnd(0, (1 << width) - 1);
        check(energy_max(input, width) == brute_energy(input), "2268C random brute");
    }
    vector<int> monotone(200001);
    for(int i = 1; i <= 200000; i++) monotone[i] = i - 1;
    int forward = energy_max(monotone);
    reverse(monotone.begin() + 1, monotone.end());
    check(energy_max(monotone) == forward, "2268C 200000 chain reverse symmetry");
    fill(monotone.begin() + 1, monotone.end(), (1 << 18) - 1);
    check(energy_max(monotone) == (1 << 18) - 1, "2268C 200000 equal maxima");
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
    test_prefix(); test_contribution(); test_construct();
    test_trie(); test_graph(); test_basis();
    test_meat(); test_pair_and_gun(); test_permutations();
    test_restaurant(); test_energy();
    cout << "PASS xor seed=20261024 checks=" << checks << '\n';
}
#endif
