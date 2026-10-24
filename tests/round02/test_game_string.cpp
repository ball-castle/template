#include <bits/stdc++.h>
using namespace std;
#if __has_include("../../competition/game/mex.hpp") && \
    __has_include("../../competition/game/normal_play.hpp")
#include "../../competition/game/mex.hpp"
#include "../../competition/game/normal_play.hpp"
#if __has_include("../../competition/string/trie.hpp") && \
    __has_include("../../competition/string/palindrome.hpp")
namespace personal_trie {
#include "../../competition/string/trie.hpp"
}
#include "../../competition/string/palindrome.hpp"
#endif
#else
int main() { cerr << "MISSING: MEX and normal-play modules\n"; return 1; }
#endif

#if __has_include("../../competition/game/mex.hpp") && \
    __has_include("../../competition/game/normal_play.hpp")
int brute_mex(const vector<int>& a) {
    for (int x = 0; ; ++x)
        if (find(a.begin(), a.end(), x) == a.end()) return x;
}

void test_mex() {
    assert(static_mex({}) == 0);
    assert(static_mex({-1, 0, 0, 2, INT_MAX}) == 1);
    assert((prefix_mex({1, 0, 3, 2}) == vector<int>{0, 2, 2, 4}));
    mt19937 rng(20261024);
    for (int run = 0; run < 300; ++run) {
        vector<int> a;
        mex_init(80); // At most 80 active values, including ignored values.
        for (int step = 0; step < 400; ++step) {
            if (a.empty() || (a.size() < 80 && rng() % 2)) {
                int x = int(rng() % 110) - 10;
                a.push_back(x); mex_add(x);
            } else {
                size_t pos = rng() % a.size();
                mex_erase(a[pos]);
                a.erase(a.begin() + ptrdiff_t(pos));
            }
            assert(mex_query() == brute_mex(a));
            assert(static_mex(a) == brute_mex(a));
            vector<int> prefix;
            auto got = prefix_mex(a);
            for (size_t i = 0; i < a.size(); ++i) {
                prefix.push_back(a[i]);
                assert(got[i] == brute_mex(prefix));
            }
        }
    }
    mex_init(2); mex_add(0); mex_add(0);
    assert(mex_query() == 1);
    mex_erase(0); assert(mex_query() == 1);
    mex_erase(0); assert(mex_query() == 0);
    for (int n = 0; n <= 15; ++n)
        for (int k = 0; k <= n; ++k) {
            vector<int> constructed;
            for (int x = 0; x < k; ++x) constructed.push_back(x);
            while (int(constructed.size()) < n) constructed.push_back(k + 1);
            assert(static_mex(constructed) == k);
        }
}

bool brute_nim(vector<unsigned long long>& a, bool misere,
               map<vector<unsigned long long>, bool>& memo) {
    auto hit = memo.find(a);
    if (hit != memo.end()) return hit->second;
    bool terminal = true;
    for (auto x : a) terminal &= (x == 0);
    if (terminal) return memo[a] = misere;
    auto initial = a;
    for (size_t i = 0; i < a.size(); ++i) {
        auto old = a[i];
        for (unsigned long long x = 0; x < old; ++x) {
            a[i] = x;
            bool next_wins = brute_nim(a, misere, memo);
            a[i] = old;
            if (!next_wins) return memo[initial] = true;
        }
    }
    return memo[initial] = false;
}

void test_game() {
    map<vector<unsigned long long>, bool> normal, misere;
    for (unsigned long long x = 0; x <= 5; ++x)
        for (unsigned long long y = 0; y <= 5; ++y)
            for (unsigned long long z = 0; z <= 5; ++z) {
                vector<unsigned long long> a{x, y, z};
                bool win = brute_nim(a, false, normal);
                assert((nim_xor(a) != 0) == win);
                auto [index, after] = nim_winning_move(a);
                if (!win) assert(index == -1);
                else {
                    assert(0 <= index && index < int(a.size()) && after < a[index]);
                    a[index] = after;
                    assert(nim_xor(a) == 0);
                    a = {x, y, z};
                }
                assert(misere_nim_win(a) == brute_nim(a, true, misere));
            }
    // State numbers are a topological order: every move v -> u has u < v.
    vector<vector<int>> moves(51);
    for (int v = 1; v <= 50; ++v)
        for (int take : {1, 3, 4}) if (v >= take) moves[v].push_back(v - take);
    auto sg = sg_ordered_dag(moves);
    vector<bool> win(51, false);
    for (int v = 1; v <= 50; ++v) {
        for (int u : moves[v]) if (!win[u]) win[v] = true;
        assert((sg[v] != 0) == win[v]);
    }
    // Independent sum checked by a direct P/N recurrence, not SG itself.
    bool sum_win[31][31]{};
    for (int a = 0; a <= 30; ++a)
        for (int b = 0; b <= 30; ++b) {
            for (int u : moves[a]) if (!sum_win[u][b]) sum_win[a][b] = true;
            for (int v : moves[b]) if (!sum_win[a][v]) sum_win[a][b] = true;
            assert(sum_win[a][b] == ((sg[a] ^ sg[b]) != 0));
        }
    assert((sg_ordered_dag({{}, {0}, {0, 1}}) == vector<int>{0, 1, 2}));
    for (vector<int> takes : {vector<int>{1, 3}, vector<int>{1, 2}}) {
        vector<vector<int>> graph(40);
        for (int v = 1; v < 40; ++v)
            for (int take : takes) if (take <= v) graph[v].push_back(v - take);
        auto grundy = sg_ordered_dag(graph);
        for (int v = 0; v < 40; ++v)
            assert((grundy[v] == 0) ==
                   (takes.back() == 3 ? v % 2 == 0 : v % 3 == 0));
    }
    for (unsigned long long x = 0; x < 100; ++x)
        assert(nim_xor({x, x}) == 0); // Equal heaps admit a mirror reply.
}

bool test_trie_and_palindrome() {
#if __has_include("../../competition/string/trie.hpp") && \
    __has_include("../../competition/string/palindrome.hpp")
    using namespace personal_trie;
    trie_init();
    trie_insert("ab"); trie_insert("abc"); trie_insert("ab");
    assert(trie_prefix("a") == 3 && trie_count("ab") == 2);
    assert(trie_count("a") == 0 && trie_prefix("ac") == 0);
    trie_init(); assert(trie_prefix("a") == 0 && tot == 0);
    trie_insert(""); assert(trie_count("") == 1 && trie_prefix("") == 1);
    mt19937 rng(20261024);
    for (int run = 0; run < 100; ++run) {
        trie_init();
        vector<string> words;
        for (int step = 0; step < 200; ++step) {
            string s;
            for (int i = int(rng() % 12); i > 0; --i)
                s += char('a' + rng() % 3);
            words.push_back(s); trie_insert(s);
            for (int length = 0; length <= int(s.size()); ++length) {
                string p = s.substr(0, length);
                int exact = 0, prefix = 0;
                for (const string& t : words) {
                    exact += (t == p);
                    prefix += (t.size() >= p.size() && t.compare(0, p.size(), p) == 0);
                }
                assert(trie_count(p) == exact && trie_prefix(p) == prefix);
            }
        }
    }
    trie_init();
    string deep(TRIE_N - 1, 'a');
    trie_insert(deep); assert(tot == TRIE_N - 1 && trie_count(deep) == 1);
    trie_init(); assert(tot == 0 && trie_count(deep) == 0);
    for (int run = 0; run < 4000; ++run) {
        string s;
        for (int i = int(rng() % 18); i > 0; --i) s += char('a' + rng() % 4);
        long long count = 0;
        int best = 0;
        for (int l = 0; l < int(s.size()); ++l)
            for (int r = l + 1; r <= int(s.size()); ++r) {
                string t = s.substr(l, r - l), rev = t;
                reverse(rev.begin(), rev.end());
                bool ok = (t == rev);
                assert(is_palindrome(s, l, r) == ok);
                if (ok) { ++count; best = max(best, r - l); }
            }
        auto info = palindrome_centers(s);
        assert(info.count == count && info.r - info.l == best);
        assert(is_palindrome(s, info.l, info.r));
        auto p = palindrome_permutation(s);
        array<int, 256> freq{};
        for (unsigned char c : s) ++freq[c];
        int odd = 0;
        for (int x : freq) odd += x % 2;
        assert(p.has_value() == (odd <= 1));
        if (p) {
            string want = s, got = *p;
            sort(want.begin(), want.end()); sort(got.begin(), got.end());
            assert(want == got && is_palindrome(*p, 0, int(p->size())));
        }
    }
    // Enumerate every fill for small binary-palindrome patterns.
    for (int n = 0; n <= 7; ++n) {
        int patterns = 1;
        for (int i = 0; i < n; ++i) patterns *= 3;
        for (int code = 0; code < patterns; ++code) {
            int value = code;
            string pattern;
            for (int i = 0; i < n; ++i) {
                pattern += string("01?")[value % 3]; value /= 3;
            }
            for (int zeros = 0; zeros <= n; ++zeros) {
                bool possible = false;
                for (int mask = 0; mask < (1 << n); ++mask) {
                    string candidate;
                    for (int i = 0; i < n; ++i)
                        candidate += ((mask >> i) & 1) ? '1' : '0';
                    if (int(std::count(candidate.begin(), candidate.end(), '0')) != zeros)
                        continue;
                    string rev = candidate; reverse(rev.begin(), rev.end());
                    if (candidate != rev) continue;
                    bool matches = true;
                    for (int i = 0; i < n; ++i)
                        if (pattern[i] != '?' && pattern[i] != candidate[i]) matches = false;
                    possible |= matches;
                }
                auto got = binary_palindrome_fill(pattern, zeros, n - zeros);
                assert(got.has_value() == possible);
                if (got) {
                    assert(is_palindrome(*got, 0, n));
                    assert(std::count(got->begin(), got->end(), '0') == zeros);
                    for (int i = 0; i < n; ++i)
                        assert(pattern[i] == '?' || pattern[i] == (*got)[i]);
                }
            }
        }
    }
    assert(is_decimal_palindrome("0"));
    assert(is_decimal_palindrome("1221"));
    assert(!is_decimal_palindrome("010"));
    assert(!is_decimal_palindrome("-121"));
    assert(!is_decimal_palindrome(""));
    return true;
#else
    cerr << "MISSING: trie and palindrome modules\n";
    return false;
#endif
}

int main() {
    test_mex(); test_game();
    if (!test_trie_and_palindrome()) return 1;
    cout << "game_string: MEX 120000 + Nim/SG + Trie + palindrome exhaustive PASS\n";
}
#endif
