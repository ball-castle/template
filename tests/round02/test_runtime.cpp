#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
namespace original_read {
#include "../../read.cpp"
}
#include "../../core/basics.hpp"
#if __has_include("../../competition/runtime/floating.hpp")
#include "../../competition/runtime/floating.hpp"
#include "../../competition/runtime/real_search.hpp"
#else
// Test-first gate: replace by the real header once it exists.
int main() { cerr << "MISSING: floating.hpp\n"; return 1; }
#endif

#if __has_include("../../competition/runtime/floating.hpp")
// Every displayed IO recipe below is executed, using a private input stream.
void test_cin_recipe() {
    istringstream input("7 -9000000000 hello Z\n  keep spaces\n");
    ostringstream output;
    streambuf* old_in = cin.rdbuf(input.rdbuf());
    streambuf* old_out = cout.rdbuf(output.rdbuf());
    int n; long long x; string s, line; char c;
    cin >> n >> x >> s >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, line);
    cout << fixed << setprecision(3) << 1.0 / 8 << '\n';
    cout << to_string_i128((__int128)x * n) << '\n';
    cin.rdbuf(old_in); cout.rdbuf(old_out);
    assert(n == 7 && x == -9000000000LL && s == "hello" && c == 'Z');
    assert(line == "  keep spaces");
    assert(output.str() == "0.125\n-63000000000\n");
    ostringstream significant, decimals;
    significant << setprecision(3) << 12.3456;
    decimals << fixed << setprecision(3) << 12.3456;
    assert(significant.str() == "12.3" && decimals.str() == "12.346");
    char text[80];
    snprintf(text, sizeof text, "%.10f", 0.125);
    assert(string(text) == "0.1250000000");
    assert(to_string_i128(0) == "0");
    __int128 low = -(__int128(1) << 126);
    low += low;
    assert(to_string_i128(low) ==
           "-170141183460469231731687303715884105728");
}

void test_original_read_valid_integers() {
    mt19937_64 rng(20261024);
    vector<long long> want{0, -1, 1, 42, -20261024,
                           1000000000000000000LL, -1000000000000000000LL};
    for (int i = 0; i < 50000; ++i)
        want.push_back((long long)(rng() % 2000000000001ULL) - 1000000000000LL);
    auto path = filesystem::temp_directory_path() /
                "acm02_runtime_legal_read_input.txt";
    {
        ofstream file(path);
        for (size_t i = 0; i < want.size(); ++i)
            file << want[i] << (i % 3 == 0 ? '\n' : ' ');
    }
    FILE* input = freopen(path.string().c_str(), "r", stdin);
    assert(input);
    for (long long x : want) assert(original_read::read() == x);
    // Exactly the declared amount is read; no call is made at EOF.
    fclose(input);
    filesystem::remove(path);
}

void test_floating_geometry_and_search() {
    assert(fp_equal(1.0L, 1.0L + 1e-13L));
    assert(!fp_equal(1.0L, 1.1L));
    assert(floor(-1.2L) == -2 && ceil(-1.2L) == -1);
    assert(trunc(-1.8L) == -1 && round(-1.5L) == -2);
    assert(round(1.5L) == 2);
    assert(fabsl(distance_2d(0, 0, 3, 4) - 5) < 1e-12L);
    assert(fabsl(angle_between(1, 0, 0, 1) - PI / 2) < 1e-12L);
    assert(fabsl(angle_between(1, 0, -1, 0) - PI) < 1e-12L);
    assert(fabsl(direction_angle(0, -1) - 3 * PI / 2) < 1e-12L);
    assert(fabsl(degrees(PI) - 180) < 1e-12L);
    assert(fabsl(radians(90) - PI / 2) < 1e-12L);
    assert(safe_acos(1 + 1e-15L) == 0);
    assert(fabsl(safe_asin(-1 - 1e-15L) + PI / 2) < 1e-12L);
    assert(fabsl(atan(1.0L) - PI / 4) < 1e-12L);
    assert(fabsl(normalize_angle(-9 * PI / 2) - 3 * PI / 2) < 1e-12L);
    // Tiny negative angles can round to exactly 2*PI after adding a period.
    assert(0 <= normalize_angle(-1e-30L) && normalize_angle(-1e-30L) < 2 * PI);
    mt19937 rng(20261024);
    for (int it = 0; it < 2000; ++it) {
        long double value = (rng() % 1000000 + 1) / 100.0L;
        long double root = real_first_true(0, max(1.0L, value),
            [=](long double x) { return x * x >= value; });
        assert(fabsl(root - sqrt(value)) < 1e-9L);
        long double center = (int(rng() % 10001) - 5000) / 100.0L;
        long double opt = ternary_min(-100, 100,
            [=](long double x) { return (x - center) * (x - center) + 7; });
        assert(fabsl(opt - center) < 1e-6L);
        long double theta = (rng() % 628319) / 100000.0L;
        assert(fabsl(direction_angle(cos(theta), sin(theta)) - theta) < 1e-10L);
    }
    assert(fabsl(real_first_true(2, 2, [](long double) { return true; }) - 2)
           < 1e-12L);
    assert(fabsl(ternary_min(-2, 3, [](long double x) { return x; }) + 2)
           < 1e-9L);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    test_cin_recipe();
    test_floating_geometry_and_search();
    test_original_read_valid_integers();
    cout << "runtime: legal read 50007 + IO + FP/search 2000 PASS\n";
}
#endif
