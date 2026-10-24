# 核心与数据结构：先失败、后修复记录

2026-10-24，Windows GCC 15.2.0 与 WSL Ubuntu GCC 13.3.0。
根目录旧源码只读取和编译；没有修改、移动或删除。

## 1. Fenwick 的调用接口不一致

在仓库根目录执行：

```powershell
g++ -std=gnu++20 -O0 -g -c Fenwick_tree.cpp -o old_fenwick.o
```

修复前退出码 1，编译器诊断：

```text
Fenwick_tree.cpp:61:25: error: 'struct Fenwick' has no member named 'rangeSum'
```

旧结构定义 `query(l,r)`，主程序却调用 `rangeSum(l,r)`。
新模板统一成 `range_sum(l,r)`；正式测试用两个点 5、7，核验结果 12。
`add(0,x)` 原本会死循环，新模板明确要求位置 1..n 并断言。

## 2. 懒标记线段树先有编译错误

```powershell
g++ -std=gnu++20 -O0 -g -c 'Segment tree2.cpp' -o old_lazy.o
```

修复前退出码 1：`pushdown` 调用 `apply` 时尚无声明；编译器报
`no matching function for call to 'apply(...)'`。
以下独立驱动只补充声明以使旧代码合法编译，没有更改旧函数实现。
文件放在构建目录 `build/core_data/old_lazy_driver.cpp`，不属于正式模板。

```cpp
#include <bits/stdc++.h>
void apply(int node, int l, int r, long long val);
#ifdef _WIN32
#include "C:/Users/TSOU/Desktop/cf/template/Segment tree2.cpp"
#else
#include "/mnt/c/Users/TSOU/Desktop/cf/template/Segment tree2.cpp"
#endif
int main(int argc, char** argv) {
    std::string mode = argc > 1 ? argv[1] : "pushdown";
    if (mode == "overflow") {
        a[1] = 2147483647; a[2] = 1;
        build(1, 1, 2);
        long long got = query(1, 1, 2, 1, 2);
        std::cout << "expected=2147483648 actual=" << got << '\n';
        return got != 2147483648LL;
    }
    a[1] = a[2] = 0;
    build(1, 1, 2);
    modify(1, 1, 2, 1, 2, 1);
    long long got = query(1, 1, 2, 1, 1);
    std::cout << "expected=1 actual=" << got << '\n';
    return got != 1;
}
```

驱动行为是旧代码对合法数组和合法区间的真实调用，不是模拟实现。

## 3. 未下传 lazy：两个元素即能失败

数组 `[0,0]`，先把 `[1,2]` 加 1，再查询 `[1,1]`。
驱动 `pushdown` 模式在 Windows 和 WSL 均退出 1：

```text
expected=1 actual=0
```

旧 `modify` 已更新根节点与根标记，但旧 `query` 查询子节点前未下传。
新 `LazySegmentTree::query` 在部分覆盖时先调用 `push`。
正式测试保留 `legacy query pushdown regression`，并继续测试叠加与子段更新。

## 4. int 区间和溢出：两个元素即能失败

数组 `[2147483647,1]`，查询整段，应为 `2147483648`。
Windows GCC `-O0` 的旧驱动 `overflow` 模式退出 1：

```text
expected=2147483648 actual=-2147483648
```

有符号溢出属于未定义行为，不能依赖上面的具体错误数值。
另用 WSL UBSan 编译运行，证据如下：

```text
g++ -std=gnu++20 -O1 -g -fsanitize=undefined
    -fno-sanitize-recover=all build/core_data/old_lazy_driver.cpp
    -o build/core_data/old_lazy_ubsan
build/core_data/old_lazy_ubsan overflow

Segment tree2.cpp:29:33: runtime error: signed integer overflow:
2147483647 + 1 cannot be represented in type 'int'
```

新节点、更新值和查询答案统一为 `ll`；长度乘法用 `1LL` 提升。
这并不代表允许 ll 溢出：模块契约仍要求所有节点和、累计标记、
长度乘增量及中间结果处于 ll 范围。
正式测试保留 `legacy int overflow regression` 和 30 亿增量反例。

## 5. 新接口的红绿记录

正式测试先于新模板写入，覆盖安全 i128 输出（包括 -2^127）、空数组、
前缀/差分、离散化、全 ll 范围二分、双指针、非负滑窗、树与单调结构。
模板尚未创建时，临时检测分支运行并退出 1：

```text
FAIL: required P0 core/data headers are missing
```

随后创建模块并移除临时检测分支。
P1 稀疏表也是先写测试：头文件尚未创建时退出 1，输出
`FAIL: P1 sparse-table header is missing`；实现后相同测试通过。
`-DP0_ONLY` 会略过 P1 的测试，便于急用版独立验收。

## 6. 修复后的实际验证

Windows 已执行（最终完整测试包括 P1 稀疏表）：

```powershell
g++ -std=gnu++20 -O2 -Wall -Wextra -Wshadow tests/test_core_data.cpp `
    -o build/core_data/test_all.exe
./build/core_data/test_all.exe
```

退出码 0，无编译警告：

```text
PASS core/data: deterministic + random(seed=20261024) + n=200000
```

测试以暴力数组/枚举所有子区间作独立对照；20 万规模包含逐点改值、
20 万次后缀加、总和与末元素检查，能检出退化为逐点区间修改的实现。
主程序另以 GNU++20、全部警告选项独立编译并空输入运行，退出码 0。

WSL 的 P0 与包含 P1 的完整测试均已用 ASan+UBSan 实际通过，
退出码均为 0，输出同上。完整检查命令为：

```text
g++ -std=gnu++20 -O1 -g -Wall -Wextra -Wshadow
    -fsanitize=address,undefined -fno-omit-frame-pointer -fno-pie -no-pie
    tests/test_core_data.cpp -o build/core_data/test_all_san
build/core_data/test_all_san
```

父流程的 `scripts/run_tests.py --p0` 也已在 Windows 与 WSL 各通过
24 项检查、0 失败；最终全仓检查与源码哈希仍由该脚本统一保存。
临时二进制不能放 WSL `/tmp` 跨工具调用依赖；本次改为仓库 `build/`。
