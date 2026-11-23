# training_pubsub（M1）

Task 01 的代码骨架：两个可执行、一条话题。

| 可执行 | 话题 | 说明 |
|---|---|---|
| `temperature_publisher` | 发布 `/temperature` | 1 Hz，`std_msgs/msg/Float64` |
| `temperature_subscriber` | 订阅 `/temperature` | 打印数值，超过阈值告警 |

## 构建与运行

```bash
colcon build --symlink-install --packages-select training_pubsub
source install/setup.bash

ros2 run training_pubsub temperature_publisher
ros2 run training_pubsub temperature_subscriber   # 另开终端
```

## 只改这里

- `src/temperature_publisher.cpp`
- `src/temperature_subscriber.cpp`

`CMakeLists.txt` / `package.xml` 这一课不需要修改。
完整任务见 [`../../docs/student/tasks/01-nodes-and-topics.md`](../../docs/student/tasks/01-nodes-and-topics.md)。
