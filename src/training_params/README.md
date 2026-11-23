# training_params（M2）

Task 02 的代码骨架：参数化节点 + launch + namespace。

| 项 | 内容 |
|---|---|
| 可执行 | `sensor_node` |
| 参数 | `sensor_id`、`publish_rate_hz`、`alarm_threshold` |
| launch | `launch/sensor.launch.py`，默认 namespace 为 `sensor_a` |
| 配置 | `config/sensor.yaml` |

## 构建与运行

```bash
colcon build --symlink-install --packages-select training_params
source install/setup.bash

ros2 launch training_params sensor.launch.py
```

## 只改这里

- `src/sensor_node.cpp`
- `launch/sensor.launch.py`

`CMakeLists.txt` / `package.xml` 这一课不需要修改。
完整任务见 [`../../docs/student/tasks/02-parameters-and-launch.md`](../../docs/student/tasks/02-parameters-and-launch.md)。
