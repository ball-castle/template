#include <bits/stdc++.h>
using namespace std;
#include "../../math/integer_bits.hpp"
#if __has_include("../../competition/stl/recipes.hpp") && \
    __has_include("../../competition/stl/heaps_sets.hpp")
#include "../../competition/stl/recipes.hpp"
#include "../../competition/stl/heaps_sets.hpp"
#else
int main() { cerr << "MISSING: tested STL recipes and heap/set models\n"; return 1; }
#endif

#if __has_include("../../competition/stl/recipes.hpp") && \
    __has_include("../../competition/stl/heaps_sets.hpp")
long long brute_merge(vector<long long> a) {
    if (a.size() <= 1) return 0;
    long long best = LLONG_MAX;
    for (int i = 0; i < int(a.size()); ++i)
        for (int j = i + 1; j < int(a.size()); ++j) {
            vector<long long> next;
            for (int k = 0; k < int(a.size()); ++k)
                if (k != i && k != j) next.push_back(a[k]);
            next.push_back(a[i] + a[j]);
            best = min(best, a[i] + a[j] + brute_merge(next));
        }
    return best;
}

void test_heap_models() {
    assert(huffman_cost({1, 2, 3, 4}) == 19);
    assert((top_k_largest({3, 1, 9, 9, -2}, 3) == vector<long long>{9, 9, 3}));
    assert(top_k_largest({}, 0).empty());
    assert((top_k_smallest({3, 1, 9, 9, -2}, 3) == vector<long long>{-2, 1, 3}));
    assert(top_k_smallest({}, 0).empty());
    mt19937 rng(20261024);
    for (int run = 0; run < 100; ++run) {
        int n = int(rng() % 6);
        vector<long long> a(n);
        for (auto& x : a) x = rng() % 10;
        assert(huffman_cost(a) == brute_merge(a));
    }
    for (int run = 0; run < 300; ++run) {
        vector<long long> a;
        median_init();
        for (int i = 0; i < 100; ++i) {
            long long x = int(rng() % 101) - 50;
            a.push_back(x); median_add(x);
            auto b = a; sort(b.begin(), b.end());
            assert(median_lower() == b[(b.size() - 1) / 2]);
            int k = int(rng() % 110);
            sort(b.rbegin(), b.rend());
            b.resize(min(b.size(), size_t(k)));
            assert(top_k_largest(a, k) == b);
            b = a; sort(b.begin(), b.end());
            b.resize(min(b.size(), size_t(k)));
            assert(top_k_smallest(a, k) == b);
        }
    }
    lazy_init(30);
    map<int, long long> oracle_active;
    for (int step = 0; step < 20000; ++step) {
        int id = int(rng() % 30) + 1;
        if (rng() % 3) {
            long long value = int(rng() % 101) - 50;
            lazy_set(id, value); oracle_active[id] = value;
        } else {
            lazy_erase(id); oracle_active.erase(id);
        }
        optional<pair<long long, int>> want;
        for (auto [i, value] : oracle_active)
            if (!want || make_pair(value, i) > *want) want = {value, i};
        assert(lazy_top() == want);
    }
    lazy_init(2); assert(!lazy_top());
    lazy_set(1, 7); lazy_set(1, 3); assert(lazy_top()->first == 3);
    lazy_erase(1); assert(!lazy_top());
}

void test_set_models() {
    set<long long> values{1, 4, 7};
    assert(strict_neighbors(values, 4) ==
           make_pair(optional<long long>(1), optional<long long>(7)));
    assert(!strict_neighbors(values, 1).first);
    assert(!strict_neighbors(values, 7).second);
    multiset<long long> slots{2, 2, 5};
    assert(assign_at_least(slots, 2) == 2 && slots.size() == 2);
    assert(assign_at_least(slots, 3) == 5);
    assert(!assign_at_least(slots, 3));
    assert(slots == multiset<long long>{2});
    mt19937 rng(20261024);
    for (int run = 0; run < 2000; ++run) {
        int n = int(rng() % 40) + 1, k = int(rng() % n) + 1;
        vector<long long> a(n), spread, medians;
        for (auto& x : a) x = int(rng() % 31) - 15;
        for (int l = 0; l + k <= n; ++l) {
            vector<long long> b(a.begin() + l, a.begin() + l + k);
            sort(b.begin(), b.end());
            spread.push_back(b.back() - b.front());
            medians.push_back(b[(k - 1) / 2]);
        }
        assert(window_spread(a, k) == spread);
        assert(window_lower_medians(a, k) == medians);
    }
    assert(window_spread({}, 0).empty());
    assert(window_lower_medians({}, 0).empty());
}

void test_integer_reference_recipes() {
    assert(gcd_abs(-6, 4) == 2);
    assert(lcm_checked(6, 4) == 12);
    assert(lowbit(12ULL) == 4 && bit_count(12ULL) == 2);
    assert(lowest_bit_index(0) == -1 && lowest_bit_index(12) == 2);
    assert(countr_zero(0ULL) == 64 && countl_zero(0ULL) == 64);
    for (unsigned long long mask = 0; mask < 256; ++mask) {
        if (mask) { // GNU builtins ctz/clz的0输入未定义，必须先判断。
            assert(__builtin_ctzll(mask) == countr_zero(mask));
            assert(__builtin_clzll(mask) == countl_zero(mask));
        }
        unsigned long long full = (1ULL << 8) - 1;
        auto complement = full ^ mask; // 仅在8位全集内取补集。
        assert((mask & complement) == 0 && (mask | complement) == full);
        int ways = 0;
        for (auto sub = mask; ; sub = (sub - 1) & mask) {
            assert((sub & mask) == sub);
            ++ways;
            if (sub == 0) break;
        }
        assert(ways == (1 << popcount(mask)));
    }
    assert(-7 / 3 == -2 && -7 % 3 == -1); // C++向0截断。
    assert(((-7 % 3) + 3) % 3 == 2); // 正模数归一化。
}

int main() {
    stl_container_recipes();
    stl_algorithm_recipes();
    stl_string_recipes();
    test_heap_models(); test_set_models(); test_integer_reference_recipes();
    cout << "STL: all recipes + Huffman exhaustive + heap/set differential PASS\n";
}
#endif
