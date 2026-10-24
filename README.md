# ACM ICPC 铜牌导向模板

面向 CF 1500 左右、正在练习稳定建模与编码的选手。先掌握“读题 → 性质 → 证明 → 编码 → 边界测试”，再使用代码。增加模板不保证获奖。

## 先打印与复习

- [成都站急用版 DOCX](板子打印版/成都站_铜牌冲刺板子_2026.docx) · [PDF](板子打印版/成都站_铜牌冲刺板子_2026.pdf)
- [长期完整版 DOCX](板子打印版/ACM_ICPC_铜牌算法板子_完整版.docx) · [PDF](板子打印版/ACM_ICPC_铜牌算法板子_完整版.pdf)
- [正式算法索引与完整接口说明](notes/INDEX.md)
- [每个模块的可靠性、手敲成本与选用判断](notes/MODULE_SELECTION.md)
- [思维卡片](notes/thinking_cards.md) · [训练计划](BRONZE_TRAINING_PLAN.md)
- [2024/2025 成都 26 题研究](CHENGDU_2024_2025_ANALYSIS.md) · [2026 资料规则证据](notes/contest_rules.md)

急用版仅收 P0。完整版增加经过验证的 P1 与精选 P2，不试图收齐全部高级算法。所有纸质数量与规格应以本届赛站确认结果为准。

## 正式代码与旧代码

`core/ data_structure/ graph/ dp/ math/ string/ advanced/` 是正式来源；`notes/modules/*.json` 给出识别信号、接口、复杂度、错误、例题与测试入口。代码中 `cp::` 等命名空间用于避免同场拼接重名；按模块说明调用即可。

根目录的 32 个历史 `.cpp` **原样保留，不是 READY 清单**。先读 [完整旧模板审计](TEMPLATE_AUDIT.md) 和 [旧代码追溯入口](legacy/README.md)。源码编译成功不等于题目 AC，历史文件的在线评测状态见证据说明。

## 测试与复现

需要 Python 3.10+ 和 GNU g++ 支持 C++20。从仓库根执行：

```sh
python3 scripts/run_tests.py
python3 scripts/run_tests.py --sanitize
```

第一条逐个编译独立驱动并运行功能/边界/随机差分测试；第二条在 Linux 上启用 ASan 与 UBSan。Windows MinGW 可执行第一条；第二条建议 WSL Ubuntu。详细已执行结果、随机种子、规模、源代码哈希和边界约束见 [验证报告](VALIDATION_REPORT.md)。构建产物位于忽略的 `build/`。

## 重新生成打印版

只修改正式源码及对应 JSON，不直接修改生成的 Word 代码段。先运行上面的两个测试，再执行：

```sh
python3 -m pip install -r scripts/requirements-docs.txt
python3 scripts/build_index.py
python3 scripts/build_notebooks.py --book both
```

生成器核对两套测试的 PASS 和每份源码 SHA256，拒绝打印测试之后发生变化的源码。DOCX 源码段与正式文件逐行一致，并输出 `.manifest.json`。

Windows 安装 Word 时，用 PowerShell 对每份文档执行：

```powershell
./scripts/export_word.ps1 -Docx './板子打印版/成都站_铜牌冲刺板子_2026.docx' -Pdf './板子打印版/成都站_铜牌冲刺板子_2026.pdf'
./scripts/export_word.ps1 -Docx './板子打印版/ACM_ICPC_铜牌算法板子_完整版.docx' -Pdf './板子打印版/ACM_ICPC_铜牌算法板子_完整版.pdf'
python scripts/check_print.py
```

也可执行 `python3 scripts/export_pdf.py`：Windows 调用 Word，Linux 调用 LibreOffice headless，并为当前 DOCX/PDF 写入导出哈希配对。`check_print.py` 会拒绝缺失或过期的配对，防止重建 Word 后误交旧 PDF。不同字体/排版引擎可能改变分页，必须重新核查索引与布局，不能照搬本次页数。安装微软雅黑与 Consolas 或替换为等宽且支持中文的字体，再检查输出。文档门禁回归测试：`python3 tests/test_print_gate.py`。

使用 Poppler 渲染，例如：

```sh
python3 scripts/render_pdf.py '板子打印版/成都站_铜牌冲刺板子_2026.pdf' --out _qa/emergency
```

所有页面都要检查。结构检查可发现分页和源码不一致，不能代替目视检查缺字、截断或重叠。

## 与 cf 子模块的更新顺序

本仓库是唯一模板权威源。`ball-castle/cf` 的 `template` 为子模块，不要复制一份独立修改。

1. 在本仓库分支完成修改、测试、生成打印文件并提交、推送。
2. 先合并本仓库 PR，保留 cf 将引用的提交可从远端获取。
3. 在 cf 的独立分支更新 `template` 至已发布提交，`git add template` 提交 gitlink，再合并 cf PR。
4. 新机器用 `git clone --recurse-submodules https://github.com/ball-castle/cf.git`，已有克隆用 `git submodule update --init --recursive`。

详细本次交付提交与限制见 [变更说明](CHANGELOG.md)。本轮不删除历史训练代码或已跟踪的构建产物，不改写历史。
