# 常见问题

> 命令、进程、环境变量的基础见 [`linux-basics.md`](linux-basics.md)；
> 编译/链接错误的原理见 [`cpp-build.md`](cpp-build.md)。

## 环境

**`ros2: command not found`**

当前终端没有 source：

```bash
source /opt/ros/humble/setup.bash
```

**`Package 'training_xxx' not found`**

```bash
colcon build --symlink-install
source install/setup.bash
```

`source` 必须在 `colcon build` 之后，且每个新终端都要做。

## 构建

**改了 msg/srv，使用方编译报字段不存在**

先构建接口包，再构建使用方：

```bash
colcon build --symlink-install --packages-select training_interfaces
source install/setup.bash
colcon build --symlink-install --packages-up-to training_sensor
```

**`undefined reference` 到消息类型**

使用接口的包没有正确声明/链接依赖。检查：

`package.xml`：

```xml
<depend>training_interfaces</depend>
```

`CMakeLists.txt`：

```cmake
find_package(training_interfaces REQUIRED)
ament_target_dependencies(你的目标 rclcpp training_interfaces)
```

如果接口和使用方在同一个包里（用 `rosidl_generate_interfaces` 直接生成并链接），
才需要 `rosidl_get_typesupport_target` + `target_link_libraries`。

**构建出现 warning**

不要忽略。`-Wall -Wextra` 下的 warning 通常是真问题：未使用变量、成员初始化顺序、
有符号/无符号比较。

## 运行

**`ros2 node list` 看不到节点**

- 进程是否真的还在运行（看终端有没有退出）
- 是否 source 了同一个工作区
- 节点名是否和预期一致（`ros2 node list` 会显示 namespace）

**`ros2 topic hz` 没有输出**

- 话题名是否写对（`ros2 topic list`）
- 发布者和检查命令是否在同一个 ROS_DOMAIN_ID / 网络
- 发布者是否真的在发布（`ros2 topic echo`）

**收不到消息，但发布者在发**

- 检查 namespace：`/sensor_a/temperature` 和 `/temperature` 是两条话题
- 检查 QoS：默认 reliable，如果一边是 best effort，可能表现异常
- WSL/虚拟机跨主机：`export ROS_LOCALHOST_ONLY=1` 试验

**`ros2 param set` 后行为没变**

- 节点实现了参数回调吗？
- 回调里是否真的重建了定时器（旧定时器先 `cancel()`）
- `ros2 param get` 确认值已更新

**节点名冲突**

两个同名节点在同一 namespace 下会冲突。用 launch 的 `namespace` 或 `name` 区分。

## 接口

**`ros2 interface show` 还是占位字段**

说明 `msg/` / `srv/` 文件还没改，或改完没有重新构建接口包。

**服务调用返回 `success=false`**

按任务文档检查 `sensor_id` 是否匹配；不要把所有情况都返回成功。

## Sanitizer

**ASan 报一堆 leak**

先设 `ASAN_OPTIONS=detect_leaks=0` 排除 DDS 自身的噪声，重点看 use-after-free / 越界。
如果关闭 leak 后仍有报告，按栈顶定位到自己的代码。

**Sanitizer 构建后程序变慢**

正常。验证完恢复普通构建：

```bash
colcon build --symlink-install
```

## Git / CI

**push 被拒绝，提示 protected branch**

`dev` / `main` 不能直接 push。从最新 `dev` 开任务分支，PR 合回 `dev`：

```bash
git checkout dev
git pull origin dev
git checkout -b task/01-nodes-topics
```

**CI 失败但本地通过**

- 本地是否漏 source？
- 是否提交了未跟踪的新文件（`git status`）
- CI 用的是干净环境，确认 `package.xml` 依赖写全

**PR 里有 build/install/log**

把它们从暂存区移除，并确认 `.gitignore` 生效：

```bash
git rm -r --cached build install log
```
