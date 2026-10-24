# ACM 02 02A 02B 统一实施约定

## 最新用户指示优先

用户要求继续上一轮两个 PR，先补丁、后完整手册，不合并 main，不删除旧文件或四份已验证打印成品。

2026-10-24 最新纠正：

1. 快读直接采用旧 `read.cpp` 的 `inline ll read()`，保留 `ll/getchar()`，不改接口、不加安全封装、不替换为 fread 类。常规合法整数输入用原版，其他输入使用 cin。只在适用条件下测试/说明，不把超出原用途的输入当成必须重构的理由。
2. 用户说明原代码经过自己验证使用，除具体小问题外直接收录；树的存储等基础写法也须收录。个人源代码是首选，不以统一形式为理由重写。
3. 正式主打印版本优先全局数组、1-indexed、独立函数，熟悉命名 fa/rk/dp/cnt/a/b/g/tree/lazy。线段树 pushup/pushdown/build/modify/query；DSU init/find/unite/same。需要动态容量时可用清楚的 vector。
4. 用户验证使用过与在线 AC 是两类证据。未查到线上记录写 UNKNOWN，不否定用户的使用经验，也不虚构 AC。

## 文件与并行边界

- 原 32 正式源码、原根目录 32 cpp、原四份 DOCX/PDF 及其 manifest/binding 保持原样。
- 原实现能满足要求时直接引用；只有接口个人化有明确收益或实际缺陷时新增 `competition/<domain>/*.hpp` / `.cpp`，从具体旧源作最小改动。
- 新主版本如与 ROUND01 版并存，必须在同一测试输入与独立 oracle 下同时验证，记录一致性；不形成无人同步维护的双版本。
- 新正式模块元数据 `notes/round02/modules/<ID>.json`。原模块直接引用，无须重复写源码。
- 题型卡 `notes/round02/cards/<DOMAIN>.json`，顶层 JSON 数组。Markdown 教程与打印稿均从卡片/规范源码生成或引用，避免抄出第二份代码。
- 新测试 `tests/round02/test_<domain>.cpp`，无依赖测试程序；可在测试内部包命名空间隔离同名全局变量，但纸面源代码不加 cp::。
- 每人只修改分配的文件；不得 commit/push、修改原文件或修改他人测试。主流程统一整合、测试、生成文件与推送。

## 模块元数据字段

沿用 `notes/modules/*.json` 的 id/title/category/priority/source/when/contract/complexity/pitfalls/example/usage/test/verify，增加：

- `idea`：一两句核心思想。
- `proof`：关键正确性理由。
- `origins`：数组，每项 `{path, idea, issue, change, interface_change, online_status}`；path 用 `cf/...` 或 `template/...`，online_status 未核验为 UNKNOWN。
- `replaces`：可选原模块 ID 列表，用于 V2 主目录选用比赛版本；不删除原版本。
- `dependencies`：必需的其他模块 ID 列表，默认 []。
- `patch`：是否适合短补丁，默认 false；只对真正高频短模块设 true。
- `verify` 在实测前写 PENDING，实测后写具体命令/测试范围，不凭推理写 PASS。

模块尽量小，代码行宽建议 85 列以内；不用刻意压成一行损害调试。

## 模型卡字段

每张卡需有 `id,title,signal,observe,brute,redundancy,derive,conditions,proof,code_refs,complexity,pitfalls,exercise,origins,patch`。

- signal 是题意识别信号；conditions 区分必要条件/充分性及取值限制。
- code_refs 是模块 ID 数组，可再用 `usage` 给真实调用，所有可执行代码必须进入实际测试。
- exercise 可用用户本地练习路径及明确的局部模型；无法确认原题时不得猜测题意。
- origins 可列来源路径，不把局部模型误当完整题解。
- 字段内容用简洁中文字符串或字符串数组，卡片必须有具体推导、反例，不能用同一套泛化套话填满。

## 验证

GNU++20，-Wall -Wextra -Wshadow；有效边界、固定种子20261024的独立差分，适合时ASan/UBSan。旧代码只作最小修改时记录具体反例；保留正确实现时避免无价值重复重构。原 read 的 EOF 检测、超域/特殊读入需求已被用户指示改用 cin，不将原 read 宣称支持这些输入。

编译器：Windows g++15.2；WSL Ubuntu g++13.3。Python 使用 C:/Users/TSOU/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe。脚本命令用 login:false。不要在同一个 build 文件名上竞争。
