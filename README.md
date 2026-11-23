# rus_ros2_training

ROS 2 Humble + C++17 入门训练仓库。目标不是「学完 ROS 2」，而是让新同学能独立写出
多个进程组成的小系统，并能用命令行自查问题，然后进入 `RUS_Engine` 做真实任务。

- 仓库根目录就是 colcon 工作区：`clone` 后直接 `colcon build`
- 三个必修模块：节点与话题 → 参数与 launch → 自定义 msg/srv
- 一个可选模块：旁路监控进程（为进入 `RUS_Engine` 做准备）
- **不提供参考答案**：任务用 Issue 发布，靠自测命令、CI 和代码审查推进

完整文档目录与阅读顺序见 [`docs/student/README.md`](docs/student/README.md)。

## 前置

- Ubuntu 22.04 + ROS 2 Humble
- `g++` / `cmake` / `colcon` / `clang-format`
- 能读写本仓库的 GitHub 权限；仓库默认分支是 `dev`，`main` 由导师维护

环境安装与验证见 [`docs/student/environment.md`](docs/student/environment.md)。

## 快速开始

```bash
# 每个新终端都要先 source ROS 2
source /opt/ros/humble/setup.bash
cd rus_ros2_training

colcon build --symlink-install
source install/setup.bash

# 例如跑第一课的实现（完成 TODO 之后）
ros2 run training_pubsub temperature_publisher
```

## 课程

| 模块 | 内容 | 代码目录 | 任务文档 |
|---|---|---|---|
| M0 | 命令行观察节点与话题（不写代码） | — | [01](docs/student/tasks/01-nodes-and-topics.md) 的开头 |
| M1 | 发布者 / 订阅者、多进程 | `src/training_pubsub` | [01-nodes-and-topics](docs/student/tasks/01-nodes-and-topics.md) |
| M2 | 参数、launch、namespace | `src/training_params` | [02-parameters-and-launch](docs/student/tasks/02-parameters-and-launch.md) |
| M3 | 自定义 msg/srv + 服务 | `src/training_interfaces`、`src/training_sensor` | [03-interfaces-and-services](docs/student/tasks/03-interfaces-and-services.md) |
| M4（可选） | 旁路监控进程 | `src/training_capstone` | [04-capstone-monitor](docs/student/tasks/04-capstone-monitor.md) |

路线、时间与刻意省略的内容见 [`docs/student/roadmap.md`](docs/student/roadmap.md)。

## 背景知识（宏观了解）

开始写代码前建议先过一遍，不要求记住命令：

- [`docs/student/linux-basics.md`](docs/student/linux-basics.md)：终端、文件、进程、环境变量（粗略版）
- [`docs/student/modern-cpp.md`](docs/student/modern-cpp.md)：本项目会用到的现代 C++ 子集
- [`docs/student/cpp-build.md`](docs/student/cpp-build.md)：源码 → 目标文件 → 链接 → 可执行文件

## 工作方式

1. 在 GitHub Issue 里认领任务（一个任务一个 Issue）
2. 从最新 `dev` 开分支：`task/01-nodes-topics`
3. 实现 `TODO`，按任务文档的验收命令逐条自测
4. 提交 PR 到 `dev`，贴上关键命令输出；CI 必须绿
5. 代码审查通过后 squash merge 到 `dev`；导师定期把 `dev` 合并到 `main`

详细规则见 [`docs/student/task-workflow.md`](docs/student/task-workflow.md)。

## 代码与文档约定

- C++17，RAII，不使用裸 `new/delete`
- `const` 正确性、`std::chrono` 表示时间、参数不写魔法数字
- 提交前跑 `clang-format`；构建要求 `-Wall -Wextra` 无警告
- 回调里不阻塞、不让异常逃出回调

代码规范见 [`docs/student/cpp-style.md`](docs/student/cpp-style.md)；
任务文档/Issue/PR 的写法见 [`docs/student/doc-style.md`](docs/student/doc-style.md)。

## 目录

```text
rus_ros2_training/
├── docs/student/          # 学弟文档：基础、规范、流程、任务
├── src/
│   ├── training_pubsub/   # M1
│   ├── training_params/   # M2
│   ├── training_interfaces/
│   ├── training_sensor/   # M3
│   └── training_capstone/ # M4（可选）
└── .github/               # Issue / PR 模板、CI
```

## 关于「没有答案」

仓库里只有骨架和 `TODO`。卡住时按顺序做：

1. 读任务文档的「提示」和官方文档
2. 把报错、已尝试的命令、你的猜测一起发到 Issue 评论
3. 超过 2 小时仍无进展再找导师；第三级提示只会在 review 时口头/评论给出

不要把网上整段代码直接贴进 PR。审查会要求你解释每一行的作用。
