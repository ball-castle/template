#pragma once
#include <bits/stdc++.h>
using namespace std;

// check 在 [l,r] 上只能 false -> true，check(r) 为 true。
// 返回转折点近似值；边界、r-l、check 运算须有限。
template<class Check>
long double real_first_true(long double l, long double r, Check check) {
    assert(l <= r && check(r));
    for (int it = 0; it < 200; ++it) {
        long double mid = l + (r - l) / 2;
        if (check(mid)) r = mid;
        else l = mid;
    }
    return r;
}
// f 连续且先严格下降后严格上升，可有连续平底最小区间。
// 也允许全程单调；不得在非最优值处存在平坦区间。
// 数值噪声/多峰函数无保证；200 次不能修复错误模型。
template<class Function>
long double ternary_min(long double l, long double r, Function f) {
    assert(l <= r);
    for (int it = 0; it < 200; ++it) {
        long double m1 = l + (r - l) / 3;
        long double m2 = r - (r - l) / 3;
        if (f(m1) <= f(m2)) r = m2;
        else l = m1;
    }
    return l + (r - l) / 2;
}
