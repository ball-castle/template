#include "../../competition/runtime/int128_input.hpp"
int main() {
    const vector<string> edges = {"0", "-0", "+00017", "-00017",
        "170141183460469231731687303715884105727",
        "-170141183460469231731687303715884105728"};
    __int128 mx = (__int128)(((__uint128_t)1 << 127) - 1);
    vector<__int128> expected_edges = {0, 0, 17, -17, mx, -mx - 1};
    for (int i = 0; i < int(edges.size()); ++i)
        assert(parse_i128(edges[i]) == expected_edges[i]);
    mt19937_64 rng(20261025);
    for (int i = 0; i < 5000; ++i) {
        unsigned long long high = rng() >> 1, low = rng();
        __int128 value = (__int128)(((__uint128_t)high << 64) + low);
        if (rng() & 1) value = -value;
        string s = to_string_i128(value); // 复用ROUND01独立验证的除10输出函数。
        assert(parse_i128(s) == value);
    }
    istringstream input("+17 -42 170141183460469231731687303715884105727");
    string s;
    for (const string expected : {"17", "-42", "170141183460469231731687303715884105727"}) {
        input >> s;
        assert(to_string_i128(parse_i128(s)) == expected);
    }
    assert(!(input >> s));
    istringstream ll_input("-9223372036854775808 9223372036854775807");
    ll lo, hi;
    assert((ll_input >> lo >> hi) && lo == LLONG_MIN && hi == LLONG_MAX);
    assert(!(ll_input >> lo));
    cout << "PASS i128 cin-string parsing: 5009 checks\n";
}
