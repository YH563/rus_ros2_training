# 路线图

> 开始前建议先扫一遍 [`linux-basics.md`](linux-basics.md) 和 [`cpp-build.md`](cpp-build.md)，
> 只求建立宏观印象，不要求记住命令。

## 1. 结业目标

完成 M1–M3 后，应当能：

- 独立写 2–3 个 C++17 ROS 2 节点，用 topic/service 组成多进程小系统
- 用参数和 launch 改变行为而不重编译
- 定义 msg/srv，理解接口包为什么独立、依赖方向为什么重要
- 用 `ros2 node/topic/service/param/interface` 命令定位常见问题
- 按 Issue → 分支 → 自测 → PR → CI → 审查 的流程交付代码

## 2. 课程安排

| 模块 | 主题 | 预计时间 | 前置 | 产出 |
|---|---|---|---|---|
| M0 | 命令行观察节点与话题 | 0.5 天 | 环境可用 | 一张节点/话题关系图（口头或笔记） |
| M1 | 发布者 / 订阅者 | 1–1.5 天 | M0 | `training_pubsub` 两个可执行 |
| M2 | 参数、launch、namespace | 1.5 天 | M1 | `training_params` 参数化节点 + launch |
| M3 | 自定义 msg/srv + 服务 | 2 天 | M2 | `training_interfaces` + `training_sensor` |
| M4（可选） | 旁路监控进程 | 1–1.5 天 | M3 | `training_capstone` |

每天结束前留 15 分钟：跑一遍验收命令、记录卡点、把新问题补进 `faq.md`。

## 3. 各模块重点

### M0 观察

- 跑 `demo_nodes_cpp` 的 talker/listener
- 用 `ros2 node list`、`ros2 topic list/info --verbose`、`ros2 topic hz/echo`
- 目标：亲眼看到「两个进程 + 一条话题」，不写代码

### M1 节点与话题

- 一个 1 Hz 发布者，一个订阅者
- 理解 publisher/subscriber、回调、定时器
- C++ 重点：`std::chrono`、`const` 正确性、成员初始化、日志节流
- **不教**：QoS、executor 配置、launch

### M2 参数与 launch

- 参数：`sensor_id`、`publish_rate_hz`、`alarm_threshold`
- 动态修改参数立即生效；非法值被拒绝
- launch 起两份实例（不同 namespace、不同参数）
- C++ 重点：默认值、参数校验、定时器重建顺序

### M3 接口与服务

- 定义 `SensorReading.msg`、`ResetCounter.srv`
- 节点发布自定义消息，提供重置服务
- 理解接口包、`package.xml` 依赖、CMake 链接
- C++ 重点：生成的结构体、值语义、`const&` 传参、`[[nodiscard]]`
