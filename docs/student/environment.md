# 环境与构建

## 1. 版本基线

| 组件 | 版本 |
|---|---|
| Ubuntu | 22.04 |
| ROS 2 | Humble |
| C++ | C++17 |
| CMake | >= 3.18 |
| clang-format | 14 |
| colcon | 随 ROS 2 安装 |

## 2. 验证环境

```bash
printenv ROS_DISTRO          # 应输出 humble
ros2 pkg list | head         # 有输出即正常
g++ --version
cmake --version
colcon version-check         # 可选
clang-format --version
```

每个**新终端**都要先 source：

```bash
source /opt/ros/humble/setup.bash
```

## 3. 构建与运行

```bash
cd rus_ros2_training
colcon build --symlink-install
source install/setup.bash
```

只构建某个包（迭代时更快）：

```bash
colcon build --symlink-install --packages-select training_pubsub
colcon build --symlink-install --packages-up-to training_sensor
```

修改代码后必须重新 `colcon build`，再 `source install/setup.bash`。
改了 msg/srv 时，先构建接口包：

```bash
colcon build --symlink-install --packages-select training_interfaces
source install/setup.bash
```

## 4. clang-format

```bash
# 格式化自己改的文件
clang-format -i src/training_pubsub/src/*.cpp

# 只检查，不改文件
git ls-files -z '*.cpp' '*.hpp' '*.h' | xargs -0 -r clang-format --dry-run --Werror
```

仓库根目录的 `.clang-format` 是唯一格式标准，不要改。

## 5. clangd / VSCode

生成编译数据库：

```bash
colcon build --symlink-install --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

`compile_commands.json` 会出现在各包的 `build/` 下。VSCode 的 clangd 插件默认找不到时，
在 `.vscode/settings.json` 里指定 `--compile-commands-dir`，或使用 `colcon` 的合并脚本。

## 6. Sanitizer（本地必须跑）

M2、M3 完成时各跑一次。ASan 能抓到 use-after-free、越界；UBSan 抓未定义行为。

```bash
colcon build --packages-select training_pubsub \
  --cmake-args \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"
```

运行节点前建议：

```bash
export ASAN_OPTIONS=detect_leaks=0
```

说明：ROS 2 / DDS 自身可能产生大量 leak 报告，初期先关闭 leak 检测，重点看
use-after-free、越界、未定义行为。关闭 leak 不等于可以随便 `new`。

跑完记得恢复正常构建：

```bash
colcon build --symlink-install
```

## 7. 常见环境问题

| 现象 | 处理 |
|---|---|
| `ros2: command not found` | 当前终端没 source ROS 2 |
| `Package 'xxx' not found` | 没 source `install/setup.bash`，或包还没构建 |
| 改了 msg 但编译不到新字段 | 先构建接口包、再 source、再构建使用方 |
| 节点能起但收不到话题 | 检查 namespace、话题名、是否 source 了同一个工作区 |
| WSL / 虚拟机里跨机器发现不到节点 | 设置 `export ROS_LOCALHOST_ONLY=1`，或统一 RMW |
| 构建很慢 | `--packages-select` 只构建相关包 |
| `undefined reference` 到消息类型 | 检查 `package.xml` 是否 depend 接口包、CMake 是否使用了 `ament_target_dependencies` |

更多见 [`faq.md`](faq.md)。
