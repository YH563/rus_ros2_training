# Task 01：节点与话题

| 模块 | 预计时间 | 前置 | 分支 |
|---|---|---|---|
| M1 | 1–1.5 天 | 环境可用 | `task/01-nodes-topics` |

## 1. 学习目标

- 理解 node 是进程边界，topic 是跨进程的消息契约
- 会写发布者和订阅者，会设置定时器与回调
- 会用 `ros2 node/topic` 命令观察和自查
- 顺带养成 `const`、`std::chrono`、日志节流等 C++ 习惯

## 2. 开始之前：先用命令行看一遍（约 30 分钟）

不写代码，先看两个进程如何通过一条话题通信。开两个终端，都先 source：

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker      # 终端 1
ros2 run demo_nodes_cpp listener    # 终端 2
```

第三个终端观察：

```bash
ros2 node list
ros2 topic list
ros2 topic info /chatter --verbose
ros2 topic echo /chatter --once
```

回答三个问题（写进 PR 描述）：

1. `/chatter` 的发布者和订阅者分别是谁？
2. 两个节点是两个进程还是两个线程？
3. `ros2 topic hz /chatter` 显示多少？

## 3. 交付物

在 `src/training_pubsub` 中完成：

| 可执行 | 行为 |
|---|---|
| `temperature_publisher` | 以 1 Hz 向 `/temperature` 发布 `std_msgs/msg/Float64`，数值为 `20.0 + 10.0 * sin(运行秒数)` |
| `temperature_subscriber` | 订阅 `/temperature`，打印数值；大于 `30.0` 时打印 WARN |

发布者每次发布用 `RCLCPP_INFO` 打印数值，但不要每秒刷很多条。

## 4. 骨架说明

- 可以改：`src/temperature_publisher.cpp`、`src/temperature_subscriber.cpp` 里的 `TODO`
- 不要改：`CMakeLists.txt`、`package.xml`（这一课不需要动构建文件）
- 不要新增第三方依赖

## 5. 验收

按顺序执行，把关键输出贴进 PR：

```bash
colcon build --symlink-install --packages-select training_pubsub
source install/setup.bash

# 终端 1
ros2 run training_pubsub temperature_publisher

# 终端 2
ros2 node list
ros2 topic info /temperature --verbose
timeout 5 ros2 topic hz /temperature
ros2 topic echo /temperature --once

# 终端 3
ros2 run training_pubsub temperature_subscriber
```

预期：

- `ros2 node list` 能看到 `temperature_publisher` 和 `temperature_subscriber`
- 频率约 1 Hz（0.9–1.1 之间都算通过）
- `echo` 输出 `data` 字段，数值在 10–30 之间变化
- 订阅者终端能看到数值；超过 30 时出现 WARN

## 6. 约束

- C++17；不使用裸 `new/delete`；不使用 `sleep`
- 回调里不阻塞
- 不修改构建文件
- 代码通过 `clang-format`

## 7. 提示

**提示 1**：读 ROS 2 官方教程的「Writing a simple publisher and subscriber (C++)」，
只读 `rclcpp` 部分，不要照抄整段。

**提示 2**：这一课会用到 `create_publisher`、`create_wall_timer`、`create_subscription`、
`RCLCPP_INFO`；时间用 `std::chrono_literals`。

**提示 3**：仅当你在 Issue 里说明已尝试超过 2 小时后，导师会在评论里给关键片段。

## 8. 自检清单

- [ ] 两个可执行都能 `ros2 run` 起来
- [ ] `ros2 node list` 能看到两个节点
- [ ] `ros2 topic hz` 约 1 Hz
- [ ] `ros2 topic echo` 有数据
- [ ] 订阅者能在 >30 时告警
- [ ] 发布者/订阅者对象作为成员保存，没有被提前销毁
- [ ] 构建 0 warning，`clang-format` 干净
- [ ] PR 描述里回答了第 2 节的三个问题

## 9. 完成定义（DoD）

- 验收命令逐条跑过，输出贴在 PR
- CI 绿
- 无 `must-fix` 遗留
- 能口头说清：谁发布、谁订阅、数据从哪个进程到哪个进程

## 10. 提交要求

- 分支：`task/01-nodes-topics`
- 提交信息：`task-01: ...`
- PR 标题：`Task 01: 节点与话题`
- PR 描述包含：数据流、验收输出、三个观察问题的答案
