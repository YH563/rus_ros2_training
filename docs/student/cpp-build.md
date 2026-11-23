# C++ 编译与构建：从源码到进程

> 目标是从宏观上知道「一行 C++ 怎么变成一个能跑的进程」，以及出错时该看哪一层。
> 不需要背编译器参数，但要知道编译错误、链接错误、运行时错误分别意味着什么。

## 1. 为什么 ROS 2 开发要懂这些

你每天会跑：

```bash
colcon build --symlink-install
source install/setup.bash
ros2 run training_pubsub temperature_publisher
```

这三条命令背后是：

1. `colcon` 调用 CMake，CMake 生成构建文件，编译器把 `.cpp` 编成目标文件，链接器拼成可执行文件
2. `source` 把「去哪里找包、找库」写进当前 shell 的环境变量
3. `ros2 run` 在 `PATH`/`AMENT_PREFIX_PATH` 里找到可执行文件并启动进程

分清「编译期 / 链接期 / 运行期」，能让你少走很多弯路。

## 2. 四个阶段

```text
main.cpp + 头文件
   │  预处理：展开 #include、#define、条件编译
   ▼
预处理后的翻译单元
   │  编译：语法/类型检查，生成汇编
   ▼
汇编代码
   │  汇编：生成目标文件 .o / .obj
   ▼
目标文件
   │  链接：合并目标文件，解析符号，链接库
   ▼
可执行文件 / 共享库
   │  运行：加载器把 .so 和可执行文件映射进内存
   ▼
进程
```

手工看每一步（了解即可）：

```bash
g++ -E main.cpp -o main.i     # 预处理
g++ -S main.cpp -o main.s     # 编译成汇编
g++ -c main.cpp -o main.o     # 编译成目标文件
g++ main.o -o main            # 链接
ldd ./main                    # 看运行时依赖哪些 .so
nm -C main.o | head           # 看目标文件里的符号
```

## 3. 头文件、翻译单元与 ODR

- 一个 `.cpp` 加上它 `#include` 进来的所有内容，叫一个**翻译单元**
- 头文件（`.hpp`/`.h`）通常只放**声明**：函数原型、类定义、模板；实现放 `.cpp`
- 头文件要被多个 `.cpp` 包含，所以需要 `#pragma once` 或 include guard，避免重复定义
- **ODR（单一定义规则）**：同一个函数/变量在程序里只能有一份定义（`inline`、模板等例外）
- 头文件里直接写普通函数定义，且被多个 `.cpp` 包含 → 链接时 `multiple definition`

```cpp
// counter.hpp
#pragma once
int next_id();          // 声明

// counter.cpp
#include "counter.hpp"
int next_id() { ... }   // 定义，只能有一处
```

`#include "x.hpp"` 先在当前目录找，再找 include 路径；`#include <x.hpp>` 直接找系统/依赖的
include 路径。找不到头文件时，先确认包依赖和 include 路径，而不是改代码。

## 4. 链接与库

链接器的工作是：把目标文件里的「未定义符号」和库里的「定义」对上。

- **静态库** `.a`：编译时把代码复制进可执行文件，体积大，部署简单
- **共享库** `.so`：运行时加载，多个程序共用，需要能找到它

常见参数：

| 参数 | 含义 |
|---|---|
| `-I/path` | 头文件搜索路径 |
| `-L/path` | 库搜索路径 |
| `-lname` | 链接 `libname.so` 或 `libname.a` |
| `-Wl,-rpath,/path` | 把运行时库路径写进可执行文件 |
| `LD_LIBRARY_PATH` | 运行时额外的库搜索路径 |

常见链接错误：

| 报错 | 通常原因 |
|---|---|
| `undefined reference to ...` | 没链接库、链接顺序不对、只有声明没有定义 |
| `multiple definition of ...` | 头文件里放了定义，或重复编译同一实现 |
| `cannot find -lxxx` | `-L` 路径不对，或库没安装 |
| `error while loading shared libraries` | 运行期找不到 `.so`，用 `ldd` 看缺哪个 |
| `recompile with -fPIC` | 共享库的源文件没按位置无关方式编译 |

`undefined reference` 是**链接期**错误，不是语法错误；先看符号属于哪个库/哪个包。

## 5. 构建系统：Make、CMake、colcon

- **Make/Ninja**：底层执行器，按依赖关系决定「先编谁、后编谁」
- **CMake**：跨平台的构建描述语言，生成 Make/Ninja 文件
- **colcon**：ROS 2 的工作区编排器，按包依赖顺序调用每个包的 CMake，并管理 `install/`

CMake 的四个阶段：

```text
configure  → 读 CMakeLists.txt，检查依赖，生成缓存
generate   → 生成 Makefile / build.ninja
build      → 编译、链接
install    → 把头文件、库、可执行文件、launch 复制到 install/
```

一个最小示例：

```cmake
cmake_minimum_required(VERSION 3.18)
project(demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(rclcpp REQUIRED)

add_executable(demo src/main.cpp)
ament_target_dependencies(demo rclcpp)
```

要点：

- `add_executable` / `add_library` 定义**目标**
- `find_package` 找依赖，`ament_target_dependencies` 把依赖的头文件/库接到目标上
- `target_link_libraries` 直接链接库
- `install(TARGETS ...)` 决定产物装到哪里；没 install 就 `ros2 run` 不到

## 6. ROS 2 工作区里的构建

```text
src/       源码（package.xml + CMakeLists.txt）
build/     每个包的 CMake 缓存与中间产物
install/   最终安装结果（可执行、库、头文件、launch、config）
log/       colcon 日志
```

`package.xml` 声明包依赖，colcon 据此决定构建顺序；CMake 再根据 `find_package`
找到依赖的头文件和库。

三个经常被问的问题：

**为什么改了 msg/srv，要先构建接口包？**

消息会生成 C++ 头文件和类型支持库，安装到 `install/`。使用方编译时读的是安装后的头文件，
所以必须先生成、再 `source`、再编译使用方。

**为什么 `source install/setup.bash` 这么重要？**

它把 `install/` 加入 `AMENT_PREFIX_PATH`、`LD_LIBRARY_PATH`、`PATH`。
不 source，`ros2 run` 找不到包，运行时也可能找不到 `.so`。

**为什么改了 CMake 参数有时要清缓存？**

CMake 会把检测结果缓存在 `build/<包>/CMakeCache.txt`。切换编译器、构建类型或关键选项后，
旧缓存可能残留。需要时用：

```bash
colcon build --cmake-clean-cache --packages-select training_pubsub
```

## 7. 编译选项与构建类型

```bash
colcon build --symlink-install \
  --cmake-args -DCMAKE_BUILD_TYPE=Debug
```

| 构建类型 | 特点 | 用途 |
|---|---|---|
| `Debug` | `-O0 -g`，可断点调试 | 定位问题、跑 sanitizer |
| `Release` | `-O3`，无调试信息 | 性能测试/部署 |
| `RelWithDebInfo` | `-O2 -g` | 日常开发折中 |

常用 warning 选项：

```text
-Wall -Wextra -Wpedantic
```

不要一上来就加 `-Werror`：第三方头文件可能产生告警，先把**自己的代码**清零。

Sanitizer 是编译期插桩，必须重新构建：

```bash
colcon build --packages-select training_pubsub \
  --cmake-args \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"
```

不同构建类型/编译器的 ABI 可能不同，不要把一个 `Debug` 编译的库混进 `Release` 程序里。

## 8. 怎么读构建错误

1. **只看第一条 error**：后面的错误经常是第一条的连锁反应
2. 分清阶段：
   - `fatal error: xxx.hpp: No such file` → 预处理/头文件路径
   - `error: ...` → 编译期语法/类型
   - `undefined reference` → 链接期
   - `error while loading shared libraries` → 运行期
3. 看完整命令：

```bash
colcon build --packages-select training_sensor \
  --event-handlers console_direct+ \
  --cmake-args -DCMAKE_VERBOSE_MAKEFILE=ON
```

4. 缩小范围：先只构建出错包，再 `--packages-up-to` 连带依赖

常见组合：

```bash
colcon build --symlink-install --packages-select training_pubsub
colcon build --symlink-install --packages-up-to training_sensor
colcon build --symlink-install --cmake-clean-cache --packages-select training_params
```

## 9. 与本训练仓库的对应

| 你看到的东西 | 对应概念 |
|---|---|
| `package.xml` 里的 `<depend>` | 包依赖，colcon 排序与 rosdep 安装 |
| `CMakeLists.txt` 里的 `add_executable` | 构建目标 |
| `ament_target_dependencies` | 把头文件/库接到目标 |
| `install/` 下的可执行文件 | `ros2 run` 实际运行的东西 |
| 改 msg 后先构建接口包 | 生成代码 + 安装 + 依赖顺序 |
| `build/`、`install/`、`log/` 不提交 | 构建产物，可重建 |

## 10. 自检

1. 编译错误和链接错误分别发生在哪个阶段？
2. 头文件里写了函数定义，被两个 `.cpp` 包含，会发生什么？
3. `undefined reference` 时，你会先看什么？
4. 为什么 `ros2 run` 之前要 `source install/setup.bash`？
5. `-I`、`-L`、`-l` 分别解决什么问题？
6. 为什么 sanitizer 构建要单独做一次，不能直接复用普通构建？

## 11. 现在不用深究

- 模板元编程、SFINAE、concepts（C++20）
- ABI 细节、符号可见性、链接时优化（LTO）
- 交叉编译、静态分析、分布式构建
- 手写 Makefile（有 CMake 就够用）

先把「源码 → 目标文件 → 链接 → 进程」这条链路走顺，遇到具体错误再往下查。
