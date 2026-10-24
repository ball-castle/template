#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

// PM06. |坐标|<=1e18。差值<=2e18；叉积/点积绝对值<=8e36。
// 必须先转 i128 再减；本模块不接受任意完整 ll 坐标范围。
struct Point { ll x, y; };

i128 dot(Point a, Point b, Point c) { // (b-a) dot (c-a)。
    return ((i128)b.x - a.x) * ((i128)c.x - a.x)
         + ((i128)b.y - a.y) * ((i128)c.y - a.y);
}

i128 cross(Point a, Point b, Point c) { // (b-a) cross (c-a)。
    return ((i128)b.x - a.x) * ((i128)c.y - a.y)
         - ((i128)b.y - a.y) * ((i128)c.x - a.x);
}

int sign(i128 x) { return (x > 0) - (x < 0); }
int orientation(Point a, Point b, Point c) { return sign(cross(a, b, c)); }

bool on_segment(Point a, Point b, Point p) {
    return cross(a, b, p) == 0
        && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
        && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

bool segments_intersect(Point a, Point b, Point c, Point d) {
    int c1 = orientation(a, b, c), c2 = orientation(a, b, d);
    int c3 = orientation(c, d, a), c4 = orientation(c, d, b);
    if (c1 == 0 && on_segment(a, b, c)) return true;
    if (c2 == 0 && on_segment(a, b, d)) return true;
    if (c3 == 0 && on_segment(c, d, a)) return true;
    if (c4 == 0 && on_segment(c, d, b)) return true;
    return c1 * c2 < 0 && c3 * c4 < 0;
}
