#pragma once
#include <bits/stdc++.h>
using namespace std;

// EPS 随量级/误差预算选；排序比较器仍用精确 <，不要掺 EPS。
constexpr long double EPS = 1e-12L;
const long double PI = acosl(-1.0L);
inline bool fp_equal(long double a, long double b) {
    return fabsl(a - b) <= EPS * max({1.0L, fabsl(a), fabsl(b)});
}
inline long double radians(long double deg) { return deg * PI / 180; }
inline long double degrees(long double rad) { return rad * 180 / PI; }
inline long double normalize_angle(long double rad) {
    rad = fmodl(rad, 2 * PI);
    if (rad < 0) rad += 2 * PI;
    if (rad >= 2 * PI) rad = 0; // 极小负数加一周可能舍入成2π。
    return rad;
}
// 只修正舍入造成的微小越界；原数学输入本来就必须在 [-1,1]。
inline long double safe_acos(long double x) {
    return acosl(clamp(x, -1.0L, 1.0L));
}
inline long double safe_asin(long double x) {
    return asinl(clamp(x, -1.0L, 1.0L));
}
inline long double distance_2d(long double x1, long double y1,
                               long double x2, long double y2) {
    return hypotl(x1 - x2, y1 - y2);
}
// (x,y) 非零；atan2 的参数是 (y,x)，返回方向角 [0,2π)。
inline long double direction_angle(long double x, long double y) {
    assert(x != 0 || y != 0);
    return normalize_angle(atan2l(y, x));
}
// 两个向量均非零且有限；先单位化，避免直接点积/模长乘积溢出。
inline long double angle_between(long double ax, long double ay,
                                 long double bx, long double by) {
    long double na = hypotl(ax, ay), nb = hypotl(bx, by);
    assert(na > 0 && nb > 0 && isfinite(na) && isfinite(nb));
    return safe_acos((ax / na) * (bx / nb) + (ay / na) * (by / nb));
}

