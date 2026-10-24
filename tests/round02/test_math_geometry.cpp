#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;
#include "../../math/modular.hpp"
namespace pe {
#include "../../competition/math/exgcd.hpp"
}
namespace pp {
#include "../../competition/math/primes.hpp"
}
namespace pd {
#include "../../competition/math/integer_division.hpp"
}
namespace ps {
#include "../../competition/math/sums.hpp"
}
namespace pi {
#include "../../competition/math/invariants.hpp"
}
namespace pg {
#include "../../competition/math/integer_geometry.hpp"
}
mt19937_64 rng(20261024);
void check(bool ok, const string& message) {
    if (!ok) { cerr << "FAIL: " << message << '\n'; exit(1); }
}
ll rnd(ll a, ll b) { return uniform_int_distribution<ll>(a, b)(rng); }

void test_exgcd() {
    for (ll a = -80; a <= 80; ++a) for (ll b = -80; b <= 80; ++b) {
        ll x, y, g = pe::exgcd(a, b, x, y);
        check(g == gcd(a, b) && (i128)a * x + (i128)b * y == g, "exgcd signed Bezout");
    }
    for (int rep = 0; rep < 20000; ++rep) {
        ll a = rnd(-1000000000000000000LL, 1000000000000000000LL);
        ll b = rnd(-1000000000000000000LL, 1000000000000000000LL);
        ll x, y, g = pe::exgcd(a, b, x, y);
        check(g == gcd(a, b) && (i128)a * x + (i128)b * y == g, "exgcd large coefficients");
    }
    for (ll mod = 2; mod <= 100; ++mod) for (ll a = -100; a <= 100; ++a) {
        ll want = -1;
        for (ll x = 0; x < mod; ++x) if ((a * x % mod + mod) % mod == 1) want = x;
        ll got = pe::inverse_mod(a, mod);
        check(got == want, "inverse vs residue enumeration");
        if (pp::is_prime(mod) && a % mod != 0)
            check(got == mod_pow(a, mod - 2, mod), "inverse same data round01 modular");
    }
    check(pe::inverse_mod(3, 8) == 3 && pe::inverse_mod(2, 8) == -1, "composite modulus");
}

void test_primes() {
    pp::sieve(10000);
    int count = 0;
    for (int n = 1; n <= 10000; ++n) {
        int smallest = 0;
        for (int d = 2; d <= n; ++d) if (n % d == 0) { smallest = d; break; }
        bool prime = n >= 2 && smallest == n;
        check(pp::lp[n] == smallest && pp::is_prime(n) == prime, "sieve/trial vs divisor scan");
        if (prime) check(pp::primes[++count] == n, "sieve sorted primes");
        auto factors = pp::factorize(n); ll product = 1;
        for (auto [p, exponent] : factors) {
            check(pp::is_prime(p) && exponent > 0, "factor exponents");
            for (int k = 0; k < exponent; ++k) product *= p;
        }
        check(product == n, "factor reconstruction");
        if (n <= 1000) {
            vector<ll> want;
            for (int d = 1; d <= n; ++d) if (n % d == 0) want.push_back(d);
            check(pp::divisors(n) == want, "all divisors direct enumeration");
        }
    }
    check(count == pp::cnt, "sieve count");
    pp::sieve(5); check(pp::cnt == 3 && pp::lp[1] == 0, "sieve small after large");
    pp::sieve(1000000); check(pp::cnt == 78498, "million sieve known count");
    auto factors = pp::factorize(1000000000000LL);
    check(factors == vector<pair<ll, int>>{{2, 12}, {5, 12}}, "factor upper boundary");
    check(!pp::is_prime(0) && !pp::is_prime(1) && !pp::is_prime(-7), "not prime boundaries");
}

void test_division() {
    for (ll a = -100; a <= 100; ++a) for (ll b = -30; b <= 30; ++b) if (b) {
        ll lo = -1000, hi = 1000;
        // 独立定义：把分母改成正数，再比较整数乘法。
        i128 numerator = b < 0 ? -(i128)a : a;
        i128 denominator = b < 0 ? -(i128)b : b;
        while ((i128)(lo + 1) * denominator <= numerator) ++lo;
        while ((i128)(hi - 1) * denominator >= numerator) --hi;
        check(pd::floor_div(a, b) == lo && pd::ceil_div(a, b) == hi, "division exhaustive inequality");
    }
    vector<ll> values{LLONG_MIN, LLONG_MIN + 1, -9, -1, 0, 1, 9, LLONG_MAX};
    for (ll a : values) for (ll b : values) if (b && !(a == LLONG_MIN && b == -1)) {
        i128 num = b < 0 ? -(i128)a : a, den = b < 0 ? -(i128)b : b;
        i128 lo = pd::floor_div(a, b), hi = pd::ceil_div(a, b);
        check(lo * den <= num && (lo + 1) * den > num, "floor full ll inequality");
        check(hi * den >= num && (hi - 1) * den < num, "ceil full ll inequality");
    }
}

void test_sums_invariants() {
    i128 sum = 0, squares = 0;
    for (ll n = 0; n <= 10000; ++n) {
        sum += n; squares += n * n;
        check(ps::sum1(n) == sum && ps::sum2(n) == squares, "sum formulas vs loop");
    }
    for (int rep = 0; rep < 1000; ++rep) {
        ll first = rnd(-100, 100), step = rnd(-100, 100), count = rnd(0, 100);
        i128 want = 0;
        for (ll k = 0; k < count; ++k) want += first + step * k;
        check(ps::arithmetic_sum(first, step, count) == want, "arithmetic progression");
        vector<ll> a((size_t)count);
        for (ll& x : a) x = rnd(-1000000000000000000LL, 1000000000000000000LL);
        want = 0;
        for (int i = 0; i < (int)a.size(); ++i) for (int j = i + 1; j < (int)a.size(); ++j) {
            i128 d = (i128)a[i] - a[j]; want += d < 0 ? -d : d;
        }
        check(ps::pair_abs_sum(a) == want, "pair difference coefficient vs n^2");
    }
    check(ps::sum1(1000000000000000000LL) > LLONG_MAX, "sum1 wide boundary");
    check(ps::sum2(1000000000000LL) > LLONG_MAX, "sum2 wide boundary");
    for (ll mod = 1; mod <= 50; ++mod) for (ll step = -50; step <= 50; ++step)
        for (ll start : {-31LL, 0LL, 37LL}) {
            set<ll> visited; ll pos = pi::norm_mod(start, mod);
            while (visited.insert(pos).second) pos = pi::norm_mod(pos + step, mod);
            for (ll target = -55; target <= 55; ++target)
                check(pi::reachable_mod(start, target, step, mod) == visited.count(pi::norm_mod(target, mod)),
                      "mod reachability vs orbit enumeration");
        }
    for (ll a : {LLONG_MIN, -7LL, 0LL, LLONG_MAX}) for (ll mod : {1LL, 2LL, 17LL, LLONG_MAX}) {
        check(pi::norm_mod(a, mod) == mod_norm(a, mod), "norm same data round01");
        check(pi::congruent(a, a, mod), "congruence reflexive");
    }
    check(pi::reachable_mod(LLONG_MIN, LLONG_MAX, 1, 17), "wide endpoint difference");
}

// 独立几何 oracle：参数方程 a+t(b-a)=c+s(d-c)，有理数分子分母。
bool oracle_intersect(pg::Point a, pg::Point b, pg::Point c, pg::Point d) {
    ll ux = b.x - a.x, uy = b.y - a.y, vx = d.x - c.x, vy = d.y - c.y;
    ll wx = c.x - a.x, wy = c.y - a.y;
    ll den = ux * vy - uy * vx;
    if (den) {
        ll tn = wx * vy - wy * vx, sn = wx * uy - wy * ux;
        if (den < 0) den = -den, tn = -tn, sn = -sn;
        return 0 <= tn && tn <= den && 0 <= sn && sn <= den;
    }
    // 小坐标穷举两个端点所在的轴向包围盒与共线关系。
    auto lies = [](pg::Point p, pg::Point x, pg::Point y) {
        ll ex = y.x - x.x, ey = y.y - x.y;
        return (p.x - x.x) * ey == (p.y - x.y) * ex
            && p.x >= min(x.x, y.x) && p.x <= max(x.x, y.x)
            && p.y >= min(x.y, y.y) && p.y <= max(x.y, y.y);
    };
    return lies(a, c, d) || lies(b, c, d) || lies(c, a, b) || lies(d, a, b);
}

void test_geometry() {
    vector<pg::Point> pts;
    for (ll x = -1; x <= 1; ++x) for (ll y = -1; y <= 1; ++y) pts.push_back({x, y});
    for (auto a : pts) for (auto b : pts) for (auto c : pts) for (auto d : pts)
        check(pg::segments_intersect(a, b, c, d) == oracle_intersect(a, b, c, d), "segment exhaustive 9^4");
    for (int rep = 0; rep < 20000; ++rep) {
        pg::Point a{rnd(-100, 100), rnd(-100, 100)}, b{rnd(-100, 100), rnd(-100, 100)};
        pg::Point c{rnd(-100, 100), rnd(-100, 100)}, d{rnd(-100, 100), rnd(-100, 100)};
        check(pg::segments_intersect(a, b, c, d) == oracle_intersect(a, b, c, d), "segment rational oracle");
        check(pg::cross(a, b, c) == -pg::cross(a, c, b), "cross antisymmetry");
        check(pg::dot(a, b, c) == pg::dot(a, c, b), "dot symmetry");
    }
    const ll B = 1000000000000000000LL;
    check(pg::segments_intersect({-B, -B}, {B, B}, {-B, B}, {B, -B}), "geometry 1e18 crossing");
    check(pg::orientation({-B, -B}, {B, -B}, {-B, B}) == 1, "geometry wide determinant");
    check(pg::dot({-B, -B}, {B, B}, {B, B}) == (i128)8 * B * B, "dot max magnitude");
    check(!pg::segments_intersect({0, 0}, {0, 0}, {1, 1}, {1, 1}), "distinct point segments");
}

void test_printed_usage() {
    { // PM01
        using namespace pe;
        ll x,y; ll d=exgcd(30,18,x,y); ll inv=inverse_mod(3,8);
        check(d == 6 && 30*x+18*y == 6 && inv == 3, "PM01 usage");
    }
    { // PM02
        using namespace pp;
        sieve(1000); bool p=is_prime(97); auto f=factorize(60); auto d=divisors(60);
        check(p && f == vector<pair<ll,int>>({{2,2},{3,1},{5,1}}) && d.size() == 12,
              "PM02 usage");
    }
    { // PM03
        using namespace pd;
        ll l=floor_div(-7,3); ll r=ceil_div(-7,3);
        check(l == -3 && r == -2, "PM03 usage");
    }
    { // PM04
        using namespace ps;
        auto s=sum1(100); auto q=sum2(100); auto a=arithmetic_sum(3,2,5);
        auto d=pair_abs_sum({1,3,6});
        check(s == 5050 && q == 338350 && a == 35 && d == 10, "PM04 usage");
    }
    { // PM05
        using namespace pi;
        ll r=norm_mod(-3,5); bool ok=reachable_mod(1,7,4,10); bool same=congruent(-3,2,5);
        check(r == 2 && ok && same, "PM05 usage");
    }
    { // PM06
        using namespace pg;
        Point a{0,0},b{2,2},c{0,2},d{2,0};
        bool ok=segments_intersect(a,b,c,d); int turn=orientation(a,b,c);
        check(ok && turn == 1, "PM06 usage");
    }
}

int main() {
    test_printed_usage();
    test_exgcd(); test_primes(); test_division(); test_sums_invariants(); test_geometry();
    cout << "PASS math/geometry: seed=20261024; signed/full-ll division; sieve; 1e18 geometry\n";
}
