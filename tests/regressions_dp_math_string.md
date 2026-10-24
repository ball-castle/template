# DP / 数学 / 字符串：旧错误与验证记录

日期：2026-10-24。旧文件保持原样；下表来自原文件或直接包含原函数的测试程序，没有改写旧算法再声称复现。旧版本基准为子模块提交 `8c9f89a`。

## 已执行的最小反例

| 旧文件 / 接口 | 输入 | 正确结果 | 实际结果与原因 | 新模块 |
|---|---|---|---|---|
| `01bag.cpp` | `n=2, cap=2; (w,v)=(1,2000000000)` 两件 | 4000000000 | UBSan：两个 int 价值相加溢出 | DP01 使用 ll |
| `ksm.cpp` | `a=-2,b=3,p=5` | 标准余数2 | -3；负数未归一化 | M02 |
| `ksm.cpp` | `a=3,b=0,p=1` | 0 | 1；乘法单位元未取模 | M02 |
| `ksm.cpp` | `a=9223372036854775806,b=2,p=9223372036854775807` | 1 | UBSan：平方触发 signed integer overflow | M02 先转 `__int128` |
| `gcd.cpp` | `a=-6,b=-4` | 非负 gcd=2 | -2；原函数未给负数规范 | M01 |
| `matrix_ksm.cpp` | `4 1 1 1 1` | 1 | 0；答案初值0后连续相乘 | M04 从1开始 |
| `matrix_ksm.cpp` | `3 2 3 5 7` | 扩展递推边界应为5 | 0 | M04 直接返回初值 |
| `matrix_ksm.cpp` | `2 2 3 5 7` | 扩展递推边界应为3 | 运行1秒仍未退出后终止；n-3为负，右移不归零 | M04 在求幂前处理n<=3 |
| `KMP.cpp::kmp` | `s="a",p=""` | 约定空模式匹配 `{0,1}` | UBSan：对空 pi 使用偏移 -1 | S01 单独处理空模式 |
| `区间dp.cpp` | `n=2; a=[1000000000,1000000000]` | 2000000000 | 1061109567；`0x3f3f3f3f` 比合法答案小 | DP04 |
| `Mobius_inversion.cpp::get_mu` | `get_mu(4)[4]` | 0 | -1；每个x都执行 `mu[x]=-1`，覆盖合数已筛出的值 | 只保留审计，不纳入本批正式代码 |

`1182E` 官方输入规定 n>=4 且初值在1..10^9。n<4 和模P为0的底数是新函数额外支持的边界，不用它们推断原题提交结论。`get_mu` 是旧文件中另一份函数，旧 `solve` 实际调用 `init_mu`；上述反例不能推出旧主程序的筛法出错。KMP 的空模式同样是函数复用边界，旧 `cin >> p` 输入路径不能读出空模式。

原背包的正序/倒序与原 LIS 的状态转移是正确的；保留其思想。DP01 新增“恰好重量”语义，不能将原来的“重量不超过容量”答案当成恰好装满答案。例如一件 `(w,v)=(2,5)`、容量3，前者为5，后者不可达。DP04 保留区间转移，修正数据范围与空输入。

## 正式测试

测试文件：`tests/test_dp_math_string.cpp`，固定种子 `20261024`。

- 600组背包：枚举物品数量，与0/1、完全背包的“至多/恰好”四种结果对照。
- 350组 LIS：枚举所有子序列，同时对照严格和非严格、二分和朴素版本。
- 250组 Hamilton：中间点全排列对照，包含有向缺边与负边；另外测试n=0/1。
- 180组区间合并：枚举所有相邻合并次序，另测超过32位的前缀和及答案。
- 1500组模幂：朴素连乘对照；单测负数、模1、指数0、LLONG_MIN、接近LLONG_MAX的乘积与和。
- gcd/lcm/位运算：负数、0、LLONG_MIN、溢出拒绝、第63位，枚举所有mask<128的子集。
- 200组矩阵：朴素重复乘法对照，含负元素、模1、0次幂和64位边界。
- 200组乘积递推：n=1..24直接递推对照，另外核对1182E两条官方样例、n=10^18与零底数。
- 组合数：n<=30全部与 Pascal 三角对照，另测小质数及质数 `2^61-1`。
- 1200组字符串：KMP、pi、Z分别与朴素匹配、前后缀枚举、直接LCP对照；子串双哈希与独立重建哈希对照，样本相等关系再与原串比较。
- 基础字符串：空串、空/偶长回文、连续和末尾分隔符、ASCII转换、相邻分组。

所有随机差分均使用独立枚举或朴素算法；哈希测试通过不代表不存在碰撞。非法模数、非质数传给PrimeComb、负背包重量、超出契约的整数范围不是支持的输入。

Windows GNU g++15.2：

```powershell
g++ -std=gnu++20 -O2 -Wall -Wextra -Wshadow `
  tests/test_dp_math_string.cpp -o build/dp_math_string/test_all.exe
./build/dp_math_string/test_all.exe
```

急用版仅编译P0，加 `-DP0_ONLY`。实际结果为退出码0，输出 `PASS dp/math/string fixed-seed differential tests`，无编译警告。

最终全量复验：Windows GNU g++15.2.0 使用上面命令通过，退出码0且无警告。WSL Ubuntu GNU g++13.3.0 使用 `-O1 -g -Wall -Wextra -Wshadow -fsanitize=address,undefined -fno-omit-frame-pointer` 编译并执行，包含追加的 DP04 在内均通过，退出码0，无 sanitizer 报告。

12份正式头文件分别生成只含该头文件和 `int main(){}` 的独立翻译单元，用 GNU++20、`-Wall -Wextra -Wshadow -fsyntax-only` 全部编译通过。12份JSON字段及源文件路径验证通过；每个头文件30至47行，按中文2列/ASCII1列计算均小于85列。`git diff HEAD -- '*.cpp'` 为空，原根目录cpp未修改。没有提交或推送。

相关题面已核对：[1182E](https://codeforces.com/problemset/problem/1182/E)、[1475E](https://codeforces.com/problemset/problem/1475/E)、[271D](https://codeforces.com/problemset/problem/271/D)、[266A](https://codeforces.com/problemset/problem/266/A)、[CSES 1690](https://cses.fi/problemset/task/1690)、[CSES 1722](https://cses.fi/problemset/task/1722)、[CSES 1753](https://cses.fi/problemset/task/1753)。仅记录本地验证，不宣称在线AC。
