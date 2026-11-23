# 学弟文档：目录与阅读顺序

> 本目录是给学员看的全部文档。按下面的顺序读，不要跳步。
> 仓库默认分支是 `dev`，日常任务都在 `dev` 上完成；`main` 由导师维护。
> 仓库根入口见 [`../../README.md`](../../README.md)。

## 1. 阅读顺序

| 顺序 | 文档 | 目的 | 预计 |
|---|---|---|---|
| 1 | [`../../README.md`](../../README.md) | 知道仓库是做什么的、怎么构建 | 5 分钟 |
| 2 | [`environment.md`](environment.md) | 装好环境，能跑通 `colcon build` | 0.5 天 |
| 3 | [`linux-basics.md`](linux-basics.md) | 非常粗略地认识终端、文件、进程、环境变量 | 30 分钟 |
| 4 | [`modern-cpp.md`](modern-cpp.md) | 知道本项目会用到的现代 C++ 子集 | 1 小时 |
| 5 | [`cpp-build.md`](cpp-build.md) | 宏观了解源码怎么变成可执行文件 | 40 分钟 |
| 6 | [`roadmap.md`](roadmap.md) | 知道学什么、不学什么、学到什么程度 | 10 分钟 |
| 7 | [`cpp-style.md`](cpp-style.md) | 写代码前扫一遍规范 | 15 分钟 |
| 8 | [`doc-style.md`](doc-style.md) | 知道文档/任务描述怎么写 | 10 分钟 |
| 9 | [`task-workflow.md`](task-workflow.md) | 知道怎么认领任务、开分支、提 PR | 15 分钟 |
| 10 | [`tasks/01-nodes-and-topics.md`](tasks/01-nodes-and-topics.md) | 第一个任务：节点与话题 | 1–1.5 天 |
| 11 | [`tasks/02-parameters-and-launch.md`](tasks/02-parameters-and-launch.md) | 参数与 launch | 1.5 天 |
| 12 | [`tasks/03-interfaces-and-services.md`](tasks/03-interfaces-and-services.md) | 自定义 msg/srv 与服务 | 2 天 |
| 13 | [`tasks/04-capstone-monitor.md`](tasks/04-capstone-monitor.md) | 可选：旁路监控进程 | 1–1.5 天 |

卡住时随时查 [`faq.md`](faq.md)。第 2、9 步不能跳：环境不对、流程不熟，后面全是无效功。

## 2. 按主题索引

| 主题 | 文档 | 什么时候读 |
|---|---|---|
| 环境、构建、sanitizer、clangd | [`environment.md`](environment.md) | 第一天；环境出问题时 |
| Linux 基础命令与概念 | [`linux-basics.md`](linux-basics.md) | 第一天；命令看不懂时 |
| 现代 C++（本项目用到的部分） | [`modern-cpp.md`](modern-cpp.md) | 写第一行代码前 |
| C++ 编译、链接、CMake、colcon | [`cpp-build.md`](cpp-build.md) | 第一课之前；构建报错时 |
| 课程路线与时间 | [`roadmap.md`](roadmap.md) | 开始前、每周末 |
| 代码规范 | [`cpp-style.md`](cpp-style.md) | 写代码前；review 前自查 |
| 文档规范与任务文档写法 | [`doc-style.md`](doc-style.md) | 写文档/任务说明时 |
| 任务流程（分支、PR、验收） | [`task-workflow.md`](task-workflow.md) | 第一个任务前 |
| 常见报错与排查 | [`faq.md`](faq.md) | 任何命令失败时 |

## 3. 任务文档

| 编号 | 任务 | 前置 | 分支 |
|---|---|---|---|
| 01 | [节点与话题](tasks/01-nodes-and-topics.md) | 环境可用 | `task/01-nodes-topics` |
| 02 | [参数与 launch](tasks/02-parameters-and-launch.md) | Task 01 已合并 | `task/02-params-launch` |
| 03 | [自定义 msg/srv 与服务](tasks/03-interfaces-and-services.md) | Task 02 已合并 | `task/03-interfaces-services` |
| 04 | [旁路监控进程（可选）](tasks/04-capstone-monitor.md) | Task 03 已合并 | `task/04-capstone-monitor` |

每个任务文档的结构一致：学习目标 → 交付物 → 骨架说明 → 验收命令 → 约束 → 三级提示 →
自检清单 → DoD → 提交要求。写法见 [`doc-style.md`](doc-style.md)。

## 4. 代码目录与文档对应

| 代码目录 | 对应任务 | 包内说明 |
|---|---|---|
| `src/training_pubsub` | Task 01 | [`../../src/training_pubsub/README.md`](../../src/training_pubsub/README.md) |
| `src/training_params` | Task 02 | [`../../src/training_params/README.md`](../../src/training_params/README.md) |
| `src/training_interfaces` | Task 03 | [`../../src/training_interfaces/README.md`](../../src/training_interfaces/README.md) |
| `src/training_sensor` | Task 03 | [`../../src/training_sensor/README.md`](../../src/training_sensor/README.md) |
| `src/training_capstone` | Task 04（可选） | [`../../src/training_capstone/README.md`](../../src/training_capstone/README.md) |

## 5. 维护约定

- 任务要求变了：同时改 `tasks/` 下文档和 GitHub Issue，保持一份来源
- 踩过的新坑：补进 [`faq.md`](faq.md)
- 课程范围调整：先改 [`roadmap.md`](roadmap.md)，再改任务文档
- 文档写法不一致时：以 [`doc-style.md`](doc-style.md) 为准
