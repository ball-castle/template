# 运行基础与数值边界

本页引用唯一规范源码与模块元数据，不复制第二套实现。

- **IO01 / `read.cpp`**：原版 `inline ll read()` 完整原样引用，用户已验证使用。读数个数和范围明确时直接用；不能读过已知个数，不宣称 EOF、LLONG_MIN、超域支持。对应特殊需求直接用 `cin`。
- **IO02 / `core/main.cpp`、CO02 / `core/basics.hpp`**：关闭同步、解绑；字符串/字符按空白提取，整行先处理残留换行。普通整数可直接 `cout`，i128调用原 `to_string_i128`。测试中的 `test_cin_recipe` 实际执行所有示例。
- **FP01 / `competition/runtime/floating.hpp`**：距离 `hypotl`；方向 `atan2l(y,x)`；夹角先单位化再 `acosl`。π由 `acosl(-1)` 得到，弧度转角度乘180/π。
- **TRS01 / `competition/runtime/real_search.hpp`**：200轮二分/三分沿用用户 `Ternary_Search.cpp` 的循环风格；单调与单谷前提分别证明。

`fixed << setprecision(k)` 显示小数点后k位；单独 `setprecision(k)` 默认显示k位有效数字。`printf("%.10f",x)`要求double，long double配`%Lf`。显示固定小数与十进制精确舍入是不同任务；货币、计数、奇偶等可按整数单位处理时优先整数。

`floor(-1.2)=-2`、`ceil(-1.2)=-1`、`trunc(-1.8)=-1`，`round(±1.5)=±2`。强转整数向0截断，但浮点必须先处在目标整数可表示范围。float有效位较少，竞赛通常使用double/long double；long double具体精度由实现决定，可查`numeric_limits<long double>::digits`。

EPS不是统一魔法常数。比较接近相等时按绝对误差与相对量级选阈值；排序必须保持严格弱序，不写`a < b-EPS`。几何定义域已被数学证明时才允许对微小误差夹紧；把非法的acos参数2夹成1会掩盖模型错误。

实数二分前写`check`的单调方向与可行端点，三分前证明最小区间外严格下降/上升。非最优平台不能无条件三分：两点值相等时，最优点可能仍在右方；多峰函数也会被错误剪枝。测试以已知根/顶点为独立答案，迭代次数不能代替模型证明。

## 已执行验证

Windows GNU++20 `-O2 -Wall -Wextra -Wshadow`；WSL GNU++20 `-O1 -fsanitize=address,undefined -fno-pie -no-pie`。`tests/round02/test_runtime.cpp` 实测50007个合法原read输入、IO与i128最小值输出、2000组数值几何与搜索。新源不修改任何ROUND01原文件。
